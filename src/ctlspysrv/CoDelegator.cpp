/////////////////////////////////////////////////////////////
// CoDelegator.cpp - Generic Delegator Component
//
// Copyright 2000, Jason Whittington, Keith Brown
//
//This file contains code based on Keith Brown's most
//excellent Universal Delegator.  It has been modified by
//Jason Whittington for the needs of COM Spy.
/////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "CoDelegator.h"
#include "Delegator.h"
#include "Unmarshaler.h"
#include "FixedAllocator.h"
//#include "ItfTypeInfo.h"
//#include "crt.h"

#include <atlbase.h>

#include<vector>

//};

//---------------------------------------------------------------------------//
//guidless is just an STL functor for putting guids into
//collections.  
struct GuidLess
{
bool operator()(const IID& arg1, const IID& arg2)
    { return (arg1.Data1 < arg2.Data1);}
};

typedef std::vector<IID> IID_Coll;

//ProbeObject is a helper function that queries an object
//for every interface in the registry and returns a CAUUID
//holding every interface the object supports.
void ProbeObject(IUnknown * pObject, CAUUID * ppIFaces)
{
//we'll put stuff into the set first...
IID_Coll iids;
LONG lRes;

CRegKey basekey;
lRes=basekey.Open(HKEY_CLASSES_ROOT,"Interface",KEY_READ);
if(lRes == ERROR_SUCCESS)
	{
	FILETIME time;
	DWORD dwSize = 256;
	TCHAR szBuffer[256];
    int i=0;
	while (RegEnumKeyEx(basekey.m_hKey, i, szBuffer, &dwSize, NULL, NULL, NULL,
		&time)==ERROR_SUCCESS)
		{
		CRegKey iface_key;
		lRes = iface_key.Open(basekey,szBuffer, KEY_READ);
		if(lRes == ERROR_SUCCESS)
            {
            //convert the string to a GUID
            CLSID iid;
            OLECHAR szwBuf[128];

            ::mbstowcs(szwBuf, szBuffer, strlen(szBuffer)+1);

            //USES_CONVERSION;  //Are we going to kill the stack with this?  --YES we ARE. 
            //::CLSIDFromString(A2OLE(szBuffer), &iid);

            CLSIDFromString(szwBuf, &iid);

            //Test the object
            IUnknown * pTest;
            if(SUCCEEDED(pObject->QueryInterface(iid, (void**)&pTest)))
                {
                iids.push_back(iid);
                pTest->Release();
                }
	    	}
        i++;
	    dwSize = 256;	
		}
    }

//Now we should have a std::vector full of GUIDs.  We pour it
//into the CAUUID and push it out.  
ppIFaces->cElems = iids.size();
//make sure it's empty...
if(ppIFaces->pElems != NULL)
    ::CoTaskMemFree(ppIFaces->pElems);
//allocate a new block for the iids...
ppIFaces->pElems = (GUID*)(::CoTaskMemAlloc(iids.size()*sizeof(GUID)));

//then get an iterator to copy the items into the cauuid.
//IID_Coll::iterator it = iids.begin();
for(int i=0; i< iids.size(); i++)
    {
    ppIFaces->pElems[i] = iids[i];
    }
}
//---------------------------------------------------------------------------//




struct __declspec(uuid("0000001C-0000-0000-C000-000000000046")) FTM;

DWORD CoDelegator::s_nTLSIndex = 0xFFFFFFFF;
CRITICAL_SECTION CoDelegator::s_sect;
CoDelegator::LinkedCallContext* CoDelegator::s_pHeadCtx;
CoDelegator::LinkedCallContext* CoDelegator::s_pCachedCtx;
FixedAllocator s_lccAlloc;

//---------------------------------------------------------------------------//
CoDelegator::CoDelegator( IUnknown* pUnkOuter, IUnknown* pUnkInner,
						  IDelegatorHookQI* pHook, DWORD grfOptions, LPOLESTR szName )
  : m_cRefs( 0 ),
	m_pUnkOuter( pUnkOuter ),
	m_pUnkInner( pUnkInner ),
	m_pHook( pHook ),
	m_grf( grfOptions ),
	m_first( 0 ),
	m_last( 0 ),
	m_end( 0 ),
	m_splitIdentity(),
    m_bstrName(szName)
#ifdef _DEBUG
	, m_cSplitIdentityRefs(0)
#endif // _DEBUG

{
    ATLTRACE("Delegator %x created\n",this);

	if ( !m_pUnkOuter )
		m_pUnkOuter = this;

	InitializeCriticalSection( &m_sect );
	m_pUnkInner->AddRef();
	if ( m_pHook )
		m_pHook->AddRef();

	// For efficiency when using MBV inproc, discover whether the
	// inner object is relying on the FTM. If so, we'll also use
	// the FTM to make the most efficient use of the GIT, etc.
	// If so, _checkForFTM sets the INTERNALFLAGS_USEFTM bit in m_grf.
	if ( DO_MBV_INPROC & grfOptions )
		_checkForFTM();

    //Null out the CAUUID.  We'll fill it in response to Probe();
    m_IIDs.cElems = 0;
    m_IIDs.pElems = 0;

}

//---------------------------------------------------------------------------//
CoDelegator::~CoDelegator()
{
    //first we alert _Module that we're going away...
    ATLTRACE("Delegator %x destroyed\n", this);
    _Module.RemoveIdentity(this);

    //then we destruct
	for ( Delegator** it = m_first; it != m_last; ++it )
        {
        ATLASSERT((*it)->m_cRefs == 0);
		delete *it;
        }
	delete [] m_first;

	if ( m_pHook )
		m_pHook->Release();
	m_pUnkInner->Release();
	DeleteCriticalSection( &m_sect );

    if(m_IIDs.pElems !=  NULL)
        {
        ::CoTaskMemFree(m_IIDs.pElems);
        }

    ATLTRACE("Delegator deleted.\n");
}

//---------------------------------------------------------------------------//
ULONG CoDelegator::OnDelegatorFinalRelease( Delegator* pDelegator )
{
	bool bReleaseOuter = false;
	{
		Lock lock( *this );

		// The Delegator's refcount transitioned to zero, but it's possible
		// that another thread could have QI'd for the same interface
		// before we acquired the lock above. The code below avoids acquiring
		// the lock during every Release() call, which would be expensive.
		// Thanks to Geoff Outhred (DCOM-list member) for catching the race.
		if ( 0 == pDelegator->m_cRefs )
		{
			// If we're not caching interfaces, atomically remove this delegator
			// from the cache and delete it. Otherwise, let it hang around
			// for the lifetime of the CoDelegator.
			if ( 0 == ( DO_CACHE_INTERFACES & m_grf ) )
			{
				for ( Delegator** it = m_first; m_last != it; ++it )
				{
					if ( *it == pDelegator )
					{
						while ( ++it != m_last )
							*(it - 1) = *it;
						--m_last;
						break;
					}
				}
				delete pDelegator;
			}

			// In any case, we need to notify the outer
			// that an interface has been released.
			bReleaseOuter = true;
		}
	}
	// this might be our last release...
	// (thus the reason for scoping the lock above)
	return bReleaseOuter ? m_pUnkOuter->Release() : 2;
}

//---------------------------------------------------------------------------//
bool CoDelegator::Startup()
{
	InitializeCriticalSection( &s_sect );
	return s_lccAlloc.Startup( sizeof LinkedCallContext, 10 );
}

//---------------------------------------------------------------------------//
void CoDelegator::Shutdown()
{
	s_lccAlloc.Shutdown();
	DeleteCriticalSection( &s_sect );
}

//---------------------------------------------------------------------------//
CallContext* CoDelegator::PushNewCallContext( Delegator& d,
	const void* pReturnAddr, DWORD nVtblOffset )
{
    ATLTRACE("Pushing call context for thread 0x%x\n", GetCurrentThreadId());
	
    
    EnterCriticalSection( &s_sect );
	void* pBuf = s_lccAlloc.Alloc();
	LeaveCriticalSection( &s_sect );
	CallContext* pCtx = 0;
	if ( pBuf )
	{
		LinkedCallContext* pHead = reinterpret_cast<LinkedCallContext*>( TlsGetValue( s_nTLSIndex ) );
		pCtx = new( pBuf ) LinkedCallContext( d, pReturnAddr, nVtblOffset, pHead );
		BOOL bRet = TlsSetValue( s_nTLSIndex, pCtx );
        ATLASSERT(bRet);
	}

    ATLASSERT(pCtx != 0);
	return pCtx;
}

//---------------------------------------------------------------------------//
CallContext* CoDelegator::PopCallContext()
{

	// Pops are guaranteed to balance pushes, so no need for runtime checks
	LinkedCallContext* pHead = reinterpret_cast<LinkedCallContext*>( TlsGetValue( s_nTLSIndex ) );
    if(pHead == NULL)
        {
        DWORD dw = ::GetLastError();
        ATLTRACE("TLS is screwed, error %d\n", dw);
        }
	BOOL bRet = TlsSetValue( s_nTLSIndex, pHead->m_pNext );
    ATLASSERT(bRet);
	return pHead;
}

//---------------------------------------------------------------------------//
CallContext* CoDelegator::PeekCallContext()
{
	return reinterpret_cast<LinkedCallContext*>( TlsGetValue( s_nTLSIndex ) );
}

//---------------------------------------------------------------------------//
void CoDelegator::DeleteCallContext( CallContext* pcc )
{
	EnterCriticalSection( &s_sect );
	s_lccAlloc.Free( static_cast<LinkedCallContext*>( pcc ) );
	LeaveCriticalSection( &s_sect );
}

//---------------------------------------------------------------------------//
HRESULT CoDelegator::GetCallContextCookie( void** ppv )
{
	// this function returns the cookie from the most-nested call context
	if ( !ppv )
		return E_POINTER;
	*ppv = 0;

	if ( 0xFFFFFFFF == s_nTLSIndex )
		return E_UNEXPECTED; // not in a call with context

	const CallContext* const pcc = PeekCallContext();
	if ( !pcc )
		return E_UNEXPECTED; // not in a call with context
	
	*ppv = pcc->m_pData;
	return S_OK;
}

//---------------------------------------------------------------------------//
HRESULT CoDelegator::_growArray()
{
	size_t capacity = 2 * (m_end - m_first);
	if ( 0 == capacity )	
		capacity = INITIAL_CAPACITY;
	
	Delegator** first = new Delegator*[capacity];
	if ( !first )
		return E_OUTOFMEMORY;
	
	Delegator** dest = first;
	for ( Delegator** it = m_first; it != m_last; ++it )
		*dest++ = *it;

	m_last = first + (m_last - m_first);
	m_end = first + capacity;
	delete m_first;
	m_first = first;
	
	return S_OK;
}

//---------------------------------------------------------------------------//
bool CoDelegator::_findDelegator( REFIID iid, Delegator*& pDelegator, HRESULT& hr )
{
	hr = S_OK;

	for ( Delegator** it = m_first; it != m_last; ++it )
	{
		if ( (*it)->m_iid == iid )
		{
			if ( Delegator::DONT_EXPOSE_FROM_QI & (*it)->m_grf )
				 hr = E_NOINTERFACE;
			else pDelegator = *it;
			return true;
		}
	}
	return false;
}

//---------------------------------------------------------------------------//
bool CoDelegator::_mbvInThisContext( DWORD nDestCtx )
{
	switch ( nDestCtx )
	{
		case MSHCTX_INPROC:				return ( m_grf & DO_MBV_INPROC ) ? true : false;
		case MSHCTX_LOCAL:				// fall through
		case MSHCTX_NOSHAREDMEM:		return ( m_grf & DO_MBV_LOCAL ) ? true : false;
		case MSHCTX_DIFFERENTMACHINE:	return ( m_grf & DO_MBV_DIFFERENTMACHINE ) ? true : false;
	}
	return false;
}

//---------------------------------------------------------------------------//
void CoDelegator::_checkForFTM()
{
	IMarshal* pMsh = 0;
	if ( SUCCEEDED( m_pUnkInner->QueryInterface( IID_IMarshal, (void**)&pMsh ) ) )
	{
		CLSID clsid;
		if ( SUCCEEDED( pMsh->GetUnmarshalClass( IID_IUnknown, m_pUnkInner,
			MSHCTX_INPROC, 0, MSHLFLAGS_NORMAL, &clsid ) ) )
		{
			if ( __uuidof( FTM ) == clsid )
				m_grf |= INTERNALFLAGS_USEFTM;
		}
		pMsh->Release();
	}
}

//---------------------------------------------------------------------------//
bool CoDelegator::_lazyAllocTLSIndex()
{
	

    bool bResult = true;
	if ( 0xFFFFFFFF == s_nTLSIndex )
	{
    ATLTRACE("Thread 0x%x allocating TLS slot\n", GetCurrentThreadId());
		s_nTLSIndex = TlsAlloc();
		if ( 0xFFFFFFFF == s_nTLSIndex )
			 bResult = false;
		else TlsSetValue( s_nTLSIndex, 0 );
	}
	return bResult;
}

//---------------------------------------------------------------------------//
BOOL GetInterfaceName(REFIID iid, LPTSTR szName, long Size);

STDMETHODIMP CoDelegator::QueryInterface( REFIID iid, void** ppv )
{
ULONG newCount = 0;
if(ppv == NULL) return E_POINTER;
*ppv = NULL;

HRESULT hr = InternalQueryInterface(iid, ppv, &newCount);
_Module.OnQI(this, iid, newCount, hr);

if(SUCCEEDED(hr)) {ATLASSERT(*ppv != NULL);}
if(FAILED(hr)) {ATLASSERT(*ppv == NULL);}

char szIface[128];
GetInterfaceName(iid, szIface, 128);

ATLTRACE("QI(%s) on thread 0x%x returning %x\n", szIface, GetCurrentThreadId(), hr);

return hr;
}

HRESULT CoDelegator::InternalQueryInterface( REFIID iid, void** ppv, ULONG * pNewIfaceRefCount)
{
IUnknown* pUnkInner = 0;
HRESULT hr = E_NOINTERFACE;
Delegator* pDelegator = 0;
Lock lock( *this );

	// we subsume the identity of the inner
	if ( IID_IUnknown == iid )
	{
		//count = reinterpret_cast<IUnknown*>(*ppv = static_cast<IUnknown*>(this))->AddRef();

        //Do a silent AddRef by calling InternalAddRef.  This way we will only fire a single
        //event. rather than an AddRef event followed by a QI event.
        *ppv = static_cast<IUnknown*>(this);
        *pNewIfaceRefCount = this->InternalAddRef(); 
        return S_OK;
	}

    //hack for now.. do NOT support this interface
    if( IID_IQuickActivate == iid)
        {
        *ppv = 0;
        return E_NOINTERFACE;
        }
    //note that I just broke the MBV semantics.  Will I want them in the future?  Who knows?
    //Now that I've started trying to hook multithreaded objects this could be a problem.
    else if(IID_IMarshal == iid) 
        {
        *ppv = 0;
        return E_NOINTERFACE;
        }

	//else if ( ( IID_IMarshal == iid ) && ( DO_MBV_ALL & m_grf ) )
	//{
	//	reinterpret_cast<IUnknown*>(*ppv = static_cast<IMarshal*>(this))->AddRef();
	//	return S_OK;
	//}

	// see if we've already assimilated the requested interface
	//HRESULT hr = S_OK;
	if ( _findDelegator( iid, pDelegator, hr ) )
	{
		if ( SUCCEEDED( hr ) )
            {
            *ppv = pDelegator;
			//count = reinterpret_cast<IUnknown*>( *ppv = pDelegator )->AddRef();
            *pNewIfaceRefCount = pDelegator->InternalAddRef();
    		return hr;
            }
	}

	// go get the requested interface from the inner and assimilate it.
	hr = m_pUnkInner->QueryInterface( iid, (void**)&pUnkInner );
	if ( SUCCEEDED( hr ) )
	{
		DWORD grfOptions = DHO_PREPROCESS_METHODS | DHO_POSTPROCESS_METHODS;
		//DWORD grfOptions = 0;
		IDelegatorHookMethods2* pHookMethods = 0;
		ItfTypeInfo* pTypeInfo = 0;

        //Grab a TLS slot now if we need it...
        if(grfOptions | DHO_POSTPROCESS_METHODS) _lazyAllocTLSIndex();


		if ( m_pHook )
		{
			// WIN_64_CHECK
			// the signature of IDelegatorHookMethods
			// is the same as IDelegatorHookMethods2,
			// until Win64, by which time this code needs
			// to be revisited anyway.
			hr = m_pHook->OnFirstDelegatorQIFor( iid, pUnkInner,
												 &grfOptions,
												 ( m_grf & INTERNALFLAGS_VERSION_2_0 ) ? IID_IDelegatorHookMethods : IID_IDelegatorHookMethods2,
												 (void**)&pHookMethods );
			if ( SUCCEEDED( hr ) )
			{
				// watch for invalid results from QI hook
				grfOptions = grfOptions & 0x00000007;
				if ( ( 0 == ( grfOptions & 0x00000003 ) ) && pHookMethods )
				{
                    // must choose either pre- or post-processing
					pHookMethods->Release();
					pHookMethods = 0;
				}
				else if ( 0x00000005 == ( grfOptions & 0x00000005 ) )
				{
					// Attempt to discover type info for this interface.
					// We must have an Oicf string so the delegator knows
					// how big the stack is when a hook chooses to block a call.
					// This will return E_NOINTERFACE if it can't be found.
					// The resulting ItfTypeInfo* is managed by a cache,
					// so we don't need to worry about freeing it.
					hr = ItfTypeInfo::GetTypeInfo( iid, pTypeInfo );

					if ( FAILED( hr ) && pHookMethods )
					{
						pHookMethods->Release();
						pHookMethods = 0;
					}
				}
			}
			
			if ( E_NOINTERFACE == hr )
			{
				// we only give the QI hook *one* chance to say this
				// to a particular interface so that we help maintain a
				// correct implementation of QI.
				grfOptions = Delegator::DONT_EXPOSE_FROM_QI;
				hr = S_OK;
			}

			// if postprocessing is required, make sure we've acquired a TLS slot
			if ( SUCCEEDED( hr )
				&& ( DHO_POSTPROCESS_METHODS & grfOptions )
				&& !_lazyAllocTLSIndex() )
			{
				hr = E_OUTOFMEMORY;	// sort of :-)
				if ( pHookMethods )
				{
					pHookMethods->Release();
					pHookMethods = 0;
				}
			}
		}

		if ( SUCCEEDED( hr ) )
		{
			// grow the array if necessary
			if ( m_last == m_end )
				hr = _growArray();
			
			if ( SUCCEEDED( hr ) )
			{
				Delegator* pDelegator = new Delegator( *this, pUnkInner, iid,
														grfOptions, pHookMethods,
														pTypeInfo );
				if ( pDelegator )
				{
					*m_last++ = pDelegator;
					if ( Delegator::DONT_EXPOSE_FROM_QI & grfOptions )
						 hr = E_NOINTERFACE;
					else 
                        {
                        *ppv = pDelegator;
                        *pNewIfaceRefCount = pDelegator->InternalAddRef();
                        }
				}
				else hr = E_OUTOFMEMORY;
			}
			if ( pHookMethods )
				pHookMethods->Release();
		}
		pUnkInner->Release();
	}

	return hr;
}

//---------------------------------------------------------------------------//
STDMETHODIMP_(ULONG) CoDelegator::InternalAddRef()
{
    ATLTRACE("Delegator %x(%x) ++(%u)\n",this, GetCurrentThreadId(), m_cRefs+1);

	extern void SvcLock();
	if ( 0 == m_cRefs )
		SvcLock();
	return InterlockedIncrement( &m_cRefs );
}


//---------------------------------------------------------------------------//
STDMETHODIMP_(ULONG) CoDelegator::AddRef()
{
    _Module.OnRefcountChanged(this,IID_IUnknown,m_cRefs, m_cRefs+1);
	return InternalAddRef();
}

//---------------------------------------------------------------------------//
STDMETHODIMP_(ULONG) CoDelegator::Release()
{
    ATLTRACE("Delegator %x(%x) --(%u)\n",this, GetCurrentThreadId(), m_cRefs-1);
    _Module.OnRefcountChanged(this,IID_IUnknown,m_cRefs, m_cRefs-1);

	extern void SvcUnlock();
	ULONG n;
	//if ( 0 != ( m_grf & DO_MAINTAIN_IDENTITY ) )
	//	 n = Unmarshaler::LockDictionaryAndReleaseDelegator( *this );
	//else
    n = InterlockedDecrement( &m_cRefs );

	if ( 0 == n )
	{
    /*
		if ( m_grf & DO_CONNECTION_NOTIFICATION )
		{
			// notify the wrapped object that the delegator's refcount
			// has dropped to zero, and that it should release any references
			// to the delegator's split identity.
			IDelegatorConnection* pdc = 0;
			if ( SUCCEEDED( m_pUnkInner->QueryInterface( IID_IDelegatorConnection,
														 (void**)&pdc ) ) )
			{
				pdc->DelegatorDisconnect();
				pdc->Release();
			}
#ifdef _DEBUG
			if ( 0 != m_cSplitIdentityRefs )
				OutputDebugString( __TEXT( "WARNING: "
										   "Objects should release references to the "
										   "CoDelegator's split identity during "
										   "IDelegatorConnection::DelegatorDisconnect\n" ) );
#endif
		}
    */
		delete this;
		SvcUnlock();
	}

	return n;
}


//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::GetUnmarshalClass( REFIID iid, void* pItf,
	DWORD nDestCtx, void* pvCtx, DWORD grfMshFlags, CLSID* pClsid )
{
	if ( !pClsid )
		return E_POINTER;

	if ( _mbvInThisContext( nDestCtx ) )
	{
		// Use FTM if inner is apartment neutral
		if ( ( MSHCTX_INPROC == nDestCtx ) && ( m_grf & INTERNALFLAGS_USEFTM ) )
		{
			*pClsid = __uuidof( FTM );
			return S_OK;
		}
		*pClsid = __uuidof( Unmarshaler );
		return S_OK;
	}

	// We obey iid_is, so the following block determines which pointer we will marshal
	HRESULT hr = S_OK;
	IUnknown* pUnkInner = 0; // don't need to release this local var
	if ( IID_IUnknown == iid )
		pUnkInner = m_pUnkInner;
	else
	{
		Delegator* pDelegator = 0;
		Lock lock( *this );
		if ( !_findDelegator( iid, pDelegator, hr ) )
			return E_UNEXPECTED; // should never be asked to marshal an unknown ptr
		pUnkInner = pDelegator->m_pUnkInner;
	}
	if ( FAILED( hr ) )
		return hr;

	IMarshal* pMsh = 0;
	hr = m_pUnkInner->QueryInterface( IID_IMarshal, (void**)&pMsh );
	if ( FAILED( hr ) )
		hr = CoGetStandardMarshal( iid, pUnkInner, nDestCtx, pvCtx, grfMshFlags, &pMsh );
	if ( FAILED( hr ) )
		return hr;
	hr = pMsh->GetUnmarshalClass( iid, pUnkInner, nDestCtx, pvCtx, grfMshFlags, pClsid );
	pMsh->Release();

	return hr;
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::GetMarshalSizeMax( REFIID iid, void* pItf,
	DWORD nDestCtx, void* pvCtx, DWORD grfMshFlags, ULONG* pcb )
{
	if ( !pcb )
		return E_POINTER;

	// We obey iid_is, so the following block determines which pointer we will marshal
	HRESULT hr = S_OK;
	IUnknown* pUnkInner = 0; // don't need to release this local var
	if ( IID_IUnknown == iid )
		pUnkInner = m_pUnkInner;
	else
	{
		Delegator* pDelegator = 0;
		Lock lock( *this );
		if ( !_findDelegator( iid, pDelegator, hr ) )
			return E_UNEXPECTED; // should never be asked to marshal an unknown ptr
		pUnkInner = pDelegator->m_pUnkInner;
	}
	if ( FAILED( hr ) )
		return hr;

	if ( !_mbvInThisContext( nDestCtx ) )
		return CoGetMarshalSizeMax( pcb, iid, pUnkInner, nDestCtx, pvCtx, grfMshFlags );

	// Use FTM if inner is apartment neutral
	if ( ( MSHCTX_INPROC == nDestCtx ) && ( m_grf & INTERNALFLAGS_USEFTM ) )
	{
		IUnknown* pUnkFTM = 0;
		HRESULT hr = CoCreateFreeThreadedMarshaler( static_cast<IMarshal*>( this ), &pUnkFTM );
		if ( SUCCEEDED( hr ) )
		{
			IMarshal* pMsh = 0;
			hr = pUnkFTM->QueryInterface( IID_IMarshal, (void**)&pMsh );
			if ( SUCCEEDED( hr ) )
			{
				hr = pMsh->GetMarshalSizeMax( iid, pItf, nDestCtx, pvCtx, grfMshFlags, pcb );
				pMsh->Release();
			}
			pUnkFTM->Release();
		}
		return hr;
	}

	if ( grfMshFlags & MSHLFLAGS_TABLESTRONG )
	{
		if ( MSHCTX_INPROC != nDestCtx )
			return E_FAIL;
	}
	else if ( grfMshFlags & MSHLFLAGS_TABLEWEAK )
		return E_FAIL;

	// some hooks support IPersistStream, which will contribute to the size
	IPersistStream* pHookPersistStream = 0;
	if ( m_pHook && FAILED( m_pHook->QueryInterface( IID_IPersistStream, (void**)&pHookPersistStream ) ) )
		pHookPersistStream = 0;

	if ( grfMshFlags & MSHLFLAGS_TABLESTRONG )
	{
		*pcb = sizeof( DWORD ) + sizeof( DWORD ) + sizeof m_grf + sizeof( CLSID );
	}
	else
	{
		hr = CoGetMarshalSizeMax( pcb, iid, pUnkInner, nDestCtx, pvCtx, grfMshFlags );
		*pcb += sizeof( DWORD ) + sizeof m_grf + sizeof( CLSID );
	}
	if ( SUCCEEDED( hr ) && pHookPersistStream )
	{
		__int64 cb;
		hr = pHookPersistStream->GetSizeMax( reinterpret_cast<ULARGE_INTEGER*>( &cb ) );
		*pcb += sizeof( DWORD ) + static_cast<ULONG>( cb );
	}

	if ( pHookPersistStream )
		pHookPersistStream->Release();

	return hr;
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::MarshalInterface( IStream* pstm, REFIID iid,
	void* pItf, DWORD nDestCtx, void* pvCtx, DWORD grfMshFlags )
{
	if ( !pstm )
		return E_INVALIDARG;

	// We obey iid_is, so the following block determines which pointer we will marshal
	HRESULT hr = S_OK;
	IUnknown* pUnkInner = 0; // don't need to release this local var
	if ( IID_IUnknown == iid )
		pUnkInner = m_pUnkInner;
	else
	{
		Delegator* pDelegator = 0;
		Lock lock( *this );
		if ( !_findDelegator( iid, pDelegator, hr ) )
			return E_UNEXPECTED; // should never be asked to marshal an unknown ptr
		pUnkInner = pDelegator->m_pUnkInner;
	}
	if ( FAILED( hr ) )
		return hr;

	if ( !_mbvInThisContext( nDestCtx ) )
		return CoMarshalInterface( pstm, iid, pUnkInner, nDestCtx, pvCtx, grfMshFlags );

	if ( DO_MAINTAIN_IDENTITY & m_grf )
	{
    ATLASSERT(FALSE);
		//hr = Unmarshaler::LazyRegisterIdentity( m_pUnkInner, static_cast<IMarshal*>( this ) );
		//if ( FAILED( hr ) )
		//	return hr;
	}

	// Use FTM if inner is apartment neutral
	if ( ( MSHCTX_INPROC == nDestCtx ) && ( m_grf & INTERNALFLAGS_USEFTM ) )
	{
		IUnknown* pUnkFTM = 0;
		HRESULT hr = CoCreateFreeThreadedMarshaler( static_cast<IMarshal*>( this ), &pUnkFTM );
		if ( SUCCEEDED( hr ) )
		{
			IMarshal* pMsh = 0;
			hr = pUnkFTM->QueryInterface( IID_IMarshal, (void**)&pMsh );
			if ( SUCCEEDED( hr ) )
			{
				hr = pMsh->MarshalInterface( pstm, iid, pItf, nDestCtx, pvCtx, grfMshFlags );
				pMsh->Release();
			}
			pUnkFTM->Release();
		}
		return hr;
	}

	// we don't support table marshals, except for the special case of
	// MSHCTX_INPROC and MSHLFLAGS_TABLESTRONG, which is a good indication
	// that we are being marshaled into the GIT. In this case, we marshal the
	// proxy into the GIT and store the cookie in our custom OBJREF.
	if ( grfMshFlags & MSHLFLAGS_TABLESTRONG )
	{
		if ( MSHCTX_INPROC != nDestCtx )
			return E_FAIL;
	}
	else if ( grfMshFlags & MSHLFLAGS_TABLEWEAK )
		return E_FAIL;

	IPersist* pHookPersist = 0;
	IPersistStream* pHookPersistStream = 0;
	if ( m_pHook )
	{
		// all hooks support at least IPersist
		if ( FAILED( m_pHook->QueryInterface( IID_IPersist, (void**)&pHookPersist ) ) )
			return E_UNEXPECTED;

		// some hooks support IPersistStream
		if ( FAILED( m_pHook->QueryInterface( IID_IPersistStream, (void**)&pHookPersistStream ) ) )
			pHookPersistStream = 0;
	}

	// put all useful information about the stream format up front to simplify
	// our life when we need to read it back
	DWORD grfDelegatorMarshal = 0;
	if ( pHookPersistStream )
		grfDelegatorMarshal |= DMSH_SERIALIZED_HOOK_STATE;
	if ( grfMshFlags & MSHLFLAGS_TABLESTRONG )
		grfDelegatorMarshal |= DMSH_INNER_IN_GIT;

	// begin marshaling
	hr = pstm->Write( &grfDelegatorMarshal, sizeof grfDelegatorMarshal, 0 );

	__int64 beginSubmarshal;
	__int64 zero = 0;
	bool bSubmarshaled = false;
	IGlobalInterfaceTable* pgit = 0;
	DWORD nGITCookie = 0;

	// record our current stream offset in case we have to back out a submarshal on failure
	if ( SUCCEEDED( hr ) )
		hr = pstm->Seek( *reinterpret_cast<LARGE_INTEGER*>(&zero), STREAM_SEEK_CUR, reinterpret_cast<ULARGE_INTEGER*>( &beginSubmarshal ) );

	//  attempt to submarshal the interface pointer
	if ( SUCCEEDED( hr ) )
	{
		if ( grfDelegatorMarshal & DMSH_INNER_IN_GIT )
		{
			// marshal into the GIT
			HRESULT hrGIT = CoCreateInstance( CLSID_StdGlobalInterfaceTable, 0, CLSCTX_INPROC_SERVER, IID_IGlobalInterfaceTable, (void**)&pgit );
			if ( SUCCEEDED( hrGIT ) )
			{
				hrGIT = pgit->RegisterInterfaceInGlobal( pUnkInner, iid, &nGITCookie );
				pgit->Release();
				if ( SUCCEEDED( hrGIT ) )
				{
					hr = pstm->Write( &nGITCookie, sizeof nGITCookie, 0 );
					bSubmarshaled = true;
				}
				else hr = E_FAIL; // TBD: need to define better error codes, but using what facility???
			}
			else pgit = 0;
		}
		else
		{
			// submarshal into the custom OBJREF
			hr = CoMarshalInterface( pstm, iid, pUnkInner, nDestCtx, pvCtx, grfMshFlags );
			if ( SUCCEEDED( hr ) )
				bSubmarshaled = true; // if we fail part way through, will need to CoReleaseMarshalData
		}
	}

	// write the rest of our state, including the hook and any state it may have
	if ( SUCCEEDED( hr ) )
		hr = pstm->Write( &m_grf, sizeof m_grf, 0 );
	if ( SUCCEEDED( hr ) && m_pHook )
	{
		CLSID clsidHook;
		hr = pHookPersist->GetClassID( &clsidHook );
		if ( SUCCEEDED( hr ) )
			hr = pstm->Write( &clsidHook, sizeof clsidHook, 0 );
		if ( SUCCEEDED( hr ) && pHookPersistStream )
		{
			// store hook data with a length prefix to make ReleaseMarshalData
			// possible without having to rehydrate the hook
			__int64 skip = sizeof DWORD;
			__int64 beginHookData;
			hr = pstm->Seek( *reinterpret_cast<LARGE_INTEGER*>(&skip), STREAM_SEEK_CUR, reinterpret_cast<ULARGE_INTEGER*>( &beginHookData ) );
			if ( SUCCEEDED( hr ) )
				hr = pHookPersistStream->Save( pstm, FALSE );
			__int64 endHookData;
			if ( SUCCEEDED( hr ) )
				hr = pstm->Seek( *reinterpret_cast<LARGE_INTEGER*>(&zero), STREAM_SEEK_CUR, reinterpret_cast<ULARGE_INTEGER*>( &endHookData ) );
			if ( SUCCEEDED( hr ) )
			{
				// back up to write the length-prefix
				__int64 hookDataHeaderPos = beginHookData - sizeof DWORD;
				hr = pstm->Seek( *reinterpret_cast<LARGE_INTEGER*>(&hookDataHeaderPos), STREAM_SEEK_SET, 0 );
			}
			if ( SUCCEEDED( hr ) )
			{
				// write the length-prefix
				DWORD cbHookData = static_cast<DWORD>( endHookData - beginHookData );
				hr = pstm->Write( &cbHookData, sizeof cbHookData, 0 );
			}
			// restore the stream to the end of the hook data
			if ( SUCCEEDED( hr ) )			
				hr = pstm->Seek( *reinterpret_cast<LARGE_INTEGER*>(&endHookData), STREAM_SEEK_SET, 0 );
		}
	}
	else hr = pstm->Write( &CLSID_NULL, sizeof CLSID_NULL, 0 );

	if ( FAILED( hr ) && bSubmarshaled )
	{
		// free the submarshal
		if ( grfDelegatorMarshal & DMSH_INNER_IN_GIT )
			pgit->RevokeInterfaceFromGlobal( nGITCookie );
		else if ( SUCCEEDED( pstm->Seek( *reinterpret_cast<LARGE_INTEGER*>( &beginSubmarshal ), STREAM_SEEK_SET, 0 ) ) )
			CoReleaseMarshalData( pstm );
	}
	if ( pgit )
		pgit->Release();
	if ( pHookPersistStream )
		pHookPersistStream->Release();
	if ( pHookPersist )
		pHookPersist->Release();

	return hr;
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::ReleaseMarshalData( IStream* pstm )
{
	return CoReleaseMarshalData( pstm );
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::UnmarshalInterface( IStream*, REFIID, void** )
{
	// implemented by unmarshaler
	return E_NOTIMPL;
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::DisconnectObject( DWORD )
{
	return S_OK;
}


//---------------------------------------------------------------------------//
HRESULT CoDelegator::Probe(DWORD dwOptions, long * pCount, REFCOUNT ** ppCounts)
{
//We don't want our refcounts changing here, now do we?
Lock lock(*this);

//First get the current number of interfaces...

if(dwOptions == 0) 
    {
    //In this case we just walk the linked list and copy in the iid and refcount
    //from each delegator struct, but only for the ones with a nonzerfo refcount.
    long count = 0;
	for ( Delegator** it = m_first; it != m_last; ++it ) 
        {
        if((*it)->m_cRefs > 0)
            count++;
        }

    *pCount = count+1;
    *ppCounts = reinterpret_cast<REFCOUNT*>(::CoTaskMemAlloc(*pCount * sizeof(REFCOUNT))); 
    if(*ppCounts == NULL) return E_OUTOFMEMORY;

    //IUnknown gets special treatment - we give our own refcount...
    (*ppCounts)[0].iid = IID_IUnknown;
    (*ppCounts)[0].rfcount = m_cRefs;

    //Now go through and copy the data out...
    int i=1;
	for (it = m_first; it != m_last; ++it ) 
        {
        if((*it)->m_cRefs > 0)
            {
            (*ppCounts)[i].iid = (*it)->m_iid;
            (*ppCounts)[i].rfcount = (*it)->m_cRefs;
            i++;
            }
        }

    }
else if (dwOptions = 1)
    {
    //In this case we put every interface in the report, including zero refcounts.  We cache
    //the itnerface info in m_IIDs so that if we get called multiple times we don't keep probing
    //the object each time.
    if(m_IIDs.cElems == 0)
        {
        ProbeObject(m_pUnkInner, &m_IIDs);
        }

    *pCount = m_IIDs.cElems;

    *ppCounts = reinterpret_cast<REFCOUNT*>(::CoTaskMemAlloc(*pCount * sizeof(REFCOUNT))); 

    ZeroMemory(*ppCounts, *pCount * sizeof(REFCOUNT));
    if(*ppCounts == NULL) return E_OUTOFMEMORY;


    //IUnknown gets special treatment - we give our own refcount...
    (*ppCounts)[0].iid = IID_IUnknown;
    (*ppCounts)[0].rfcount = m_cRefs;

    for(int i=1; i < *pCount; i++)
        {
        HRESULT hr;
        Delegator * pD = 0;
        _findDelegator(m_IIDs.pElems[i], pD, hr);
        (*ppCounts)[i].iid = m_IIDs.pElems[i];
        (*ppCounts)[i].rfcount = (pD != NULL)? pD->m_cRefs : 0;
        }
    }

return S_OK;
}


//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::SplitIdentity::QueryInterface( REFIID iid, void** ppv )
{
	if ( ( IID_IDelegatorSplitIdentity == iid ) || IID_IUnknown == iid )
		*ppv = static_cast<IDelegatorSplitIdentity*>( this );
	else return (*ppv = 0), E_NOINTERFACE;
	AddRef();
	return S_OK;
}

//---------------------------------------------------------------------------//
STDMETHODIMP_(ULONG) CoDelegator::SplitIdentity::AddRef()
{
#ifdef _DEBUG
	return InterlockedIncrement( &Trunk()->m_cSplitIdentityRefs );
#else
	return 2;
#endif // _DEBUG
}

//---------------------------------------------------------------------------//
STDMETHODIMP_(ULONG) CoDelegator::SplitIdentity::Release()
{
#ifdef _DEBUG
	return InterlockedDecrement( &Trunk()->m_cSplitIdentityRefs );
#else
	return 1;
#endif // _DEBUG
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::SplitIdentity::DelegatorSafeRef( REFIID iid, void** ppv )
{
	return Trunk()->m_pUnkOuter->QueryInterface( iid, ppv );
}

//---------------------------------------------------------------------------//
STDMETHODIMP CoDelegator::SplitIdentity::GetHook( REFIID iid, void** ppv )
{
	if ( !Trunk()->m_pHook )
		return (*ppv = 0), E_UNEXPECTED;
	return Trunk()->m_pHook->QueryInterface( iid, ppv );
}



