/* this ALWAYS GENERATED file contains the definitions for the interfaces */


/* File created by MIDL compiler version 5.01.0164 */
/* at Sat Jul 24 12:41:57 1999
 */
/* Compiler settings for C:\projects\ctlspy2\debugger\debugger.idl:
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

#ifndef __debugger_h__
#define __debugger_h__

#ifdef __cplusplus
extern "C"{
#endif 

/* Forward Declarations */ 

#ifndef __IDebugger_FWD_DEFINED__
#define __IDebugger_FWD_DEFINED__
typedef interface IDebugger IDebugger;
#endif 	/* __IDebugger_FWD_DEFINED__ */


#ifndef __IDebuggerEvents_FWD_DEFINED__
#define __IDebuggerEvents_FWD_DEFINED__
typedef interface IDebuggerEvents IDebuggerEvents;
#endif 	/* __IDebuggerEvents_FWD_DEFINED__ */


#ifndef __IClientAccessor_FWD_DEFINED__
#define __IClientAccessor_FWD_DEFINED__
typedef interface IClientAccessor IClientAccessor;
#endif 	/* __IClientAccessor_FWD_DEFINED__ */


#ifndef __Debugger_FWD_DEFINED__
#define __Debugger_FWD_DEFINED__

#ifdef __cplusplus
typedef class Debugger Debugger;
#else
typedef struct Debugger Debugger;
#endif /* __cplusplus */

#endif 	/* __Debugger_FWD_DEFINED__ */


#ifndef __ClientAccessor_FWD_DEFINED__
#define __ClientAccessor_FWD_DEFINED__

#ifdef __cplusplus
typedef class ClientAccessor ClientAccessor;
#else
typedef struct ClientAccessor ClientAccessor;
#endif /* __cplusplus */

#endif 	/* __ClientAccessor_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"

void __RPC_FAR * __RPC_USER MIDL_user_allocate(size_t);
void __RPC_USER MIDL_user_free( void __RPC_FAR * ); 

/* interface __MIDL_itf_debugger_0000 */
/* [local] */ 

struct  PROCESS_INFO
    {
    long dwCookie;
    BSTR bstrProcessName;
    };


extern RPC_IF_HANDLE __MIDL_itf_debugger_0000_v0_0_c_ifspec;
extern RPC_IF_HANDLE __MIDL_itf_debugger_0000_v0_0_s_ifspec;

#ifndef __IDebugger_INTERFACE_DEFINED__
#define __IDebugger_INTERFACE_DEFINED__

/* interface IDebugger */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebugger;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3F209DC0-3314-11D3-8BF2-00105A6DC077")
    IDebugger : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE AdviseProcess( 
            /* [in] */ BSTR bstrProcessName,
            /* [in] */ long ProcessId,
            /* [in] */ REFIID iidPkt,
            /* [in] */ IStream __RPC_FAR *pMarshaledPacket) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebuggerVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDebugger __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDebugger __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDebugger __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AdviseProcess )( 
            IDebugger __RPC_FAR * This,
            /* [in] */ BSTR bstrProcessName,
            /* [in] */ long ProcessId,
            /* [in] */ REFIID iidPkt,
            /* [in] */ IStream __RPC_FAR *pMarshaledPacket);
        
        END_INTERFACE
    } IDebuggerVtbl;

    interface IDebugger
    {
        CONST_VTBL struct IDebuggerVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebugger_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebugger_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebugger_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebugger_AdviseProcess(This,bstrProcessName,ProcessId,iidPkt,pMarshaledPacket)	\
    (This)->lpVtbl -> AdviseProcess(This,bstrProcessName,ProcessId,iidPkt,pMarshaledPacket)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebugger_AdviseProcess_Proxy( 
    IDebugger __RPC_FAR * This,
    /* [in] */ BSTR bstrProcessName,
    /* [in] */ long ProcessId,
    /* [in] */ REFIID iidPkt,
    /* [in] */ IStream __RPC_FAR *pMarshaledPacket);


void __RPC_STUB IDebugger_AdviseProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebugger_INTERFACE_DEFINED__ */


#ifndef __IDebuggerEvents_INTERFACE_DEFINED__
#define __IDebuggerEvents_INTERFACE_DEFINED__

/* interface IDebuggerEvents */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IDebuggerEvents;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3F209DC1-3314-11D3-8BF2-00105A6DC077")
    IDebuggerEvents : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE OnProcessAdded( 
            /* [in] */ struct PROCESS_INFO __RPC_FAR *pinfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE OnProcessClosed( 
            /* [in] */ struct PROCESS_INFO __RPC_FAR *pinfo) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IDebuggerEventsVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IDebuggerEvents __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IDebuggerEvents __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IDebuggerEvents __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnProcessAdded )( 
            IDebuggerEvents __RPC_FAR * This,
            /* [in] */ struct PROCESS_INFO __RPC_FAR *pinfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *OnProcessClosed )( 
            IDebuggerEvents __RPC_FAR * This,
            /* [in] */ struct PROCESS_INFO __RPC_FAR *pinfo);
        
        END_INTERFACE
    } IDebuggerEventsVtbl;

    interface IDebuggerEvents
    {
        CONST_VTBL struct IDebuggerEventsVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IDebuggerEvents_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IDebuggerEvents_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IDebuggerEvents_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IDebuggerEvents_OnProcessAdded(This,pinfo)	\
    (This)->lpVtbl -> OnProcessAdded(This,pinfo)

#define IDebuggerEvents_OnProcessClosed(This,pinfo)	\
    (This)->lpVtbl -> OnProcessClosed(This,pinfo)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IDebuggerEvents_OnProcessAdded_Proxy( 
    IDebuggerEvents __RPC_FAR * This,
    /* [in] */ struct PROCESS_INFO __RPC_FAR *pinfo);


void __RPC_STUB IDebuggerEvents_OnProcessAdded_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IDebuggerEvents_OnProcessClosed_Proxy( 
    IDebuggerEvents __RPC_FAR * This,
    /* [in] */ struct PROCESS_INFO __RPC_FAR *pinfo);


void __RPC_STUB IDebuggerEvents_OnProcessClosed_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IDebuggerEvents_INTERFACE_DEFINED__ */


#ifndef __IClientAccessor_INTERFACE_DEFINED__
#define __IClientAccessor_INTERFACE_DEFINED__

/* interface IClientAccessor */
/* [unique][helpstring][uuid][object] */ 


EXTERN_C const IID IID_IClientAccessor;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("3F209DC2-3314-11D3-8BF2-00105A6DC077")
    IClientAccessor : public IUnknown
    {
    public:
        virtual HRESULT STDMETHODCALLTYPE GetProcessList( 
            /* [out] */ long __RPC_FAR *pCount,
            /* [size_is][size_is][out] */ struct PROCESS_INFO __RPC_FAR *__RPC_FAR *ppInfo) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE AttachToProcess( 
            long ProcessId,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppProcess) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Advise( 
            /* [in] */ REFIID riid,
            /* [iid_is][in] */ IUnknown __RPC_FAR *pSink,
            /* [out] */ long __RPC_FAR *pCookie) = 0;
        
        virtual HRESULT STDMETHODCALLTYPE Unadvise( 
            /* [in] */ long Cookie) = 0;
        
    };
    
#else 	/* C style interface */

    typedef struct IClientAccessorVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *QueryInterface )( 
            IClientAccessor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ void __RPC_FAR *__RPC_FAR *ppvObject);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *AddRef )( 
            IClientAccessor __RPC_FAR * This);
        
        ULONG ( STDMETHODCALLTYPE __RPC_FAR *Release )( 
            IClientAccessor __RPC_FAR * This);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *GetProcessList )( 
            IClientAccessor __RPC_FAR * This,
            /* [out] */ long __RPC_FAR *pCount,
            /* [size_is][size_is][out] */ struct PROCESS_INFO __RPC_FAR *__RPC_FAR *ppInfo);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *AttachToProcess )( 
            IClientAccessor __RPC_FAR * This,
            long ProcessId,
            /* [in] */ REFIID riid,
            /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppProcess);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Advise )( 
            IClientAccessor __RPC_FAR * This,
            /* [in] */ REFIID riid,
            /* [iid_is][in] */ IUnknown __RPC_FAR *pSink,
            /* [out] */ long __RPC_FAR *pCookie);
        
        HRESULT ( STDMETHODCALLTYPE __RPC_FAR *Unadvise )( 
            IClientAccessor __RPC_FAR * This,
            /* [in] */ long Cookie);
        
        END_INTERFACE
    } IClientAccessorVtbl;

    interface IClientAccessor
    {
        CONST_VTBL struct IClientAccessorVtbl __RPC_FAR *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IClientAccessor_QueryInterface(This,riid,ppvObject)	\
    (This)->lpVtbl -> QueryInterface(This,riid,ppvObject)

#define IClientAccessor_AddRef(This)	\
    (This)->lpVtbl -> AddRef(This)

#define IClientAccessor_Release(This)	\
    (This)->lpVtbl -> Release(This)


#define IClientAccessor_GetProcessList(This,pCount,ppInfo)	\
    (This)->lpVtbl -> GetProcessList(This,pCount,ppInfo)

#define IClientAccessor_AttachToProcess(This,ProcessId,riid,ppProcess)	\
    (This)->lpVtbl -> AttachToProcess(This,ProcessId,riid,ppProcess)

#define IClientAccessor_Advise(This,riid,pSink,pCookie)	\
    (This)->lpVtbl -> Advise(This,riid,pSink,pCookie)

#define IClientAccessor_Unadvise(This,Cookie)	\
    (This)->lpVtbl -> Unadvise(This,Cookie)

#endif /* COBJMACROS */


#endif 	/* C style interface */



HRESULT STDMETHODCALLTYPE IClientAccessor_GetProcessList_Proxy( 
    IClientAccessor __RPC_FAR * This,
    /* [out] */ long __RPC_FAR *pCount,
    /* [size_is][size_is][out] */ struct PROCESS_INFO __RPC_FAR *__RPC_FAR *ppInfo);


void __RPC_STUB IClientAccessor_GetProcessList_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IClientAccessor_AttachToProcess_Proxy( 
    IClientAccessor __RPC_FAR * This,
    long ProcessId,
    /* [in] */ REFIID riid,
    /* [iid_is][out] */ IUnknown __RPC_FAR *__RPC_FAR *ppProcess);


void __RPC_STUB IClientAccessor_AttachToProcess_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IClientAccessor_Advise_Proxy( 
    IClientAccessor __RPC_FAR * This,
    /* [in] */ REFIID riid,
    /* [iid_is][in] */ IUnknown __RPC_FAR *pSink,
    /* [out] */ long __RPC_FAR *pCookie);


void __RPC_STUB IClientAccessor_Advise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);


HRESULT STDMETHODCALLTYPE IClientAccessor_Unadvise_Proxy( 
    IClientAccessor __RPC_FAR * This,
    /* [in] */ long Cookie);


void __RPC_STUB IClientAccessor_Unadvise_Stub(
    IRpcStubBuffer *This,
    IRpcChannelBuffer *_pRpcChannelBuffer,
    PRPC_MESSAGE _pRpcMessage,
    DWORD *_pdwStubPhase);



#endif 	/* __IClientAccessor_INTERFACE_DEFINED__ */



#ifndef __DEBUGGERLib_LIBRARY_DEFINED__
#define __DEBUGGERLib_LIBRARY_DEFINED__

/* library DEBUGGERLib */
/* [helpstring][version][uuid] */ 


EXTERN_C const IID LIBID_DEBUGGERLib;

EXTERN_C const CLSID CLSID_Debugger;

#ifdef __cplusplus

class DECLSPEC_UUID("58C849A0-3314-11D3-8BF2-00105A6DC077")
Debugger;
#endif

EXTERN_C const CLSID CLSID_ClientAccessor;

#ifdef __cplusplus

class DECLSPEC_UUID("664E10F0-3F2B-11D3-8BFA-00105A6DC077")
ClientAccessor;
#endif
#endif /* __DEBUGGERLib_LIBRARY_DEFINED__ */

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
