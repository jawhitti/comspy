/////////////////////////////////////////////////////////////
// ItfTypeInfo.cpp - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
/////////////////////////////////////////////////////////////
#include "stdafx.h"
//#include "ItfTypeInfo.h"
#include "thk_Win9X.h"
//#include "crt.h"

extern "C" BOOL _divineStubInfo( const void* pStubBuffer, const IID* piid,
	const BYTE** ppProcFmtString, const WORD** ppFcnOffsets, DWORD* pcMethods,
		BOOL* pbDual );
extern "C" BOOL _divineProxyInfo( const void* pItf, const IID* piid,
	const BYTE** ppProcFmtString, const WORD** ppFcnOffsets );

struct CacheRecord;

const DWORD HASH_TABLE_SIZE = 251;

static CRITICAL_SECTION s_cs;
static CacheRecord**	s_hashTable;

struct __declspec(uuid("00020424-0000-0000-C000-000000000046")) PSOAInterface;
struct __declspec(uuid("00020420-0000-0000-C000-000000000046")) PSDispatch;

const WORD IDISPATCH_STACK_SIZE_3 = 4+(4*1); // GetTypeInfoCount
const WORD IDISPATCH_STACK_SIZE_4 = 4+(4*3); // GetTypeInfo
const WORD IDISPATCH_STACK_SIZE_5 = 4+(4*5); // GetIDsOfNames
const WORD IDISPATCH_STACK_SIZE_6 = 4+(4*8); // Invoke


inline DWORD hash( const IID& iid )
{
	return iid.Data1 ^ iid.Data2 ^ iid.Data3 ^ (*(DWORD*)iid.Data4);
}

// this guy makes interface stubs happy
struct DummyObject : IUnknown
{
	STDMETHODIMP QueryInterface( REFIID iid, void** ppv )
	{
		if ( m_iid == iid || IID_IUnknown == iid )
			 return (*ppv = this), S_OK;
		else return (*ppv = 0), E_NOINTERFACE;
	}
	STDMETHODIMP_(ULONG) AddRef()  { return 2; }
	STDMETHODIMP_(ULONG) Release() { return 1; }
	const IID& m_iid;
	DummyObject( const IID& iid ) : m_iid( iid ) {}
};

struct CacheRecord : ItfTypeInfo
{
	CacheRecord*	pNext;
	IID				iid;

	virtual void Destroy() = 0;
};

struct CacheRecordOicf : CacheRecord
{
	WORD rgStackSizes[1]; // variable length array

	DWORD GetStackSize( DWORD nMethod )
	{
		return rgStackSizes[nMethod - 3];
	}

	void Destroy() { CoTaskMemFree( this ); }

	static CacheRecordOicf* Create( const IID& iid, DWORD cMethods )
	{
		CacheRecordOicf* p = (CacheRecordOicf*) CoTaskMemAlloc( sizeof( CacheRecordOicf ) + ( sizeof(WORD) * ( cMethods - 1 ) ) );
		if ( p )
		{
			new (p) CacheRecordOicf; // set up vptr
			p->pNext = 0;
			p->iid = iid;
		}
		return p;
	}
};

// Since typelib generated stubs don't provide a method count
// (at least not that's obvious -- the stub looks totally different
//  from a MIDL generated /Oicf stub), we unfortunately cannot simply
// cache the stack sizes for all methods in the interface.
// Rather, we must hold an interface proxy and lookup the requested
// method's stack size from the proc string it maintains.
// No real difference in per-method lookup time,
// rather just a difference in space.
struct CacheRecordTLB : CacheRecord
{
	// if you can believe it, CreateProxy won't give me an interface pointer
	// so I have to call Connect then QI, which of course requires a channel.
	// I also let this guy masquerade as the proxy manager to avoid another vptr
	struct DummyObject : IRpcChannelBuffer
	{
		STDMETHODIMP QueryInterface( REFIID iid, void** ppv )
		{
			// I found it interesting that I get QI'd for IClientSecurity
			// while inside Connect
			if ( IID_IUnknown == iid || IID_IRpcChannelBuffer == iid )
				 return (*ppv = this), S_OK;
			else return (*ppv = 0), E_NOINTERFACE;
		}
		STDMETHODIMP_(ULONG) AddRef()  { return 2; }
		STDMETHODIMP_(ULONG) Release() { return 1; }
		STDMETHODIMP GetBuffer( RPCOLEMESSAGE*, REFIID )	{ return E_NOTIMPL; }
		STDMETHODIMP SendReceive( RPCOLEMESSAGE*, ULONG* )	{ return E_NOTIMPL; }
		STDMETHODIMP FreeBuffer( RPCOLEMESSAGE* )			{ return E_NOTIMPL; }
		STDMETHODIMP GetDestCtx( DWORD*, void** )			{ return E_NOTIMPL; }
		STDMETHODIMP IsConnected()							{ return E_NOTIMPL; }
	};

	DummyObject			dummyObject;
	IUnknown*			pUnkForRelease;
	const BYTE*			pProcFmtString;
	const WORD*			pFcnOffsets;
	
	CacheRecordTLB() : pUnkForRelease(0) {}

	DWORD GetStackSize( DWORD nMethod )
	{
		const DWORD fcnOffset = pFcnOffsets[nMethod];
		if ( 0xFFFF == fcnOffset )
		{
			// dual interfaces have dummy entries for IDispatch
			switch ( nMethod )
			{
				case 3: return IDISPATCH_STACK_SIZE_3;
				case 4: return IDISPATCH_STACK_SIZE_4;
				case 5: return IDISPATCH_STACK_SIZE_5;
				case 6: return IDISPATCH_STACK_SIZE_6;
			}
		}
		return pProcFmtString[4 + fcnOffset] - sizeof( void* );
	}

	void Destroy()
	{
		if ( pUnkForRelease )
			pUnkForRelease->Release();
		delete this;
	}

	static CacheRecordTLB* Create( const IID& iid, HRESULT& hr )
	{

		hr = S_OK;
		CacheRecordTLB* pRecord = 0;
		void* pProxyItf = 0;
		{
			IPSFactoryBuffer* pfactory = 0;
			if ( FAILED( hr = CoGetClassObject( __uuidof(PSOAInterface),
						CLSCTX_INPROC_SERVER, 0,
						IID_IPSFactoryBuffer, (void**)&pfactory ) ) )
				return 0;
			pRecord = new CacheRecordTLB;
			if ( pRecord )
			{
				IRpcProxyBuffer* pProxyBuffer = 0;
				if ( SUCCEEDED( hr = pfactory->CreateProxy( &pRecord->dummyObject, iid,
														 &pProxyBuffer, &pProxyItf ) ) )
				{
					pRecord->pUnkForRelease = pProxyBuffer;
					if ( !pProxyItf ) // don't you love these TLB marshalers???
					{
						if ( FAILED( hr = pProxyBuffer->Connect( &pRecord->dummyObject ) ) ||
							 FAILED( hr = pProxyBuffer->QueryInterface( iid, &pProxyItf ) ) )
						{
							pRecord->Destroy();
							pRecord = 0;
						}
					}
				}
				else
				{
					pRecord->Destroy();
					pRecord = 0;
				}
			}
			else hr = E_OUTOFMEMORY;

			pfactory->Release();
		}

		if ( pRecord )
		{
			pRecord->pNext = 0;
			pRecord->iid = iid;
			if ( !_divineProxyInfo( pProxyItf, &iid, &pRecord->pProcFmtString, &pRecord->pFcnOffsets ) )
			{
				pRecord->Destroy();
				pRecord = 0;
				hr = E_NOINTERFACE;
			}
		}
		return pRecord;
	}
};

struct LockCache
{
    LockCache() { EnterCriticalSection( &s_cs ); }
   ~LockCache() { LeaveCriticalSection( &s_cs ); }
};

HRESULT AddToCache( CacheRecord* pRecord );

static HRESULT Init()
{
	HRESULT hr = S_OK;

	// WIN_64_CHECK
	// deal with some well-known interfaces that don't use /Oicf
	CacheRecordOicf* pRecord = CacheRecordOicf::Create( IID_IDispatch, 4 );
	if ( pRecord )
	{
		WORD* it = pRecord->rgStackSizes;
		*it++ = IDISPATCH_STACK_SIZE_3;  // GetTypeInfoCount
		*it++ = IDISPATCH_STACK_SIZE_4;  // GetTypeInfo
		*it++ = IDISPATCH_STACK_SIZE_5;  // GetIDsOfNames
		*it++ = IDISPATCH_STACK_SIZE_6;  // Invoke
		hr = AddToCache( pRecord );
		if ( FAILED( hr ) )
			pRecord->Destroy();
	}
	
	return hr;
}

static HRESULT AddToCache( CacheRecord* pRecord )
{
	if ( !s_hashTable )
	{
		s_hashTable = new CacheRecord*[HASH_TABLE_SIZE];
		ZeroMemory( s_hashTable, sizeof( CacheRecord* ) * HASH_TABLE_SIZE );
		if ( !s_hashTable )
			return E_OUTOFMEMORY;
	}
	CacheRecord** pBucket = s_hashTable + ( hash( pRecord->iid ) % HASH_TABLE_SIZE );
	if ( *pBucket )
	{
		pRecord->pNext = *pBucket;
		*pBucket = pRecord;
	}
	else *pBucket = pRecord;
	return S_OK;
}

static bool FindInCache( const IID& iid, ItfTypeInfo*& pti )
{
	if ( !s_hashTable )
		return false;

	const CacheRecord* pRecord = s_hashTable[hash( iid ) % HASH_TABLE_SIZE];
	while ( pRecord )
	{
		if ( iid == pRecord->iid )
		{
			pti = const_cast<CacheRecord*>( pRecord );
			return true;
		}
		pRecord = pRecord->pNext;
	}
	return false;
}

static HRESULT CacheOicf( const IID& iid, const CLSID& clsidPSFactoryHint,
						  ItfTypeInfo*& pti )
{
    pti = 0;
	IPSFactoryBuffer* pFactory = 0;
	HRESULT hr = CoGetClassObject( clsidPSFactoryHint, CLSCTX_INPROC_SERVER, 0,
									IID_IPSFactoryBuffer, (void**)&pFactory );
	if ( FAILED( hr ) )
	{
#ifdef _DEBUG
		wchar_t sz[80];
		StringFromGUID2( iid, sz, sizeof sz / sizeof *sz );
		thk_OutputDebugStringW( sz );
		thk_OutputDebugStringW( L"\nERROR: Delegator cannot block calls to the above interface because its stub cannot be loaded (make sure your p/s dll or typelib is registered).\nReturning E_NOINTERFACE to caller!\n" );
#endif // _DEBUG
		return hr;
	}

	DummyObject dummyObject( iid );
	IRpcStubBuffer* pStubBuffer = 0;
	hr = pFactory->CreateStub( iid, &dummyObject, &pStubBuffer );
	if ( SUCCEEDED( hr ) )
	{
		const BYTE* pProcFmtString = 0;
		const WORD* itFcnOffset = 0;
		DWORD cMethods = 0;
		BOOL bDual = FALSE;
		if ( _divineStubInfo( pStubBuffer, &iid, &pProcFmtString, &itFcnOffset, &cMethods, &bDual ) )
		{
			CacheRecordOicf* pRecord = CacheRecordOicf::Create( iid, cMethods );
			if ( pRecord )
			{
				WORD* itDst = pRecord->rgStackSizes;
				const WORD* const end = itDst + cMethods;
				if ( bDual )
				{
					*itDst++ = IDISPATCH_STACK_SIZE_3;  // GetTypeInfoCount
					*itDst++ = IDISPATCH_STACK_SIZE_4;  // GetTypeInfo
					*itDst++ = IDISPATCH_STACK_SIZE_5;  // GetIDsOfNames
					*itDst++ = IDISPATCH_STACK_SIZE_6;  // Invoke

					itFcnOffset += 4;
				}
				while ( itDst != end )
				{
					// subtract size of return address, so stack size
					// only consists of args and implicit this pointer
					*itDst++ = pProcFmtString[8 + *itFcnOffset++] - sizeof( void* );
				}

				// rgStackSizes now points to a contiguous array of stack sizes
				// for each method in the interface
				hr = AddToCache( pRecord );
				if ( SUCCEEDED( hr ) )
					 pti = pRecord;
				else pRecord->Destroy();
			}
			else hr = E_OUTOFMEMORY;
		}
		else hr = E_NOINTERFACE; // must have fully-interpreted stubs

		pStubBuffer->Release();
		pStubBuffer = 0;
	}
	pFactory->Release();
	pFactory = 0;

    return hr;
}

static HRESULT CacheTLB( const IID& iid, ItfTypeInfo*& pti )
{
	HRESULT hr;
	pti = CacheRecordTLB::Create( iid, hr );
	return hr;
}

HRESULT ItfTypeInfo::GetTypeInfo( const IID& iid, ItfTypeInfo*& pti )
{
	HRESULT hr = S_OK;
	pti = 0;

	LockCache lock;

	// lazy init these resources
    // not everyone needs this stuff
	if ( !s_hashTable )
		hr = Init();
	if ( FAILED( hr ) )
		return hr;
	
	if ( FindInCache( iid, pti ) )
		return S_OK;

	CLSID clsid;
	hr = CoGetPSClsid( iid, &clsid );
	if ( FAILED( hr ) )
		return hr;

	if ( __uuidof( PSOAInterface ) == clsid )
		return CacheTLB( iid, pti );
	else if ( __uuidof( PSDispatch ) == clsid )
	{
		// special case for pure dispinterfaces (marshaler's CLSID is IID_IDispatch)
		FindInCache( IID_IDispatch, pti );
		return S_OK;
	}
	else return CacheOicf( iid, clsid, pti );
}

void ItfTypeInfo::Startup()
{
	InitializeCriticalSection( &s_cs );
}

void ItfTypeInfo::Shutdown()
{
	if ( s_hashTable )
	{
		CacheRecord** end = s_hashTable + HASH_TABLE_SIZE;
		for ( CacheRecord** it = s_hashTable; it != end; ++it )
		{
			CacheRecord* p = *it;
			if ( p )
				p->Destroy();
		}
		delete s_hashTable;
	}
	DeleteCriticalSection( &s_cs );
}
