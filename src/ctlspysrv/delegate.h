/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Mon Mar 08 22:30:57 1999
 */
/* Compiler settings for delegate.idl:
    Os (OptLev=s), W1, Zp8, env=Win32, ms_ext, c_ext
    error checks: allocation ref bounds_check enum stub_data 
*/
//@@MIDL_FILE_HEADING(  )


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 440
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __delegate_h__
#define __delegate_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDelegatorHookMethods_FWD_DEFINED__
#define __IDelegatorHookMethods_FWD_DEFINED__
typedef interface IDelegatorHookMethods IDelegatorHookMethods;
#endif 	/* __IDelegatorHookMethods_FWD_DEFINED__ */


#ifndef __IDelegatorHookMethods2_FWD_DEFINED__
#define __IDelegatorHookMethods2_FWD_DEFINED__
typedef interface IDelegatorHookMethods2 IDelegatorHookMethods2;
#endif 	/* __IDelegatorHookMethods2_FWD_DEFINED__ */


#ifndef __IDelegatorGetCallContext_FWD_DEFINED__
#define __IDelegatorGetCallContext_FWD_DEFINED__
typedef interface IDelegatorGetCallContext IDelegatorGetCallContext;
#endif 	/* __IDelegatorGetCallContext_FWD_DEFINED__ */


#ifndef __IDelegatorHookQI_FWD_DEFINED__
#define __IDelegatorHookQI_FWD_DEFINED__
typedef interface IDelegatorHookQI IDelegatorHookQI;
#endif 	/* __IDelegatorHookQI_FWD_DEFINED__ */


#ifndef __IDelegatorSplitIdentity_FWD_DEFINED__
#define __IDelegatorSplitIdentity_FWD_DEFINED__
typedef interface IDelegatorSplitIdentity IDelegatorSplitIdentity;
#endif 	/* __IDelegatorSplitIdentity_FWD_DEFINED__ */


#ifndef __IDelegatorConnection_FWD_DEFINED__
#define __IDelegatorConnection_FWD_DEFINED__
typedef interface IDelegatorConnection IDelegatorConnection;
#endif 	/* __IDelegatorConnection_FWD_DEFINED__ */


#ifndef __IDelegatorFactory_FWD_DEFINED__
#define __IDelegatorFactory_FWD_DEFINED__
typedef interface IDelegatorFactory IDelegatorFactory;
#endif 	/* __IDelegatorFactory_FWD_DEFINED__ */


#ifndef __IAlternateCredentialDelegatorHook_FWD_DEFINED__
#define __IAlternateCredentialDelegatorHook_FWD_DEFINED__
typedef interface IAlternateCredentialDelegatorHook IAlternateCredentialDelegatorHook;
#endif 	/* __IAlternateCredentialDelegatorHook_FWD_DEFINED__ */


#ifndef __IDelegatorFactory0_FWD_DEFINED__
#define __IDelegatorFactory0_FWD_DEFINED__
typedef interface IDelegatorFactory0 IDelegatorFactory0;
#endif 	/* __IDelegatorFactory0_FWD_DEFINED__ */


#ifndef __CoDelegator21_FWD_DEFINED__
#define __CoDelegator21_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoDelegator21 CoDelegator21;
#else
typedef struct CoDelegator21 CoDelegator21;
#endif /* __cplusplus */

#endif 	/* __CoDelegator21_FWD_DEFINED__ */


#ifndef __CoDelegator20_FWD_DEFINED__
#define __CoDelegator20_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoDelegator20 CoDelegator20;
#else
typedef struct CoDelegator20 CoDelegator20;
#endif /* __cplusplus */

#endif 	/* __CoDelegator20_FWD_DEFINED__ */


#ifndef __CoDelegator10_FWD_DEFINED__
#define __CoDelegator10_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoDelegator10 CoDelegator10;
#else
typedef struct CoDelegator10 CoDelegator10;
#endif /* __cplusplus */

#endif 	/* __CoDelegator10_FWD_DEFINED__ */


#ifndef __CoDelegatorGetCallContext_FWD_DEFINED__
#define __CoDelegatorGetCallContext_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoDelegatorGetCallContext CoDelegatorGetCallContext;
#else
typedef struct CoDelegatorGetCallContext CoDelegatorGetCallContext;
#endif /* __cplusplus */

#endif 	/* __CoDelegatorGetCallContext_FWD_DEFINED__ */


#ifndef __CoAnonymousDelegatorHook_FWD_DEFINED__
#define __CoAnonymousDelegatorHook_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoAnonymousDelegatorHook CoAnonymousDelegatorHook;
#else
typedef struct CoAnonymousDelegatorHook CoAnonymousDelegatorHook;
#endif /* __cplusplus */

#endif 	/* __CoAnonymousDelegatorHook_FWD_DEFINED__ */


#ifndef __CoAlternateCredentialDelegatorHook_FWD_DEFINED__
#define __CoAlternateCredentialDelegatorHook_FWD_DEFINED__

#ifdef __cplusplus
typedef class CoAlternateCredentialDelegatorHook CoAlternateCredentialDelegatorHook;
#else
typedef struct CoAlternateCredentialDelegatorHook CoAlternateCredentialDelegatorHook;
#endif /* __cplusplus */

#endif 	/* __CoAlternateCredentialDelegatorHook_FWD_DEFINED__ */


/* header files for imported files */
#include "unknwn.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

#ifndef __IDelegatorHookMethods_INTERFACE_DEFINED__
#define __IDelegatorHookMethods_INTERFACE_DEFINED__

/* interface IDelegatorHookMethods */
/* [local][object][uuid] */ 


EXTERN_C const IID IID_IDelegatorHookMethods;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9b8c32f1-249a-11d2-a7bb-006008d25ccf")
    IDelegatorHookMethods : public IUnknown
    {
    public:
        virtual void STDMETHODCALLTYPE DelegatorPreprocess( 
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ void __RPC_FAR *pArgs,
            /* [out] */ DWORD __RPC_FAR *pnCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DelegatorPostprocess( 
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ HRESULT hrFromInner,
            /* [in] */ DWORD nCookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorHookMethodsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorHookMethods __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorHookMethods __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorHookMethods __RPC_FAR * This);
        
        void ( STDMETHODCALLTYPE __RPC_FAR *DelegatorPreprocess )( 
            IDelegatorHookMethods __RPC_FAR * This,
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ void __RPC_FAR *pArgs,
            /* [out] */ DWORD __RPC_FAR *pnCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DelegatorPostprocess )( 
            IDelegatorHookMethods __RPC_FAR * This,
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ HRESULT hrFromInner,
            /* [in] */ DWORD nCookie);
        
        END_INTERFACE
    } IDelegatorHookMethodsVtbl;

    interface IDelegatorHookMethods
    {
        CONST_VTBL struct IDelegatorHookMethodsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorHookMethods_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorHookMethods_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorHookMethods_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorHookMethods_DelegatorPreprocess(This,nVtblIndex,pArgs,pnCookie)	\
    (This)->lpVtbl -> DelegatorPreprocess(This,nVtblIndex,pArgs,pnCookie)

#define IDelegatorHookMethods_DelegatorPostprocess(This,nVtblIndex,hrFromInner,nCookie)	\
    (This)->lpVtbl -> DelegatorPostprocess(This,nVtblIndex,hrFromInner,nCookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



void STDMETHODCALLTYPE IDelegatorHookMethods_DelegatorPreprocess_Proxy( 
    IDelegatorHookMethods __RPC_FAR * This,
    /* [in] */ DWORD nVtblIndex,
    /* [in] */ void __RPC_FAR *pArgs,
    /* [out] */ DWORD __RPC_FAR *pnCookie);


void __RPC_STUB IDelegatorHookMethods_DelegatorPreprocess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDelegatorHookMethods_DelegatorPostprocess_Proxy( 
    IDelegatorHookMethods __RPC_FAR * This,
    /* [in] */ DWORD nVtblIndex,
    /* [in] */ HRESULT hrFromInner,
    /* [in] */ DWORD nCookie);


void __RPC_STUB IDelegatorHookMethods_DelegatorPostprocess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorHookMethods_INTERFACE_DEFINED__ */


#ifndef __IDelegatorHookMethods2_INTERFACE_DEFINED__
#define __IDelegatorHookMethods2_INTERFACE_DEFINED__

/* interface IDelegatorHookMethods2 */
/* [local][object][uuid] */ 


EXTERN_C const IID IID_IDelegatorHookMethods2;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("5f8bea30-60ce-11d2-a7ea-006008d25ccf")
    IDelegatorHookMethods2 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DelegatorPreprocess( 
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ void __RPC_FAR *pArgs,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DelegatorPostprocess( 
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ HRESULT hrFromInner,
            /* [in] */ void __RPC_FAR *pv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorHookMethods2Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorHookMethods2 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorHookMethods2 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorHookMethods2 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DelegatorPreprocess )( 
            IDelegatorHookMethods2 __RPC_FAR * This,
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ void __RPC_FAR *pArgs,
            /* [out] */ void __RPC_FAR *__RPC_FAR *ppv);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DelegatorPostprocess )( 
            IDelegatorHookMethods2 __RPC_FAR * This,
            /* [in] */ DWORD nVtblIndex,
            /* [in] */ HRESULT hrFromInner,
            /* [in] */ void __RPC_FAR *pv);
        
        END_INTERFACE
    } IDelegatorHookMethods2Vtbl;

    interface IDelegatorHookMethods2
    {
        CONST_VTBL struct IDelegatorHookMethods2Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorHookMethods2_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorHookMethods2_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorHookMethods2_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorHookMethods2_DelegatorPreprocess(This,nVtblIndex,pArgs,ppv)	\
    (This)->lpVtbl -> DelegatorPreprocess(This,nVtblIndex,pArgs,ppv)

#define IDelegatorHookMethods2_DelegatorPostprocess(This,nVtblIndex,hrFromInner,pv)	\
    (This)->lpVtbl -> DelegatorPostprocess(This,nVtblIndex,hrFromInner,pv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorHookMethods2_DelegatorPreprocess_Proxy( 
    IDelegatorHookMethods2 __RPC_FAR * This,
    /* [in] */ DWORD nVtblIndex,
    /* [in] */ void __RPC_FAR *pArgs,
    /* [out] */ void __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDelegatorHookMethods2_DelegatorPreprocess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDelegatorHookMethods2_DelegatorPostprocess_Proxy( 
    IDelegatorHookMethods2 __RPC_FAR * This,
    /* [in] */ DWORD nVtblIndex,
    /* [in] */ HRESULT hrFromInner,
    /* [in] */ void __RPC_FAR *pv);


void __RPC_STUB IDelegatorHookMethods2_DelegatorPostprocess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorHookMethods2_INTERFACE_DEFINED__ */


#ifndef __IDelegatorGetCallContext_INTERFACE_DEFINED__
#define __IDelegatorGetCallContext_INTERFACE_DEFINED__

/* interface IDelegatorGetCallContext */
/* [local][object][uuid] */ 


EXTERN_C const IID IID_IDelegatorGetCallContext;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("bd8f05b0-9145-11d2-a802-006008d25ccf")
    IDelegatorGetCallContext : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetCallContext( 
            /* [retval][out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorGetCallContextVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorGetCallContext __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorGetCallContext __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorGetCallContext __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetCallContext )( 
            IDelegatorGetCallContext __RPC_FAR * This,
            /* [retval][out] */ void __RPC_FAR *__RPC_FAR *ppv);
        
        END_INTERFACE
    } IDelegatorGetCallContextVtbl;

    interface IDelegatorGetCallContext
    {
        CONST_VTBL struct IDelegatorGetCallContextVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorGetCallContext_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorGetCallContext_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorGetCallContext_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorGetCallContext_GetCallContext(This,ppv)	\
    (This)->lpVtbl -> GetCallContext(This,ppv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorGetCallContext_GetCallContext_Proxy( 
    IDelegatorGetCallContext __RPC_FAR * This,
    /* [retval][out] */ void __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDelegatorGetCallContext_GetCallContext_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorGetCallContext_INTERFACE_DEFINED__ */


#ifndef __IDelegatorHookQI_INTERFACE_DEFINED__
#define __IDelegatorHookQI_INTERFACE_DEFINED__

/* interface IDelegatorHookQI */
/* [local][object][uuid] */ 

typedef 
enum _DelegatorHookOptions
    {	DHO_PREPROCESS_METHODS	= 0x1,
	DHO_POSTPROCESS_METHODS	= 0x2,
	DHO_MAY_BLOCK_CALLS	= 0x4
    }	DelegatorHookOptions;


EXTERN_C const IID IID_IDelegatorHookQI;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9b8c32f9-249a-11d2-a7bb-006008d25ccf")
    IDelegatorHookQI : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ IUnknown __RPC_FAR *pUnkInner) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnFirstDelegatorQIFor( 
            /* [in] */ REFIID iid,
            /* [iid_is][in] */ IUnknown __RPC_FAR *pItfInner,
            /* [out] */ DWORD __RPC_FAR *pgrfDelegatorHookOptions,
            /* [in] */ REFIID iidMethodHook,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppMethodHook) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorHookQIVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorHookQI __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorHookQI __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorHookQI __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IDelegatorHookQI __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkInner);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnFirstDelegatorQIFor )( 
            IDelegatorHookQI __RPC_FAR * This,
            /* [in] */ REFIID iid,
            /* [iid_is][in] */ IUnknown __RPC_FAR *pItfInner,
            /* [out] */ DWORD __RPC_FAR *pgrfDelegatorHookOptions,
            /* [in] */ REFIID iidMethodHook,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppMethodHook);
        
        END_INTERFACE
    } IDelegatorHookQIVtbl;

    interface IDelegatorHookQI
    {
        CONST_VTBL struct IDelegatorHookQIVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorHookQI_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorHookQI_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorHookQI_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorHookQI_Init(This,pUnkInner)	\
    (This)->lpVtbl -> Init(This,pUnkInner)

#define IDelegatorHookQI_OnFirstDelegatorQIFor(This,iid,pItfInner,pgrfDelegatorHookOptions,iidMethodHook,ppMethodHook)	\
    (This)->lpVtbl -> OnFirstDelegatorQIFor(This,iid,pItfInner,pgrfDelegatorHookOptions,iidMethodHook,ppMethodHook)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorHookQI_Init_Proxy( 
    IDelegatorHookQI __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkInner);


void __RPC_STUB IDelegatorHookQI_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDelegatorHookQI_OnFirstDelegatorQIFor_Proxy( 
    IDelegatorHookQI __RPC_FAR * This,
    /* [in] */ REFIID iid,
    /* [iid_is][in] */ IUnknown __RPC_FAR *pItfInner,
    /* [out] */ DWORD __RPC_FAR *pgrfDelegatorHookOptions,
    /* [in] */ REFIID iidMethodHook,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppMethodHook);


void __RPC_STUB IDelegatorHookQI_OnFirstDelegatorQIFor_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorHookQI_INTERFACE_DEFINED__ */


#ifndef __IDelegatorSplitIdentity_INTERFACE_DEFINED__
#define __IDelegatorSplitIdentity_INTERFACE_DEFINED__

/* interface IDelegatorSplitIdentity */
/* [local][object][uuid] */ 


EXTERN_C const IID IID_IDelegatorSplitIdentity;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6bf39960-7173-11d2-a7fa-006008d25ccf")
    IDelegatorSplitIdentity : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DelegatorSafeRef( 
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetHook( 
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorSplitIdentityVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorSplitIdentity __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorSplitIdentity __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorSplitIdentity __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DelegatorSafeRef )( 
            IDelegatorSplitIdentity __RPC_FAR * This,
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetHook )( 
            IDelegatorSplitIdentity __RPC_FAR * This,
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);
        
        END_INTERFACE
    } IDelegatorSplitIdentityVtbl;

    interface IDelegatorSplitIdentity
    {
        CONST_VTBL struct IDelegatorSplitIdentityVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorSplitIdentity_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorSplitIdentity_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorSplitIdentity_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorSplitIdentity_DelegatorSafeRef(This,iid,ppv)	\
    (This)->lpVtbl -> DelegatorSafeRef(This,iid,ppv)

#define IDelegatorSplitIdentity_GetHook(This,iid,ppv)	\
    (This)->lpVtbl -> GetHook(This,iid,ppv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorSplitIdentity_DelegatorSafeRef_Proxy( 
    IDelegatorSplitIdentity __RPC_FAR * This,
    /* [in] */ REFIID iid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDelegatorSplitIdentity_DelegatorSafeRef_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDelegatorSplitIdentity_GetHook_Proxy( 
    IDelegatorSplitIdentity __RPC_FAR * This,
    /* [in] */ REFIID iid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDelegatorSplitIdentity_GetHook_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorSplitIdentity_INTERFACE_DEFINED__ */


#ifndef __IDelegatorConnection_INTERFACE_DEFINED__
#define __IDelegatorConnection_INTERFACE_DEFINED__

/* interface IDelegatorConnection */
/* [local][object][uuid] */ 


EXTERN_C const IID IID_IDelegatorConnection;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("6bf39961-7173-11d2-a7fa-006008d25ccf")
    IDelegatorConnection : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE DelegatorConnect( 
            /* [in] */ IDelegatorSplitIdentity __RPC_FAR *pdsi) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE DelegatorDisconnect( void) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorConnectionVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorConnection __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorConnection __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorConnection __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DelegatorConnect )( 
            IDelegatorConnection __RPC_FAR * This,
            /* [in] */ IDelegatorSplitIdentity __RPC_FAR *pdsi);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *DelegatorDisconnect )( 
            IDelegatorConnection __RPC_FAR * This);
        
        END_INTERFACE
    } IDelegatorConnectionVtbl;

    interface IDelegatorConnection
    {
        CONST_VTBL struct IDelegatorConnectionVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorConnection_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorConnection_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorConnection_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorConnection_DelegatorConnect(This,pdsi)	\
    (This)->lpVtbl -> DelegatorConnect(This,pdsi)

#define IDelegatorConnection_DelegatorDisconnect(This)	\
    (This)->lpVtbl -> DelegatorDisconnect(This)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorConnection_DelegatorConnect_Proxy( 
    IDelegatorConnection __RPC_FAR * This,
    /* [in] */ IDelegatorSplitIdentity __RPC_FAR *pdsi);


void __RPC_STUB IDelegatorConnection_DelegatorConnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDelegatorConnection_DelegatorDisconnect_Proxy( 
    IDelegatorConnection __RPC_FAR * This);


void __RPC_STUB IDelegatorConnection_DelegatorDisconnect_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorConnection_INTERFACE_DEFINED__ */


#ifndef __IDelegatorFactory_INTERFACE_DEFINED__
#define __IDelegatorFactory_INTERFACE_DEFINED__

/* interface IDelegatorFactory */
/* [local][object][uuid] */ 

typedef 
enum _DelegatorOptions
    {	DO_MBV_INPROC	= 0x1,
	DO_MBV_LOCAL	= 0x2,
	DO_MBV_DIFFERENTMACHINE	= 0x4,
	DO_MBV_ALL	= 0x7,
	DO_MAINTAIN_IDENTITY	= 0x8,
	DO_CACHE_INTERFACES	= 0x10,
	DO_CONNECTION_NOTIFICATION	= 0x20
    }	DelegatorOptions;


EXTERN_C const IID IID_IDelegatorFactory;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9b8c32f3-249a-11d2-a7bb-006008d25ccf")
    IDelegatorFactory : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateDelegator( 
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ IUnknown __RPC_FAR *pUnkInner,
            /* [in] */ CLSID __RPC_FAR *pclsidHook,
            /* [in] */ IUnknown __RPC_FAR *pHook,
            /* [in] */ DWORD grfOptions,
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorFactoryVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorFactory __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorFactory __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorFactory __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDelegator )( 
            IDelegatorFactory __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ IUnknown __RPC_FAR *pUnkInner,
            /* [in] */ CLSID __RPC_FAR *pclsidHook,
            /* [in] */ IUnknown __RPC_FAR *pHook,
            /* [in] */ DWORD grfOptions,
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);
        
        END_INTERFACE
    } IDelegatorFactoryVtbl;

    interface IDelegatorFactory
    {
        CONST_VTBL struct IDelegatorFactoryVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorFactory_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorFactory_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorFactory_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorFactory_CreateDelegator(This,pUnkOuter,pUnkInner,pclsidHook,pHook,grfOptions,iid,ppv)	\
    (This)->lpVtbl -> CreateDelegator(This,pUnkOuter,pUnkInner,pclsidHook,pHook,grfOptions,iid,ppv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorFactory_CreateDelegator_Proxy( 
    IDelegatorFactory __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
    /* [in] */ IUnknown __RPC_FAR *pUnkInner,
    /* [in] */ CLSID __RPC_FAR *pclsidHook,
    /* [in] */ IUnknown __RPC_FAR *pHook,
    /* [in] */ DWORD grfOptions,
    /* [in] */ REFIID iid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDelegatorFactory_CreateDelegator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorFactory_INTERFACE_DEFINED__ */


#ifndef __IAlternateCredentialDelegatorHook_INTERFACE_DEFINED__
#define __IAlternateCredentialDelegatorHook_INTERFACE_DEFINED__

/* interface IAlternateCredentialDelegatorHook */
/* [local][object][uuid] */ 

typedef 
enum _ACDH_OPTIONS
    {	ACDH_AUTHNSVC	= 0x1,
	ACDH_AUTHZSVC	= 0x2,
	ACDH_SERVERPRINCIPAL	= 0x4,
	ACDH_AUTHNLEVEL	= 0x8,
	ACDH_IMPLEVEL	= 0x10,
	ACDH_CREDENTIALS	= 0x20,
	ACDH_CAPS	= 0x40
    }	ACDH_OPTIONS;


EXTERN_C const IID IID_IAlternateCredentialDelegatorHook;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("9b8c32f8-249a-11d2-a7bb-006008d25ccf")
    IAlternateCredentialDelegatorHook : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE Init( 
            /* [in] */ DWORD grfOptions,
            /* [in] */ DWORD nAuthnSvc,
            /* [in] */ DWORD nAuthzSvc,
            /* [in] */ const OLECHAR __RPC_FAR *pszServerPrincipal,
            /* [in] */ DWORD nAuthnLevel,
            /* [in] */ DWORD nImpLevel,
            /* [in] */ const OLECHAR __RPC_FAR *pszAuthority,
            /* [in] */ const OLECHAR __RPC_FAR *pszPrincipal,
            /* [in] */ const OLECHAR __RPC_FAR *pszPassword,
            /* [in] */ DWORD grfCaps) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IAlternateCredentialDelegatorHookVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IAlternateCredentialDelegatorHook __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IAlternateCredentialDelegatorHook __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IAlternateCredentialDelegatorHook __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Init )( 
            IAlternateCredentialDelegatorHook __RPC_FAR * This,
            /* [in] */ DWORD grfOptions,
            /* [in] */ DWORD nAuthnSvc,
            /* [in] */ DWORD nAuthzSvc,
            /* [in] */ const OLECHAR __RPC_FAR *pszServerPrincipal,
            /* [in] */ DWORD nAuthnLevel,
            /* [in] */ DWORD nImpLevel,
            /* [in] */ const OLECHAR __RPC_FAR *pszAuthority,
            /* [in] */ const OLECHAR __RPC_FAR *pszPrincipal,
            /* [in] */ const OLECHAR __RPC_FAR *pszPassword,
            /* [in] */ DWORD grfCaps);
        
        END_INTERFACE
    } IAlternateCredentialDelegatorHookVtbl;

    interface IAlternateCredentialDelegatorHook
    {
        CONST_VTBL struct IAlternateCredentialDelegatorHookVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IAlternateCredentialDelegatorHook_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IAlternateCredentialDelegatorHook_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IAlternateCredentialDelegatorHook_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IAlternateCredentialDelegatorHook_Init(This,grfOptions,nAuthnSvc,nAuthzSvc,pszServerPrincipal,nAuthnLevel,nImpLevel,pszAuthority,pszPrincipal,pszPassword,grfCaps)	\
    (This)->lpVtbl -> Init(This,grfOptions,nAuthnSvc,nAuthzSvc,pszServerPrincipal,nAuthnLevel,nImpLevel,pszAuthority,pszPrincipal,pszPassword,grfCaps)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IAlternateCredentialDelegatorHook_Init_Proxy( 
    IAlternateCredentialDelegatorHook __RPC_FAR * This,
    /* [in] */ DWORD grfOptions,
    /* [in] */ DWORD nAuthnSvc,
    /* [in] */ DWORD nAuthzSvc,
    /* [in] */ const OLECHAR __RPC_FAR *pszServerPrincipal,
    /* [in] */ DWORD nAuthnLevel,
    /* [in] */ DWORD nImpLevel,
    /* [in] */ const OLECHAR __RPC_FAR *pszAuthority,
    /* [in] */ const OLECHAR __RPC_FAR *pszPrincipal,
    /* [in] */ const OLECHAR __RPC_FAR *pszPassword,
    /* [in] */ DWORD grfCaps);


void __RPC_STUB IAlternateCredentialDelegatorHook_Init_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IAlternateCredentialDelegatorHook_INTERFACE_DEFINED__ */


#ifndef __IDelegatorFactory0_INTERFACE_DEFINED__
#define __IDelegatorFactory0_INTERFACE_DEFINED__

/* interface IDelegatorFactory0 */
/* [hidden][restricted][local][object][uuid] */ 


EXTERN_C const IID IID_IDelegatorFactory0;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("CE8555A1-977B-11d0-B33A-00400530E4E8")
    IDelegatorFactory0 : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE CreateDelegator( 
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ IUnknown __RPC_FAR *pUnkInner,
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDelegatorFactory0Vtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDelegatorFactory0 __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDelegatorFactory0 __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDelegatorFactory0 __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *CreateDelegator )( 
            IDelegatorFactory0 __RPC_FAR * This,
            /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
            /* [in] */ IUnknown __RPC_FAR *pUnkInner,
            /* [in] */ REFIID iid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);
        
        END_INTERFACE
    } IDelegatorFactory0Vtbl;

    interface IDelegatorFactory0
    {
        CONST_VTBL struct IDelegatorFactory0Vtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDelegatorFactory0_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDelegatorFactory0_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDelegatorFactory0_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDelegatorFactory0_CreateDelegator(This,pUnkOuter,pUnkInner,iid,ppv)	\
    (This)->lpVtbl -> CreateDelegator(This,pUnkOuter,pUnkInner,iid,ppv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDelegatorFactory0_CreateDelegator_Proxy( 
    IDelegatorFactory0 __RPC_FAR * This,
    /* [in] */ IUnknown __RPC_FAR *pUnkOuter,
    /* [in] */ IUnknown __RPC_FAR *pUnkInner,
    /* [in] */ REFIID iid,
    /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDelegatorFactory0_CreateDelegator_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDelegatorFactory0_INTERFACE_DEFINED__ */



#ifndef __DelegatorLib_LIBRARY_DEFINED__
#define __DelegatorLib_LIBRARY_DEFINED__

/* library DelegatorLib */
/* [lcid][version][uuid] */ 


EXTERN_C const IID LIBID_DelegatorLib;

EXTERN_C const CLSID CLSID_CoDelegator21;

#ifdef __cplusplus

class DECLSPEC_UUID("5f8bea31-60ce-11d2-a7ea-006008d25ccf")
CoDelegator21;
#endif

EXTERN_C const CLSID CLSID_CoDelegator20;

#ifdef __cplusplus

class DECLSPEC_UUID("9b8c32f0-249a-11d2-a7bb-006008d25ccf")
CoDelegator20;
#endif

EXTERN_C const CLSID CLSID_CoDelegator10;

#ifdef __cplusplus

class DECLSPEC_UUID("CE8555A0-977B-11d0-B33A-00400530E4E8")
CoDelegator10;
#endif

EXTERN_C const CLSID CLSID_CoDelegatorGetCallContext;

#ifdef __cplusplus

class DECLSPEC_UUID("bd8f05b1-9145-11d2-a802-006008d25ccf")
CoDelegatorGetCallContext;
#endif

EXTERN_C const CLSID CLSID_CoAnonymousDelegatorHook;

#ifdef __cplusplus

class DECLSPEC_UUID("9b8c32f6-249a-11d2-a7bb-006008d25ccf")
CoAnonymousDelegatorHook;
#endif

EXTERN_C const CLSID CLSID_CoAlternateCredentialDelegatorHook;

#ifdef __cplusplus

class DECLSPEC_UUID("9b8c32f7-249a-11d2-a7bb-006008d25ccf")
CoAlternateCredentialDelegatorHook;
#endif
#endif /* __DelegatorLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
