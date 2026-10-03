#ifndef __SPYMODULE_H
#define __SPYMODULE_H


#include "delegator.h"
#include <vector>
#include <stack>

#include "thk_Win9X.h"
#include "ItfTypeInfo.h"
#include "resource.h"

#include <list>

#include "ctlspysrv.h"

#include "debugger.h"


class CoDelegator;

// this is where the template class gets its pointer to the GIT
extern IGlobalInterfaceTable *g_pGIT;

//Whenever an interface is addrefed or released we will fire one of these to
//the debugger.
struct RefcountChange
    {
    DWORD dwCookie;  //Really a CoDelegator *
    IID iid;         //iid that's changing
    ULONG oldCount;  //old count
    ULONG newCount;  //new count
    };

//Messages we will fire. We are going to do as much of this asynchronously as possible.
#define WM_OBJECTADDED (WM_APP+1)     //We'll send the cookie 
#define WM_OBJECTDELETED (WM_APP+2)   //We'll send the cookie
#define WM_REFCOUNTCHANGED (WM_APP+3) //We'll send a RefcountChange

//options for wrapping identities.  IDENTITY_COCLASS means we own this object; we 
//intercepted CoCreateInstance and control its lifetime.  IDENTITY_WEAK means we
//are wrapping this object ex post facto, and only are tracking limited use on it.
//IDENTITY_PROXY means we have reason to believe that this object is a proxy (meaning
//it supports IProxyManager).



/////////////////////////////////////////////////////////////////////////////
// CModuleAccessor
// ----------
//Whenever we connect to the debugger manager we give it a CModuleAccessorObject to
//play with.  It will hold this object pretty much forever.  An alternate way to
//handle this would be with a named event.  We could have a thread that pends on
//a named event, and when the debugger wants us it will ping the event, at which
//point we could marshal an interface pointer manually then hand it over.
class CModuleAccessor : public ISpyAccessor,
                        public IDebuggerPrivate
{
public:

long m_dwRef;

CModuleAccessor()
{
ATLTRACE("Module accessor created\n");
m_dwRef = 0;
//_Module.Lock();
}

~CModuleAccessor()
{
ATLTRACE("Module accessor destroyed\n");
//_Module.Unlock();
}

STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
    {
    *ppv = NULL;
    if((riid == IID_ISpyAccessor) || (riid == IID_IUnknown))
        *ppv = static_cast<ISpyAccessor*>(this);
    else if(riid ==  IID_IDebuggerPrivate)
        *ppv = static_cast<IDebuggerPrivate*>(this);

    if(*ppv != NULL)
        AddRef(); 

    return(*ppv == NULL)? E_NOINTERFACE : S_OK;
    }
STDMETHODIMP_(ULONG) AddRef() {ATLTRACE("Module Accessor++(%d)\n",m_dwRef+1); return m_dwRef++;}
STDMETHODIMP_(ULONG) Release(){ATLTRACE("Module Accessor--(%d)\n",m_dwRef-1); return m_dwRef--;}

STDMETHODIMP CModuleAccessor::GetAllObjects(/*in*/long * pCount, /*out, size_is(,pCount)*/ struct IDENTITY_INFO ** ppObjs);
STDMETHODIMP CModuleAccessor::AccessObject(/*in*/long Cookie, /*in*/REFIID riid, /*out,iid_is(riid)*/IUnknown ** ppAccessor);
STDMETHODIMP CModuleAccessor::Advise(/*in*/long ObjCookie, /*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie);
STDMETHODIMP CModuleAccessor::Unadvise(/*in*/long Cookie);
STDMETHODIMP CModuleAccessor::QueryAttachDebugger(REFIID riid, IUnknown ** ppv);
};

//////////////////////////////////////////////////////////
//CSpyModule
//----------
//CSpyModule is where almost all of the action is in this program

//Once a spy object is running the user is likely going to want to inspect it.  We don't want
//to just bring up a window in-proc, because the reason for debugging is likely to be that the
//process is dying.  So we need a way for debugger processes to get at this module object.  Obviously
//we'd like to do this via com interfaces, but there is no COM way to broadcast.  What I will 
//pretty much have to do is register a named object and start a thread that waits on it.  Debugger
//processes can pulse the event and if any processes are being debugged they can respond by calling
//CoCreateInstance to connect to the debugger process.  

//I would like to use a window, but an MTA thread has no business creating a window, and I don't want
//to cut myself off from the MTA entirely, at least not if I can help it. So I think I'll just have
//to create the thread and do the event bit.  If nothing else what the thread can do is connect to the
//debugger, send over an interface pointer, and stick the reply in the GIT, then store the cookie in 
//a member var.  If any thread wants to talk to the debugger they'll have to first extract the GIT cookie.

//Whenever the module wraps an object it will grab the object's IUnknown and stick it in the GIT. Each C++
//CSpy object will be assigned this GIT cookie as an internal identifier.  This does not help us, however,
//when we come up with some pointer from God knows where and what to know if we have the object wrapped 
//already.  Actually there may not be too much we can do here. 

//So I've got this threading class CAsyncMessagePump, but it relies on CMessageMap.  This is problematic,
//because the header file that defines CMessageMap (atlwin.h) assumes that _Module has been defined.  Thus, it
//is going to be impossible to have CComModule derive from CMessageMap. Perhaps I can cheat and redefine CMessageMap
//here.  Should work, since I doubt I'll use any of the ATLWIN stuff in this program.
class ATL_NO_VTABLE CMessageMap
{
public:
	virtual BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam,
		LRESULT& lResult, DWORD dwMsgMapID) = 0;
};


#define BEGIN_MSG_MAP(theClass) \
public: \
	BOOL ProcessWindowMessage(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, LRESULT& lResult, DWORD dwMsgMapID = 0) \
	{ \
		BOOL bHandled = TRUE; \
		hWnd; \
		uMsg; \
		wParam; \
		lParam; \
		lResult; \
		bHandled; \
		switch(dwMsgMapID) \
		{ \
		case 0:

#define ALT_MSG_MAP(msgMapID) \
		break; \
		case msgMapID:

#define MESSAGE_HANDLER(msg, func) \
	if(uMsg == msg) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define MESSAGE_RANGE_HANDLER(msgFirst, msgLast, func) \
	if(uMsg >= msgFirst && uMsg <= msgLast) \
	{ \
		bHandled = TRUE; \
		lResult = func(uMsg, wParam, lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define COMMAND_HANDLER(id, code, func) \
	if(uMsg == WM_COMMAND && id == LOWORD(wParam) && code == HIWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define COMMAND_ID_HANDLER(id, func) \
	if(uMsg == WM_COMMAND && id == LOWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define COMMAND_CODE_HANDLER(code, func) \
	if(uMsg == WM_COMMAND && code == HIWORD(wParam)) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define COMMAND_RANGE_HANDLER(idFirst, idLast, func) \
	if(uMsg == WM_COMMAND && LOWORD(wParam) >= idFirst  && LOWORD(wParam) <= idLast) \
	{ \
		bHandled = TRUE; \
		lResult = func(HIWORD(wParam), LOWORD(wParam), (HWND)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define NOTIFY_HANDLER(id, cd, func) \
	if(uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom && cd == ((LPNMHDR)lParam)->code) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define NOTIFY_ID_HANDLER(id, func) \
	if(uMsg == WM_NOTIFY && id == ((LPNMHDR)lParam)->idFrom) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define NOTIFY_CODE_HANDLER(cd, func) \
	if(uMsg == WM_NOTIFY && cd == ((LPNMHDR)lParam)->code) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define NOTIFY_RANGE_HANDLER(idFirst, idLast, func) \
	if(uMsg == WM_NOTIFY && ((LPNMHDR)lParam)->idFrom >= idFirst && ((LPNMHDR)lParam)->idFrom <= idLast) \
	{ \
		bHandled = TRUE; \
		lResult = func((int)wParam, (LPNMHDR)lParam, bHandled); \
		if(bHandled) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP(theChainClass) \
	{ \
		if(theChainClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_MEMBER(theChainMember) \
	{ \
		if(theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_ALT(theChainClass, msgMapID) \
	{ \
		if(theChainClass::ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_ALT_MEMBER(theChainMember, msgMapID) \
	{ \
		if(theChainMember.ProcessWindowMessage(hWnd, uMsg, wParam, lParam, lResult, msgMapID)) \
			return TRUE; \
	}

#define CHAIN_MSG_MAP_DYNAMIC(dynaChainID) \
	{ \
		if(CDynamicChain::CallChain(dynaChainID, hWnd, uMsg, wParam, lParam, lResult)) \
			return TRUE; \
	}

#define END_MSG_MAP() \
			break; \
		default: \
			ATLTRACE2(atlTraceWindowing, 0, _T("Invalid message map ID (%i)\n"), dwMsgMapID); \
			ATLASSERT(FALSE); \
			break; \
		} \
		return FALSE; \
	}

#include "atlthread.h"

typedef std::list<CoDelegator*> IdentityList;
typedef std::list<CoDelegator*>::iterator IdentityIterator;

class CSpyModule : public CComModule, public CMessageMap
    {                  
    //We start up this secondary thread to communicate with the outside world.
    //CAsyncMessagePump m_WorkerThread;
    /*IDebugger * m_pDebugger;*/                        DWORD m_dwDebugger;

    //We only allow one client to attach right now.  Keeps the semantics (and the implementation) simple.
    /*IObjectEvents           * m_pEventClient;*/       DWORD m_dwEventClient;
    IObjectBreakpointEvents * m_pBreakpointClient;
    CModuleAccessor         m_Accessor;
   
    friend class CModuleAccessor;

    //TODO: If the CRT is not linked in then this guy's constructor won't get called.
    IdentityList m_Identities;


    //We need a critical section for most methods that access our data structures.
    CComCriticalSection m_cs;
    struct CModuleLock
    {
    CModuleLock();
    ~CModuleLock();
    };
    friend struct CModuleLock;
    

 public:
    HRESULT Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid = NULL);
    void Term();

    HRESULT RegisterServer(BOOL bRegTypeLib = FALSE, const CLSID * pCLSID = NULL)
        { return CComModule::UpdateRegistryFromResource(IDR_SPY, TRUE);  }

    HRESULT UnregisterServer(BOOL bRegTypeLib = FALSE, const CLSID * pCLSID = NULL)
        { return CComModule::UpdateRegistryFromResource(IDR_SPY, FALSE); }


    LONG Lock();
    LONG Unlock();

    //We override GetClassObject to do the registry special effects.
    HRESULT GetClassObject(REFCLSID rclsid, REFIID riid, LPVOID *ppv);

    STDMETHODIMP WrapObject(IUnknown * pUnkOuter, IUnknown * pObjRaw, long OptionFlags,
                            LPOLESTR bstrName, REFIID riid, void ** ppObjWrapped);

    //These two functions do standard pre- and post- processing.
    HRESULT Preprocess(CoDelegator * pDelegator, REFIID riid, long nVtblIndex, void * pArgs, void ** ppHookData);
    HRESULT PostProcess(CoDelegator * pDelegator, REFIID riid, long nVtblIndex, HRESULT hrFromInner, void * pHookData);


    //These two functions manage the list of identities.  It does not do AddRef/Release on the passed object because
    //that would create reference counting cycles.  CoDelegator will call RemoveIdentity in it's destructor.
    HRESULT AddIdentity(CoDelegator * p, DWORD dwOptions);
    HRESULT RemoveIdentity(CoDelegator * p);

    void OnQI(CoDelegator * p, REFIID riid, ULONG newCount, HRESULT hr);
    
    //This function allows us to send notifications of addref/release events
    void OnRefcountChanged(CoDelegator * p, REFIID riid, ULONG oldCount, ULONG newCount);

    //These manage the delegator->cookie mapping.  Pretty simple right now... :)  
    //TODO:Error checking in case someone passes us an invalid cookie.
    CoDelegator * FindDelegator(DWORD dwCookie){return reinterpret_cast<CoDelegator*>(dwCookie);}
    DWORD GetCookie(CoDelegator * pd){return reinterpret_cast<DWORD>(pd);}

    long GetIdentityCount(){return m_Identities.size();}
    void EnumIdentities(IdentityIterator& begin, IdentityIterator& end)
    {begin = m_Identities.begin(); end = m_Identities.end();}

    HRESULT GetIdentityInfo(long Cookie, struct IDENTITY_INFO * pInfo);
    HRESULT GetIdentitySnapshot(long * pCount,  struct IDENTITY_INFO ** ppObjs);


//7/24/99= playing around with this thing gave quite a shock that I should have thought of.
//If the thread does not pump messages then there is _no_way_ for the outside word to get
//in.  This is another indication that we should do something other than COM when we send
//notifications.  Shared memory would be fast and wouldn't have any limitations, but it's
//so hard ;).  Still, I think that's the only real way to get this to work.
    HRESULT Advise(/*in*/long ObjCookie, /*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie);
    HRESULT Unadvise(long Cookie);

    void SetupDebuggerConnection();
    void TeardownDebuggerConnection();

    /////////////////////////////////////////////////////////////////////////////////////////
    //Everything below this line is done in the context of the worker thread
    /////////////////////////////////////////////////////////////////////////////////////////

    BEGIN_MSG_MAP(CSpyModule)
        // Handler prototypes:
        //  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
        //  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
        //  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);
       MESSAGE_HANDLER(WM_OBJECTADDED,     Fire_ObjectAdded)
       MESSAGE_HANDLER(WM_OBJECTDELETED,   Fire_ObjectDeleted)
       MESSAGE_HANDLER(WM_REFCOUNTCHANGED, Fire_RefcountChanged)
    END_MSG_MAP()

    LRESULT Fire_ObjectAdded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT Fire_ObjectDeleted(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    LRESULT Fire_RefcountChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
    };


#endif







