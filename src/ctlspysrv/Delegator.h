/////////////////////////////////////////////////////////////
// Delegator.h - Generic Delegator Component
//
// Copyright 1998, Keith Brown
//
// The Delegator class holds a single interface pointer and
// implements the delegating unknown, as well as the auto-
// forwarding of method calls to the inner object.
// Delegator objects notify their CoDelegator parent when
// their individual refcount drops to zero so the parent
// can destroy them and remove them from the collection.
/////////////////////////////////////////////////////////////
#ifndef _DELEGATOR_H
#define _DELEGATOR_H

#include "delegate.h"
#include "ItfTypeInfo.h"
#include "methodNames.h"

class CoDelegator;
interface IDelegatorHookMethods;

//---------------------------------------------------------------------------//
//The delegator structure acts a tearoff for each interface we export.  It knows
//the IID and refcount for the interface.  Thus, per-interface reference counting
//is very easy to manage.

//That makes this structure a very convenient place to store interface and method
//options.  For example, the user will be able to hide interfaces by toggling an option 
//here.

//The m_pHook member means something different here than in the original delegator.  We
//will always do pre/post method processing right here on the delegator.  We may optionally
//hand off to a custom hook.
struct Delegator
{
	static const void* _assignVptr( DWORD grfOptions )
	{
    //JW: We always do postprocessing, regardless of the options passed in.  The
    //pre- and post-processing takes place right here.  Custom hooks may be brought
    //in later.
	
    return (grfOptions & DHO_POSTPROCESS_METHODS) ? s_vptr2 : s_vptr;
	}

    public:
    Delegator( CoDelegator& parent, IUnknown* pUnkInner, const IID& iid,
				DWORD grfOptions, IDelegatorHookMethods2* pHook,
				ItfTypeInfo* pTypeInfo );

    ~Delegator();

    STDMETHODIMP DelegatorPreprocess(DWORD nVtblIndex, void* pArgs, void** ppHookDefinedData );
    STDMETHODIMP DelegatorPostprocess(DWORD nVtblIndex, HRESULT hrFromInner, void* pHookDefinedData );

    ULONG InternalAddRef();

	// this flag will be set in m_grf if the QI hook returned
	// a failure code from OnFirstDelegatorQIFor
	enum { DONT_EXPOSE_FROM_QI = 0x80000000 };

	const void*	const		m_vptr;			// ORDER_DEPENDENCY (this + 0 bytes)
	IUnknown*				m_pUnkInner;	// ORDER_DEPENDENCY (this + 4 bytes)
	DWORD					m_grf;			// ORDER_DEPENDENCY (this + 8 bytes)
	CoDelegator&			m_parent;
	IDelegatorHookMethods2*	m_pHook;         //used for custom processing
	const IID				m_iid;
	long					m_cRefs;		// refcount for this interface
	ItfTypeInfo*			m_pTypeInfo;	// used for inhibiting method calls

	static const void* const s_vptr;		// used when only preprocessing is desired
	static const void* const s_vptr2;		// used when postprocessing also needed

    //CComBSTR m_bstrIfaceName;               //friendly name for this interface
};

#endif // _DELEGATOR_H
