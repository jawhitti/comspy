/////////////////////////////////////////////////////////////
// Delegator.cpp - COMSpy
//
// Copyright 2000, Jason Whittington, Keith Brown
//
//This code is based on Keith Brown's universal delegator
//code, but it has been tweaked by JW.
/////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Delegator.h"
#include "CoDelegator.h"

Delegator::Delegator( CoDelegator& parent, IUnknown* pUnkInner, const IID& iid,
				DWORD grfOptions, IDelegatorHookMethods2* pHook,
				ItfTypeInfo* pTypeInfo )
	  : m_vptr( _assignVptr( grfOptions ) ),
		m_pUnkInner( pUnkInner ),
		m_grf( grfOptions ),
		m_parent( parent ),
		m_pHook( pHook ),
		m_iid( iid ),
		m_cRefs( 0 ),
		m_pTypeInfo( pTypeInfo ) // we don't own this
	{
    //ATLTRACE("Delegator %x created\n", this);

	m_pUnkInner->AddRef();
		//if ( m_pHook )
		//	m_pHook->AddRef();
	}


ULONG Delegator::InternalAddRef()
    {
	// thread-safe - only the initial thread hits both resources
	if ( 0 == this->m_cRefs )
        {
		this->m_parent.GetOuter()->AddRef();
        }
	return InterlockedIncrement( &m_cRefs );
    }

Delegator::~Delegator()
	{
    //ATLTRACE("Delegator %x deleted\n", this);
	//	if ( m_pHook )
	//		m_pHook->Release();
		m_pUnkInner->Release();
	}


STDMETHODIMP Delegator::DelegatorPreprocess(DWORD nVtblIndex, void* pArgs, void** ppHookDefinedData )
    {
    //For most method calls we won't need to do anything here.  The only time we do anything interesting
    //is when we hit a breakpoint.  I think we will push the interesting functionality off onto _Module.
    ATLTRACE("Preprocessing thread 0x%x (%s)\n", GetCurrentThreadId(), GetApartmentType());
    _Module.Preprocess(&m_parent, m_iid, nVtblIndex, pArgs, ppHookDefinedData);

    //BSTR bstrMethodName = NULL;
    //GetMethodName(m_iid, nVtblIndex, &bstrMethodName);
    //ATLTRACE("%S::%S::%S...", m_parent.m_bstrName, m_bstrIfaceName, bstrMethodName); 
    //::SysFreeString(bstrMethodName);
    return S_OK;
    }

STDMETHODIMP Delegator::DelegatorPostprocess(DWORD nVtblIndex, HRESULT hrFromInner, void* pHookDefinedData )
    {
    //We will want to output the Object::Interface::method::HRESULT from here.  If we have NOT hit a 
    //breakpoint then we will PostThreadMessage here with the information.

    //If we DO hit a breakpoint we will have to handle things a bit differently.  I would like to do an
    //"active wait" so that the outside world can come in an poke around.  Naturally the debugger will want
    //a way to change the HRESULT.

    //The only thing that I see keeping us from making out of apartment calls here is our good friend
    //[input_sync].  There are few enough of these methods around, however, that I'm comfortable spinning
    //in a message loop, especially if we can _exclude_ windowing events and only allow COM method calls
    //through.  All this said, I doubt I'll handle things any differently in _this_ routine - I'll just
    //notify _Module.

    //ATLTRACE("0x%08X\n", hrFromInner);
    HRESULT hr = _Module.PostProcess(&m_parent, m_iid, nVtblIndex, hrFromInner, pHookDefinedData);
    //ATLTRACE("Postprocessing thread 0x%x\n", GetCurrentThreadId());
    return hr;
    }


// delegating unknown
STDMETHODIMP Delegator_QueryInterface( Delegator* pThis, REFIID iid, void** ppv )
{
	HRESULT hr = pThis->m_parent.GetOuter()->QueryInterface( iid, ppv );
    if(SUCCEEDED(hr)) {ATLASSERT(*ppv != 0);}
    if(FAILED(hr)) {ATLASSERT(*ppv == 0);}
    
    return hr;
}

STDMETHODIMP_(ULONG) Delegator_AddRef( Delegator* pThis )
{
    //Notify _Module of our refcount change
    _Module.OnRefcountChanged(&(pThis->m_parent), pThis->m_iid, pThis->m_cRefs, (pThis->m_cRefs)+1);
    //ATLTRACE("Delegator_AddRef\n");

    return pThis->InternalAddRef();
}

STDMETHODIMP_(ULONG) Delegator_Release( Delegator* pThis )
{
    //Notify _Module of our refcount change
    _Module.OnRefcountChanged(&(pThis->m_parent), pThis->m_iid, pThis->m_cRefs, (pThis->m_cRefs)-1);
    //Notify the outer of our release as well...
    //pThis->m_parent.Release();

	const long cRefs = InterlockedDecrement( &pThis->m_cRefs );
	if ( 0 != cRefs )
		return cRefs;
    //ATLTRACE("Delegator_Release\n");
	return pThis->m_parent.OnDelegatorFinalRelease( pThis );
}

#pragma code_seg(".orpc")

// this method is called when *only* preprocessing is required
// preprocess( args, this, retaddr, nVtbloffset, ebp, stackSize, hr );
// Note that this function is called in an odd way to avoid copying
// the arguments onto the stack (for efficiency and simplicity in the calling code)
// This is why the call is explicitly declared __cdecl
DWORD __cdecl preprocess(
							volatile HRESULT hrFromPreprocess,
							volatile DWORD stackSize,
							DWORD /*ebp*/,
							DWORD nVtblOffset,
							void* /*retaddr*/,
							Delegator& d,
							DWORD argStart
							 )
{
    //ATLTRACE("Preprocessing (%d)...", GetCurrentThreadId());
	const DWORD nMethod = nVtblOffset / sizeof( void* );
	//hrFromPreprocess = d.m_pHook->DelegatorPreprocess( nMethod, &argStart, 0 );
	hrFromPreprocess = d.DelegatorPreprocess( nMethod, &argStart, 0 );
	if ( FAILED( hrFromPreprocess ) && ( d.m_grf & DHO_MAY_BLOCK_CALLS ) )
	{
		stackSize = d.m_pTypeInfo->GetStackSize( nMethod );
		return 0;
	}
    //ATLTRACE("Ok.\n");
	return 1;
}

// this method is called if we need to postprocess as well
DWORD __cdecl preprocess2(
							volatile DWORD hrFromPreprocess,
							volatile DWORD stackSize,
							DWORD /*ebp*/,
							DWORD nVtblOffset,
							DWORD /*localVar*/,
							DWORD /*localVar*/,
							const void* pReturnAddr,
							Delegator& d,
							DWORD argStart )
{
	const DWORD nMethod = nVtblOffset / sizeof( void* );
    ATLTRACE("preprocess2(0x%x)\n", GetCurrentThreadId());
	// first try to acquire a buffer - if this fails, we cannot do any delegation
	// at all - just shunt the method directly to the inner object and forget it.
	CallContext* pcc = CoDelegator::PushNewCallContext( d, pReturnAddr, nVtblOffset );
	if ( !pcc )
	{
		// If the delegator supports inhibiting calls,
		// don't allow this call to leak through.
		if ( d.m_grf & DHO_MAY_BLOCK_CALLS )
		{
			stackSize = d.m_pTypeInfo->GetStackSize( nMethod );
			hrFromPreprocess = E_OUTOFMEMORY;
			return 1;	// inhibit call
		}
		else return 2;  // delegate call without pre- or post-processing
	}

	if ( DHO_PREPROCESS_METHODS & d.m_grf )
	{
		//hrFromPreprocess = d.m_pHook->DelegatorPreprocess( nMethod, &argStart, &pcc->m_pData );
		hrFromPreprocess = d.DelegatorPreprocess( nMethod, &argStart, &pcc->m_pData );
		if ( FAILED( hrFromPreprocess ) && ( d.m_grf & DHO_MAY_BLOCK_CALLS ) )
		{
			stackSize = d.m_pTypeInfo->GetStackSize( nMethod );
			return 1;	// inhibit call
		}
	}
	return 0;	// delegate call as normal
}

HRESULT __stdcall postprocess( HRESULT hrFromInner,
								const void** ppReturnAddr )
{
	// get the call context back from TLS
    ATLTRACE("postprocess(0x%x)...", GetCurrentThreadId());

 	CallContext* const pcc = CoDelegator::PopCallContext();

	HRESULT hr = pcc->m_pDelegator->DelegatorPostprocess( pcc->m_nVtblOffset / sizeof( void* ), hrFromInner, pcc->m_pData );

	*ppReturnAddr = pcc->m_pReturnAddr;
	CoDelegator::DeleteCallContext( pcc );

    ATLTRACE("Ok\n");

	return hr;
}

static __declspec(naked) void delegate(void)
{
	__asm
	{
		push ebp			// set up simple stack frame
		mov  ebp, esp

		sub  esp, 8			// set up local and register vars
		// localVar(hrFromPreprocess)/localVar(stackSize)/ebp/nVtblOffset/retaddr/this/args

		mov  eax, [ebp+12]	// eax = this
		mov  eax, [eax+8]	// if ( 0 == ( this->m_grf & 1 ) )
		test eax, 1			//   goto delegateCall;
		jz delegateCall

		call preprocess		// eax = preprocess() - function shares our stack frame for efficiency

		test eax, 1			// check to see if delegation was inhibited
		jnz delegateCall
							// the following code adjusts the stack and returns directly
							// to the caller, without delegating the call.
							// This involves copying the return address and the HRESULT
							// to the bottom of the stack frame, adjusting the stack
							// pointer, and returning to the caller.
		push esi
		mov  esi, [ebp-4]   // esi = stackSize
		add  esi, 8
		add  esi, ebp		// esi points to bottom arg on stack
		
		mov  eax, [ebp+8]	// copy retaddr down
		mov  [esi], eax
		sub  esi, 4
		mov  eax, [ebp-8]	// copy hrFromPreprocess down
		mov  [esi], eax
		
		mov  eax, esi		// reset stack and return to caller
		pop  esi
		mov  ebp, [ebp]
		mov  esp, eax
		pop  eax
		ret

	delegateCall:
		
		mov  eax, [ebp+12]	// eax = this = pInner
		mov  eax, [eax+4]
		mov  [ebp+12], eax	

		mov  eax, [eax]
		add  eax, [ebp+4]
		mov  eax, [eax]		// eax = address of sink's virtual function

		add  esp, 8
		pop  ebp
		mov  [esp], eax		// overwrite nVtblOffset on stack with inner's fcn ptr
		ret					// pop fcn ptr off stack and jump to inner's fcn
	}
}

static __declspec(naked) void delegateAndPostprocess(void)
{
	__asm
	{
		// get the vtbl index
		pop  eax			// eax = vtbl index (in bytes)
		sub  esp, 8
		push eax
		push ebp			// set up simple stack frame
		mov  ebp, esp
		sub  esp, 8

		// ebp-8 = local variable: hrFromPreprocess
		// ebp-4  = local variable: stackSize
		// ebp+0  = ebp
		// ebp+4  = local variable: vtbl offset (in bytes)
		// ebp+8  = local variable: result of context allocation
		// ebp+12 = local variable: address of inner's method
		// ebp+16 = retaddr
		// ebp+20 = this
		// ebp+24 = args

		call preprocess2	// eax = preprocess() - function shares our stack frame for efficiency

		test eax, 1			// see if we should block the call
		jz   continueWithCall
							// the following code adjusts the stack and returns directly
							// to the caller, without delegating the call.
							// This involves copying the return address and the HRESULT
							// to the bottom of the stack frame, adjusting the stack
							// pointer, and returning to the caller.
		push esi
		mov  esi, [ebp-4]   // esi = stackSize
		add  esi, 16
		add  esi, ebp		// esi points to bottom arg on stack
		
		mov  eax, [ebp+16]	// copy retaddr down
		mov  [esi], eax
		sub  esi, 4
		mov  eax, [ebp-8]	// copy hrFromPreprocess down
		mov  [esi], eax
		
		mov  eax, esi		// reset stack and return to caller
		pop  esi
		mov  ebp, [ebp]
		mov  esp, eax
		pop  eax
		ret

	continueWithCall:
		mov  [ebp+8], eax	// store result of context allocation

		mov  eax, [ebp+20]	// this = eax = pInner
		mov  eax, [eax+4]
		mov  [ebp+20], eax	

		mov  eax, [eax]		// store address of inner's virtual function
		add  eax, [ebp+4]
		mov  eax, [eax]		
		mov  [ebp+12], eax

		add  esp, 8			// tear down stack frame
		pop  ebp
		pop  eax			// discard vtbl offset

		pop  eax			// was context alloc successful?
		test eax, 2
		jz  allocSuccessful

		pop	 eax			// delegate without postprocessing
		jmp  eax

	allocSuccessful:
		pop  eax
		add  esp, 4			// remove caller's return addr from stack and call inner
		call eax

		sub  esp, 4			// make room for original return addr
		push esp			// eax = postprocess( eax, ppReturnAddr )
		push eax
		call postprocess

		ret
	}
}

#define DELEGATOR_ENTRY_POINTS(n) \
static void __declspec(naked) del_##n(void)  \
{ __asm push (n*4) __asm jmp delegate }		 \
static void __declspec(naked) del2_##n(void) \
{ __asm push (n*4) __asm jmp delegateAndPostprocess }

#include "entrypoints.inc"
