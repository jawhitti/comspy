/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Thu Jan 20 15:25:04 2000
 */
/* Compiler settings for C:\projects\comspy\ctlspysrv\ctlspysrv.idl:
    Oicf (OptLev=i2), W1, Zp8, env=Win32, ms_ext, c_ext
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

#ifndef __ctlspysrv_h__
#define __ctlspysrv_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDebuggerPrivate_FWD_DEFINED__
#define __IDebuggerPrivate_FWD_DEFINED__
typedef interface IDebuggerPrivate IDebuggerPrivate;
#endif 	/* __IDebuggerPrivate_FWD_DEFINED__ */


#ifndef __IObjectEvents_FWD_DEFINED__
#define __IObjectEvents_FWD_DEFINED__
typedef interface IObjectEvents IObjectEvents;
#endif 	/* __IObjectEvents_FWD_DEFINED__ */


#ifndef __IObjectBreakpointEvents_FWD_DEFINED__
#define __IObjectBreakpointEvents_FWD_DEFINED__
typedef interface IObjectBreakpointEvents IObjectBreakpointEvents;
#endif 	/* __IObjectBreakpointEvents_FWD_DEFINED__ */


#ifndef __IDebuggedObject_FWD_DEFINED__
#define __IDebuggedObject_FWD_DEFINED__
typedef interface IDebuggedObject IDebuggedObject;
#endif 	/* __IDebuggedObject_FWD_DEFINED__ */


#ifndef __ISpyAccessor_FWD_DEFINED__
#define __ISpyAccessor_FWD_DEFINED__
typedef interface ISpyAccessor ISpyAccessor;
#endif 	/* __ISpyAccessor_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_ctlspysrv_0000 */
/* [local] */ 

struct  IDENTITY_INFO
    {
    long dwCookie;
    long dwFlags;
    BSTR bstrFriendlyName;
    };
struct  REFCOUNT
    {
    IID iid;
    ULONG rfcount;
    };
typedef 
enum tagIDENTITY_FLAGS
    {	IDENTITY_COCLASS	= 1,
	IDENTITY_WEAK	= 2,
	IDENTITY_PROXY	= 4
    }	IDENTITY_FLAGS;



extern RPC_IF_HANDLE __MIDL_itf_ctlspysrv_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_ctlspysrv_0000_v0_0_s_ifspec;

#ifndef __IDebuggerPrivate_INTERFACE_DEFINED__
#define __IDebuggerPrivate_INTERFACE_DEFINED__

/* interface IDebuggerPrivate */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebuggerPrivate;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F10764D6-342A-11D3-8BF3-00105A6DC077")
    IDebuggerPrivate : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE QueryAttachDebugger( 
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebuggerPrivateVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDebuggerPrivate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDebuggerPrivate __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDebuggerPrivate __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryAttachDebugger )( 
            IDebuggerPrivate __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv);
        
        END_INTERFACE
    } IDebuggerPrivateVtbl;

    interface IDebuggerPrivate
    {
        CONST_VTBL struct IDebuggerPrivateVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebuggerPrivate_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebuggerPrivate_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebuggerPrivate_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebuggerPrivate_QueryAttachDebugger(This,riid,ppv)	\
    (This)->lpVtbl -> QueryAttachDebugger(This,riid,ppv)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebuggerPrivate_QueryAttachDebugger_Proxy( 
    IDebuggerPrivate __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppv);


void __RPC_STUB IDebuggerPrivate_QueryAttachDebugger_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebuggerPrivate_INTERFACE_DEFINED__ */


#ifndef __IObjectEvents_INTERFACE_DEFINED__
#define __IObjectEvents_INTERFACE_DEFINED__

/* interface IObjectEvents */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjectEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F10764D5-342A-11D3-8BF3-00105A6DC077")
    IObjectEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ObjCreated( 
            /* [in] */ struct IDENTITY_INFO __RPC_FAR *pif) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ObjDestroyed( 
            /* [in] */ long Cookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ObjQI( 
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ ULONG newCount,
            /* [in] */ HRESULT hrQI) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ObjRefCountChanged( 
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ ULONG oldCount,
            /* [in] */ ULONG newCount) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ObjMethodCalled( 
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ long vtblIndex,
            /* [in] */ long reserved,
            /* [in] */ HRESULT hr) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjectEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IObjectEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IObjectEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IObjectEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjCreated )( 
            IObjectEvents __RPC_FAR * This,
            /* [in] */ struct IDENTITY_INFO __RPC_FAR *pif);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjDestroyed )( 
            IObjectEvents __RPC_FAR * This,
            /* [in] */ long Cookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjQI )( 
            IObjectEvents __RPC_FAR * This,
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ ULONG newCount,
            /* [in] */ HRESULT hrQI);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjRefCountChanged )( 
            IObjectEvents __RPC_FAR * This,
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ ULONG oldCount,
            /* [in] */ ULONG newCount);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjMethodCalled )( 
            IObjectEvents __RPC_FAR * This,
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ long vtblIndex,
            /* [in] */ long reserved,
            /* [in] */ HRESULT hr);
        
        END_INTERFACE
    } IObjectEventsVtbl;

    interface IObjectEvents
    {
        CONST_VTBL struct IObjectEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IObjectEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IObjectEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IObjectEvents_ObjCreated(This,pif)	\
    (This)->lpVtbl -> ObjCreated(This,pif)

#define IObjectEvents_ObjDestroyed(This,Cookie)	\
    (This)->lpVtbl -> ObjDestroyed(This,Cookie)

#define IObjectEvents_ObjQI(This,Cookie,riid,newCount,hrQI)	\
    (This)->lpVtbl -> ObjQI(This,Cookie,riid,newCount,hrQI)

#define IObjectEvents_ObjRefCountChanged(This,Cookie,riid,oldCount,newCount)	\
    (This)->lpVtbl -> ObjRefCountChanged(This,Cookie,riid,oldCount,newCount)

#define IObjectEvents_ObjMethodCalled(This,Cookie,riid,vtblIndex,reserved,hr)	\
    (This)->lpVtbl -> ObjMethodCalled(This,Cookie,riid,vtblIndex,reserved,hr)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IObjectEvents_ObjCreated_Proxy( 
    IObjectEvents __RPC_FAR * This,
    /* [in] */ struct IDENTITY_INFO __RPC_FAR *pif);


void __RPC_STUB IObjectEvents_ObjCreated_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IObjectEvents_ObjDestroyed_Proxy( 
    IObjectEvents __RPC_FAR * This,
    /* [in] */ long Cookie);


void __RPC_STUB IObjectEvents_ObjDestroyed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IObjectEvents_ObjQI_Proxy( 
    IObjectEvents __RPC_FAR * This,
    /* [in] */ long Cookie,
    /* [in] */ REFIID riid,
    /* [in] */ ULONG newCount,
    /* [in] */ HRESULT hrQI);


void __RPC_STUB IObjectEvents_ObjQI_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IObjectEvents_ObjRefCountChanged_Proxy( 
    IObjectEvents __RPC_FAR * This,
    /* [in] */ long Cookie,
    /* [in] */ REFIID riid,
    /* [in] */ ULONG oldCount,
    /* [in] */ ULONG newCount);


void __RPC_STUB IObjectEvents_ObjRefCountChanged_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IObjectEvents_ObjMethodCalled_Proxy( 
    IObjectEvents __RPC_FAR * This,
    /* [in] */ long Cookie,
    /* [in] */ REFIID riid,
    /* [in] */ long vtblIndex,
    /* [in] */ long reserved,
    /* [in] */ HRESULT hr);


void __RPC_STUB IObjectEvents_ObjMethodCalled_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IObjectEvents_INTERFACE_DEFINED__ */


#ifndef __IObjectBreakpointEvents_INTERFACE_DEFINED__
#define __IObjectBreakpointEvents_INTERFACE_DEFINED__

/* interface IObjectBreakpointEvents */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IObjectBreakpointEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F10764D4-342A-11D3-8BF3-00105A6DC077")
    IObjectBreakpointEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE ObjPreBreakpoint( 
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ long vtblIndex) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE ObjPostBreakpoint( 
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ long vtblIndex,
            /* [out][in] */ HRESULT __RPC_FAR *phrFromInner) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IObjectBreakpointEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IObjectBreakpointEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IObjectBreakpointEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IObjectBreakpointEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjPreBreakpoint )( 
            IObjectBreakpointEvents __RPC_FAR * This,
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ long vtblIndex);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *ObjPostBreakpoint )( 
            IObjectBreakpointEvents __RPC_FAR * This,
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [in] */ long vtblIndex,
            /* [out][in] */ HRESULT __RPC_FAR *phrFromInner);
        
        END_INTERFACE
    } IObjectBreakpointEventsVtbl;

    interface IObjectBreakpointEvents
    {
        CONST_VTBL struct IObjectBreakpointEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IObjectBreakpointEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IObjectBreakpointEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IObjectBreakpointEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IObjectBreakpointEvents_ObjPreBreakpoint(This,Cookie,riid,vtblIndex)	\
    (This)->lpVtbl -> ObjPreBreakpoint(This,Cookie,riid,vtblIndex)

#define IObjectBreakpointEvents_ObjPostBreakpoint(This,Cookie,riid,vtblIndex,phrFromInner)	\
    (This)->lpVtbl -> ObjPostBreakpoint(This,Cookie,riid,vtblIndex,phrFromInner)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IObjectBreakpointEvents_ObjPreBreakpoint_Proxy( 
    IObjectBreakpointEvents __RPC_FAR * This,
    /* [in] */ long Cookie,
    /* [in] */ REFIID riid,
    /* [in] */ long vtblIndex);


void __RPC_STUB IObjectBreakpointEvents_ObjPreBreakpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IObjectBreakpointEvents_ObjPostBreakpoint_Proxy( 
    IObjectBreakpointEvents __RPC_FAR * This,
    /* [in] */ long Cookie,
    /* [in] */ REFIID riid,
    /* [in] */ long vtblIndex,
    /* [out][in] */ HRESULT __RPC_FAR *phrFromInner);


void __RPC_STUB IObjectBreakpointEvents_ObjPostBreakpoint_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IObjectBreakpointEvents_INTERFACE_DEFINED__ */


#ifndef __IDebuggedObject_INTERFACE_DEFINED__
#define __IDebuggedObject_INTERFACE_DEFINED__

/* interface IDebuggedObject */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_IDebuggedObject;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F10764D3-342A-11D3-8BF3-00105A6DC077")
    IDebuggedObject : public IUnknown
    {
    public:
        virtual /* [propget] */ HRESULT STDMETHODCALLTYPE get_Info( 
            /* [retval][out] */ struct IDENTITY_INFO __RPC_FAR *pif) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE GetInterfaceCounts( 
            /* [in] */ long dwOptions,
            /* [out] */ long __RPC_FAR *pCount,
            /* [size_is][size_is][out] */ struct REFCOUNT __RPC_FAR *__RPC_FAR *pRC) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebuggedObjectVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDebuggedObject __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDebuggedObject __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDebuggedObject __RPC_FAR * This);
        
        /* [propget] */ HRESULT ( STDMETHODCALLTYPE __RPC_FAR *get_Info )( 
            IDebuggedObject __RPC_FAR * This,
            /* [retval][out] */ struct IDENTITY_INFO __RPC_FAR *pif);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetInterfaceCounts )( 
            IDebuggedObject __RPC_FAR * This,
            /* [in] */ long dwOptions,
            /* [out] */ long __RPC_FAR *pCount,
            /* [size_is][size_is][out] */ struct REFCOUNT __RPC_FAR *__RPC_FAR *pRC);
        
        END_INTERFACE
    } IDebuggedObjectVtbl;

    interface IDebuggedObject
    {
        CONST_VTBL struct IDebuggedObjectVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebuggedObject_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebuggedObject_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebuggedObject_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebuggedObject_get_Info(This,pif)	\
    (This)->lpVtbl -> get_Info(This,pif)

#define IDebuggedObject_GetInterfaceCounts(This,dwOptions,pCount,pRC)	\
    (This)->lpVtbl -> GetInterfaceCounts(This,dwOptions,pCount,pRC)

#endif /* COBJMACROS */


#endif 	/* C style interface */



/* [propget] */ HRESULT STDMETHODCALLTYPE IDebuggedObject_get_Info_Proxy( 
    IDebuggedObject __RPC_FAR * This,
    /* [retval][out] */ struct IDENTITY_INFO __RPC_FAR *pif);


void __RPC_STUB IDebuggedObject_get_Info_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebuggedObject_GetInterfaceCounts_Proxy( 
    IDebuggedObject __RPC_FAR * This,
    /* [in] */ long dwOptions,
    /* [out] */ long __RPC_FAR *pCount,
    /* [size_is][size_is][out] */ struct REFCOUNT __RPC_FAR *__RPC_FAR *pRC);


void __RPC_STUB IDebuggedObject_GetInterfaceCounts_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebuggedObject_INTERFACE_DEFINED__ */


#ifndef __ISpyAccessor_INTERFACE_DEFINED__
#define __ISpyAccessor_INTERFACE_DEFINED__

/* interface ISpyAccessor */
/* [unique][uuid][object] */ 


EXTERN_C const IID IID_ISpyAccessor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("F10764D2-342A-11D3-8BF3-00105A6DC077")
    ISpyAccessor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetAllObjects( 
            /* [out] */ long __RPC_FAR *pCount,
            /* [size_is][size_is][out] */ struct IDENTITY_INFO __RPC_FAR *__RPC_FAR *pCookies) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AccessObject( 
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppAccessor) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ long ObjCookie,
            /* [in] */ REFIID riid,
            /* [iid_is][in] */ IUnknown __RPC_FAR *pSink,
            /* [out] */ long __RPC_FAR *pCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ long Cookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct ISpyAccessorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            ISpyAccessor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            ISpyAccessor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            ISpyAccessor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetAllObjects )( 
            ISpyAccessor __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pCount,
            /* [size_is][size_is][out] */ struct IDENTITY_INFO __RPC_FAR *__RPC_FAR *pCookies);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AccessObject )( 
            ISpyAccessor __RPC_FAR * This,
            /* [in] */ long Cookie,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppAccessor);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Advise )( 
            ISpyAccessor __RPC_FAR * This,
            /* [in] */ long ObjCookie,
            /* [in] */ REFIID riid,
            /* [iid_is][in] */ IUnknown __RPC_FAR *pSink,
            /* [out] */ long __RPC_FAR *pCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unadvise )( 
            ISpyAccessor __RPC_FAR * This,
            /* [in] */ long Cookie);
        
        END_INTERFACE
    } ISpyAccessorVtbl;

    interface ISpyAccessor
    {
        CONST_VTBL struct ISpyAccessorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ISpyAccessor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define ISpyAccessor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define ISpyAccessor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define ISpyAccessor_GetAllObjects(This,pCount,pCookies)	\
    (This)->lpVtbl -> GetAllObjects(This,pCount,pCookies)

#define ISpyAccessor_AccessObject(This,Cookie,riid,ppAccessor)	\
    (This)->lpVtbl -> AccessObject(This,Cookie,riid,ppAccessor)

#define ISpyAccessor_Advise(This,ObjCookie,riid,pSink,pCookie)	\
    (This)->lpVtbl -> Advise(This,ObjCookie,riid,pSink,pCookie)

#define ISpyAccessor_Unadvise(This,Cookie)	\
    (This)->lpVtbl -> Unadvise(This,Cookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE ISpyAccessor_GetAllObjects_Proxy( 
    ISpyAccessor __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pCount,
    /* [size_is][size_is][out] */ struct IDENTITY_INFO __RPC_FAR *__RPC_FAR *pCookies);


void __RPC_STUB ISpyAccessor_GetAllObjects_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpyAccessor_AccessObject_Proxy( 
    ISpyAccessor __RPC_FAR * This,
    /* [in] */ long Cookie,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppAccessor);


void __RPC_STUB ISpyAccessor_AccessObject_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpyAccessor_Advise_Proxy( 
    ISpyAccessor __RPC_FAR * This,
    /* [in] */ long ObjCookie,
    /* [in] */ REFIID riid,
    /* [iid_is][in] */ IUnknown __RPC_FAR *pSink,
    /* [out] */ long __RPC_FAR *pCookie);


void __RPC_STUB ISpyAccessor_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE ISpyAccessor_Unadvise_Proxy( 
    ISpyAccessor __RPC_FAR * This,
    /* [in] */ long Cookie);


void __RPC_STUB ISpyAccessor_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __ISpyAccessor_INTERFACE_DEFINED__ */


/* Additional Prototypes for ALL interfaces */

unsigned long             __RPC_USER  BSTR_UserSize(     unsigned long __RPC_FAR *, unsigned long            , BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserMarshal(  unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
unsigned char __RPC_FAR * __RPC_USER  BSTR_UserUnmarshal(unsigned long __RPC_FAR *, unsigned char __RPC_FAR *, BSTR __RPC_FAR * ); 
void                      __RPC_USER  BSTR_UserFree(     unsigned long __RPC_FAR *, BSTR __RPC_FAR * ); 

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif
