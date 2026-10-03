/////////////////////////////////////////////////////////////
// CoDelegator.h - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
// The CoDelegator class implements a standard non-delegating
// unknown and caches interface pointers lazily as they
// are asked for by the outer object.
// Each new interface pointer is wrapped by a Delegator
// object and refcounted individually, until the refcount
// on the interface drops to zero, at which time it is
// released and removed from the collection.
/////////////////////////////////////////////////////////////
#ifndef _CODELEGATOR_H
#define _CODELEGATOR_H

#include "delegator.h"
//#include "crt.h"

//#include "delegatorsite.h"

#include "dharma.h"

struct CallContext
{
	CallContext()
	  : m_pDelegator( 0 ),
	    m_pReturnAddr( 0 ),
		m_pData( 0 ),
		m_nVtblOffset( 0 )
	{}
	CallContext( Delegator& d, const void* pReturnAddr, DWORD nVtblOffset )
	  : m_pDelegator( &d ),
		m_pReturnAddr( pReturnAddr ),
		m_pData( 0 ),
		m_nVtblOffset( nVtblOffset )
	{}
	Delegator*	m_pDelegator;
	const void* m_pReturnAddr;
	void*		m_pData;
	DWORD		m_nVtblOffset;
};

//---------------------------------------------------------------------------//
// CoDelegator
//   This class represents the COM identity of the delegation layer.
//   It morphs itself to look like another class by answering QI requests
//   from the client and delegating to the wrapped object.
//   Each interface is implemented by an instance of the Delegator structure,
//   which in turn holds an interface pointer to the wrapped object to which
//   it delegates calls.

class CoDelegator : public IMarshal
{
	friend struct Delegator;
	friend class CoDelegatorFactory;
	friend class CoDebugDelegatorFactory;
    friend class CoDebugDelegator;
public:
	CoDelegator( IUnknown* pUnkOuter, IUnknown* pUnkInner,
				 IDelegatorHookQI* pHook, DWORD grfOptions, LPOLESTR szName = NULL);
	~CoDelegator();

	ULONG OnDelegatorFinalRelease( Delegator* pDelegator );
	IUnknown* GetOuter() { return m_pUnkOuter; }
	IUnknown* GetInner() { return m_pUnkInner; }
	IDelegatorSplitIdentity* GetSplitIdentity() { return &m_splitIdentity; }

	// used by the Unmarshaler to implement atomic Release for
	// delegators with the DO_MAINTAIN_IDENTITY flag
	ULONG DecrementRefcount() { return InterlockedDecrement( &m_cRefs ); }
		
	static CallContext* PushNewCallContext( Delegator& d,
											const void* pReturnAddr,
											DWORD nVtblOffset );
	static CallContext* PopCallContext();
	static CallContext* PeekCallContext();
	static void DeleteCallContext( CallContext* pcc );
	static HRESULT GetCallContextCookie( void** ppv );

	// allow initialization and cleanup of static state
	static bool Startup();
	static void Shutdown();

	enum { DMSH_SERIALIZED_HOOK_STATE	= 0x00000001,
		   DMSH_INNER_IN_GIT			= 0x00000002 };

	enum { INTERNALFLAGS_USEFTM			= 0x80000000,
		   INTERNALFLAGS_VERSION_2_0	= 0x40000000 };

    HRESULT InternalQueryInterface( REFIID iid, void** ppv, ULONG * pNewIfaceRefCount);
	STDMETHODIMP QueryInterface( REFIID iid, void** ppv ); 
    STDMETHODIMP_(ULONG) InternalAddRef();  //internal refcount munger
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	void DelegatorRelease(REFIID iid, ULONG newCount);

	STDMETHODIMP GetUnmarshalClass( REFIID iid, void* pv, DWORD grfDestCtx, void*, DWORD grfMshFlags, CLSID* pClsid );
	STDMETHODIMP GetMarshalSizeMax( REFIID iid, void* pv, DWORD grfDestCtx, void*, DWORD grfMshFlags, ULONG* pcb );
	STDMETHODIMP MarshalInterface( IStream* pstm, REFIID iid, void* pv, DWORD grfDestCtx, void*, DWORD grfMshFlags );
	STDMETHODIMP ReleaseMarshalData( IStream* pstm );
	STDMETHODIMP UnmarshalInterface( IStream* pstm, REFIID iid, void** ppv );
	STDMETHODIMP DisconnectObject( DWORD );

    //helper for scanning the refcounts on this object
    HRESULT Probe(DWORD dwOptions, long * pCount, REFCOUNT ** ppCounts);

private:
	HRESULT _growArray();
	bool _findDelegator( REFIID iid, Delegator*& pDelegator, HRESULT& hr );
	bool _mbvInThisContext( DWORD nDestCtx );	
	void _checkForFTM();
	static bool _lazyAllocTLSIndex();

	struct SplitIdentity : IDelegatorSplitIdentity
	{
		CoDelegator* Trunk();
		STDMETHODIMP QueryInterface( REFIID iid, void** ppv );
		STDMETHODIMP_(ULONG) AddRef();
		STDMETHODIMP_(ULONG) Release();
		STDMETHODIMP DelegatorSafeRef( REFIID iid, void** ppv );
		STDMETHODIMP GetHook( REFIID iid, void** ppv );
	};
	friend struct SplitIdentity;

	class Lock
	{
	public:
		Lock( CoDelegator& obj )
		  : m_obj( obj )
		{ EnterCriticalSection( &m_obj.m_sect ); }
		~Lock() { LeaveCriticalSection( &m_obj.m_sect ); }
	private:
		CoDelegator& m_obj;
	};
	friend class Lock;

	struct LinkedCallContext : CallContext
	{
		LinkedCallContext()
		  : CallContext(),
		    m_pNext( 0 )
		{}
		LinkedCallContext( Delegator& d, const void* pReturnAddr, DWORD nVtblOffset, LinkedCallContext* pNext )
		  : CallContext( d, pReturnAddr, nVtblOffset ),
		    m_pNext ( pNext )
		{}
		LinkedCallContext* m_pNext;
	};

    public:
    //We'll go ahead and store a GIT cookie and option flags here.  These are used by _Module and
    //by the accessor objects.
    DWORD m_dwGITCookie;
    DWORD m_dwIdentityFlags;
    CAUUID m_IIDs; //we will lazy-fill this if anybody calls ProbeObject(1).

    BSTR GetName(){return m_bstrName;}
    DWORD GetIdentityFlags(){return m_dwIdentityFlags;}
    DWORD GetGITCookie(){return m_dwGITCookie;}
    void Init(DWORD dwGITCookie, DWORD dwIdentityFlags)
       {m_dwGITCookie = dwGITCookie, m_dwIdentityFlags = dwIdentityFlags;}


    //Right now we only allow one debugger to attach per object, so we just store pointers
    //here.


    private:
	enum { INITIAL_CAPACITY = 3 };

	long				m_cRefs;
	IUnknown*			m_pUnkOuter;
	IUnknown*			m_pUnkInner;
	IDelegatorHookQI*	m_pHook;
	DWORD				m_grf;
	Delegator**			m_first;
	Delegator**			m_last;
	Delegator**			m_end;
	CRITICAL_SECTION	m_sect;
	SplitIdentity		m_splitIdentity;


#ifdef _DEBUG
	long				m_cSplitIdentityRefs;
#endif // _DEBUG

	static DWORD				s_nTLSIndex;
	static CRITICAL_SECTION		s_sect;
	static LinkedCallContext*	s_pHeadCtx;
	static LinkedCallContext*   s_pCachedCtx;

    CComBSTR m_bstrName;
};

inline CoDelegator* CoDelegator::SplitIdentity::Trunk()
{
	return (CoDelegator*)(((BYTE*)this)-offsetof(CoDelegator, m_splitIdentity));
}

#endif // _CODELEGATOR_H
