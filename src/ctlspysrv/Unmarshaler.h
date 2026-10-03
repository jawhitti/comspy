/////////////////////////////////////////////////////////////
// Unmarshaler.h - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
// This is the COM class that is responsible for unmarshaling
// interface pointers from delegators having various DO_MBV_XXXX
// options set. The reason this is a separate class is to
// to support the DO_MAINTAIN_IDENTITY option.
/////////////////////////////////////////////////////////////
#ifndef __UNMARSHALER_H
#define __UNMARSHALER_H

class CoDelegator;
interface IDelegatorHookQI;

//---------------------------------------------------------------------------//
// Unmarshaler - used for MBV and MBV + identity
//---------------------------------------------------------------------------//
class __declspec(uuid("9b8c32f4-249a-11d2-a7bb-006008d25ccf"))
	Unmarshaler : public IMarshal, public IClassFactory
{
public:
	STDMETHODIMP QueryInterface( REFIID iid, void** ppv );
	STDMETHODIMP_(ULONG) AddRef();
	STDMETHODIMP_(ULONG) Release();

	STDMETHODIMP GetUnmarshalClass( REFIID iid, void* pv, DWORD grfDestCtx, void*, DWORD grfMshFlags, CLSID* pClsid );
	STDMETHODIMP GetMarshalSizeMax( REFIID iid, void* pv, DWORD grfDestCtx, void*, DWORD grfMshFlags, ULONG* pcb );
	STDMETHODIMP MarshalInterface( IStream* pstm, REFIID iid, void* pv, DWORD grfDestCtx, void*, DWORD grfMshFlags );
	STDMETHODIMP ReleaseMarshalData( IStream* pstm );
	STDMETHODIMP UnmarshalInterface( IStream* pstm, REFIID iid, void** ppv );
	STDMETHODIMP DisconnectObject( DWORD );

	STDMETHODIMP CreateInstance( IUnknown*, REFIID iid, void** ppv );
	STDMETHODIMP LockServer( BOOL bLock );

	static bool Startup();
	static void Shutdown();
	static ULONG LockDictionaryAndReleaseDelegator( CoDelegator& d );
	static HRESULT LazyRegisterIdentity( IUnknown* pUnkInner, IUnknown* pUnkDelegator );
	static Unmarshaler& TheUnmarshaler();
private:
	HRESULT _createHook( IStream* pstm, REFCLSID clsidHook, IUnknown* pUnkInner, bool bSerializedHookState, IDelegatorHookQI*& pHook );
	HRESULT _createNew( IStream* pstm, IUnknown* pUnkInner, DWORD grf, REFCLSID clsidHook, bool bSerializedHookState, IUnknown*& pUnkDelegator );
	IUnknown* _lookupExisting( IUnknown* pUnkInner );

	static CRITICAL_SECTION s_sect;
};

#endif // __UNMARSHALER_H
