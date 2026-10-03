/////////////////////////////////////////////////////////////
// SpyModule.cpp - COMSpy
//
// Copyright 2000, Jason Whittington
//
//COMSpy overrides the ATL _Module object to do more
//work.  That code is present in class CSpyModule.  
/////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "spymodule.h"

#include <atlcom.h>
#include <docobj.h>
//#include "spy.h"

#include "classfactory.h"
#include "CoDelegator.h"

#include "dharma.h"

//The one and only GIT pointer
IGlobalInterfaceTable *g_pGIT;


//This simple class just locks and unlocks the module.
CSpyModule::CModuleLock::CModuleLock(){_Module.m_cs.Lock();}
CSpyModule::CModuleLock::~CModuleLock(){_Module.m_cs.Unlock();}


///////////////////////////////////////////////////////////////////////////////
//The following code comes from Dharma Shukla's web page.  Thanks, Dharma!
#define x86TEB 0x18        

#pragma warning (disable:4035) //warning C4035: : no return value blah blah 


DWORD CoHackGetTLSValue(void) { __asm mov eax, fs:[x86TEB] 
                                       __asm mov eax, [eax+0x0F80]
                                       __asm mov eax, [eax+0x0C] }
#pragma warning (default:4035) 


COHACK_APTTYPE CoHackGetAptType()
{
    DWORD dw = CoHackGetTLSValue();
    return (dw & 0x800) ? COHACK_APTTYPE_TNA : (dw & 0x80) ? COHACK_APTTYPE_STA :COHACK_APTTYPE_MTA;
}


char* GetApartmentType()
{
switch(CoHackGetAptType())
    {
    case COHACK_APTTYPE_TNA: return "TNA";
    case COHACK_APTTYPE_STA: return "STA";
    case COHACK_APTTYPE_MTA: return "MNA";
    }
return "???";
}
///////////////////////////////////////////////////////////////////////////////



BOOL GetInterfaceName(REFIID iid, LPTSTR szName, long Size)
    {
    //CComBSTR bstrIfaceName;
    USES_CONVERSION;

    szName[0] = 0;

    //convert the iid to a string...
    LPOLESTR pszGUID = NULL;
    StringFromCLSID(iid, &pszGUID);
    DWORD dwType;

    // Attempt to find the iface in the interfaces section...
    CRegKey key;

    key.Open(HKEY_CLASSES_ROOT, _T("Interface"), KEY_READ);
    if (key.Open(key, OLE2T(pszGUID), KEY_READ) == S_OK)
        {
	    *szName = 0;
	    RegQueryValueEx(key.m_hKey, (LPTSTR)NULL, NULL, &dwType, (LPBYTE)szName, (LPDWORD)&Size);
        //bstrIfaceName = szName;
	    //CoTaskMemFree(pszGUID);
	    }

    //give up and just give back a stringized GUID.
    if(szName[0] == 0)
        {
        ::strncpy(szName, W2T(pszGUID),Size-1);
        }

    CoTaskMemFree(pszGUID);

    return 0;
    }



/////////////////////////////////////////////////////////////////////////////
// CIdentityAccessor
// -----------------
//When client processes want to talk to one of the debugged objects they'll have
//to come through here.  I don't want the debugger to have easy access to the objects
//themselves.  This is a perfect example of a cookie-driven object, incidentally.
//
//The object that we're watching might vanish.
class CIdentityAccessor : public CComObjectRootEx<CComMultiThreadModel>,
                          public IDebuggedObject
    {
    long m_Cookie;
    public:
    CIdentityAccessor()
    {
    ATLTRACE("Identity accessor created\n");
    }

    ~CIdentityAccessor()
    {
    ATLTRACE("Identity accessor destroyed\n");
    }


    BEGIN_COM_MAP(CIdentityAccessor)
      COM_INTERFACE_ENTRY(IDebuggedObject)
    END_COM_MAP()

    HRESULT Init(long dwCookie)
       {
       m_Cookie = dwCookie; return S_OK;
       }


    STDMETHODIMP get_Info(struct IDENTITY_INFO *pif)
       {
       return _Module.GetIdentityInfo(m_Cookie,pif);
       }
    STDMETHODIMP GetInterfaceCounts(long dwOptions, long *pCount, struct REFCOUNT ** pRC)
       {
       CoDelegator * pDelegator = _Module.FindDelegator(m_Cookie);
       if(pDelegator == NULL) return E_FAIL;

       return pDelegator->Probe(dwOptions, pCount, pRC);
       }
    };


/////////////////////////////////////////////////////////////////////////////
// CModuleAccessor
// ---------------
STDMETHODIMP CModuleAccessor::GetAllObjects(/*in*/long * pCount, /*out, size_is(,pCount)*/ struct IDENTITY_INFO ** ppObjs)
    {
    if(pCount == NULL) return E_POINTER;
    if(ppObjs == NULL) return E_POINTER;

    return _Module.GetIdentitySnapshot(pCount, ppObjs);
    }

STDMETHODIMP CModuleAccessor::AccessObject(/*in*/long Cookie, /*in*/REFIID riid, /*out,iid_is(riid)*/IUnknown ** ppAccessor)
    {
    //For now I'll use CComObject, which will lock our module.  This might mitigate some of the 
    //VB unpleasantness.
    
    CComObject<CIdentityAccessor> * pAccessor = new CComObject<CIdentityAccessor>();
    pAccessor->Init(Cookie);
    HRESULT hr = pAccessor->QueryInterface(riid, (void**)ppAccessor);
    if(FAILED(hr)) delete pAccessor;
    return hr;
    }

STDMETHODIMP CModuleAccessor::Advise(/*in*/long ObjCookie, /*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie)
    {
    return _Module.Advise(ObjCookie, riid, pSink, pCookie);
    }

STDMETHODIMP CModuleAccessor::Unadvise(/*in*/long Cookie)
    {
    return _Module.Unadvise(Cookie);
    }

//IDebuggerPrivate - It occurs to me that we could accomplish the same thing with IServiceProvider.  For 
//now we treat this like queryinterface, but we make no guarantees about the COM identity that comes back
//from here.
STDMETHODIMP CModuleAccessor::QueryAttachDebugger(REFIID riid, IUnknown ** ppv)
    {
    return QueryInterface(riid, (void**)ppv);
    }




/////////////////////////////////////////////////////////////////////////////
// CSpyModule
// ----------
HRESULT CSpyModule::Init(_ATL_OBJMAP_ENTRY* p, HINSTANCE h, const GUID* plibid)
    {
    //m_pDebugger = NULL;
    //inherited from Keith's code
	if ( !CoDelegator::Startup() )
		return E_FAIL;
    thk_Win9X::Startup();
	ItfTypeInfo::Startup();

        
    //m_pEventClient = 0;
    m_pBreakpointClient = 0;
    m_dwDebugger        = 0;
    m_dwEventClient     = 0;

    m_cs.Init();

    HRESULT hr = CoCreateInstance(CLSID_StdGlobalInterfaceTable, 0, CLSCTX_ALL,
                          IID_IGlobalInterfaceTable, (void**)&g_pGIT);

    if(SUCCEEDED(hr))
        hr = CComModule::Init(p,h,plibid);

    return hr;
    }

void CSpyModule::Term()
    {
    TeardownDebuggerConnection();

    g_pGIT->RevokeInterfaceFromGlobal(m_dwDebugger);
    g_pGIT->RevokeInterfaceFromGlobal(m_dwEventClient);

    ATLTRACE("SpyModule::Term()...");
    CComModule::Term();

    ItfTypeInfo::Shutdown();
    thk_Win9X::Shutdown();
    CoDelegator::Shutdown();
    m_cs.Term();
    ATLTRACE("Ok.\n");
    }



LONG CSpyModule::Lock()
    {
    CModuleLock lock();

    CComModule::Lock();
    LONG l = CComModule::GetLockCount();
    ATLTRACE("Module Lock(%d)\n", GetLockCount());
    return l;
    }

LONG CSpyModule::Unlock()
    {
    //We think we're going away.  What happens with VB, however, is that our refcount goes
    //to zero,the DLL is unloaded, then promptly reloaded.  But this is our last call before DLLMain PROCESS_DETACH, 
    //and we can't release interface pointers there.  So we're going to _have_ to play along with
    //the DLLCanUnloadNow semantics, and dutifully shut down and start back up whenever we transition
    //from run mode back into design mode.  The debugger manager should try and cope with this.

    //So it will be safe for _us_ to hold interface pointers on the _manager_, but not vice versa.  This
    //suggests that we should use something else (like events + shared memory) to allow the manager to
    //"get its foot in the door" and obtain an interface pointer into the process.  For now, however,
    //I will just CoDisconnectObject whenever we shut down.  This is very rude, and shared memory could
    //help ensure continuity because I can do that at DllMain time. I think I'm going to have to do it that
    //way.  It's the only way I can represent continuity in this process.

    //My first thought here was to cruft up an accessor and table marshal it, then give the packet to the
    //debugger, who could unmarshal it at will.  This seems like it could work as long as the debugger process
    //agrees to free the marshaled data whenever we go away.  This is a big win, because we don't have to 
    //tell the debugger when we go away.

    //The debugger already monitors our process.  If it detects that we're gone that it can trash the marshal packet.
    //If we register and it thinks we're alive then it trashes the packet and takes the new one.  If it tries to 
    //unmarshal and it fails, then it trashes the packet.  If it detects that the process died, then it trashes the
    //packet.  So in all cases the marshal data is trashed appropriately.
    
    LONG l = CComModule::Unlock();
    if (l == 0)
        {
        CModuleLock lock();
        ATLTRACE("Module shutting down.\n");
        ATLASSERT(m_Identities.size() == 0);
        //m_WorkerThread.Stop();

        if(m_Accessor.m_dwRef > 0)
            {
            //screw our clients...
            ::CoDisconnectObject(static_cast<ISpyAccessor*>(&m_Accessor),0);
            }

        //We do NOT release the event pointers here, because it's quite possible that we're just
        //going from run-mode in VB back into design time mode.  So we either do not release this
        //stuff, or we're going to need a way to tell the debugger "I'm back" and hope it remembers.
        //Very ugly, and all because we can't do COM in DllMain
        
        //Actually it's worse than that. VB kicks us OUT of the process and then loads us back in 
        //when it makes the transition.  The user of the spy doesn't want to be exposed to this
        //though...

        ATLTRACE("Releasing event pointers...");
        if(m_dwEventClient != 0){g_pGIT->RevokeInterfaceFromGlobal(m_dwEventClient); m_dwEventClient=0; }
        //if(m_pBreakpointClient != NULL){m_pBreakpointClient->Release(); m_pBreakpointClient = 0;}
        ATLTRACE("Ok.\n");
        if(m_dwDebugger != NULL)
            {
            //Unadvise the debugger of our pointer...
            g_pGIT->RevokeInterfaceFromGlobal(m_dwDebugger);
            m_dwDebugger = 0;
            //m_pDebugger->Release();
            //m_pDebugger = NULL;
            }
        ATLTRACE("Module is shut down\n");

        //Kill the GIT
        g_pGIT->Release();
        }

    ATLTRACE("Module Unlock(%d)\n", GetLockCount());

    return l;
    }


void CSpyModule::SetupDebuggerConnection()
    {
    //We start the worker thread so that we can fire notifications asynchronously.  We will do this with
    //refcount notifications as well as method notifications.  If the user sets breakpoints we will do
    //things synchronously instead, so that the outside world can come in and mess with the object while
    //we pend on the result.
    //m_WorkerThread.Start(this, 1, COINIT_MULTITHREADED);

    //TODO: Contact the debugger and give it an object on _this_ thread.  Take the pointer
    //to the debugger and put it in the GIT.  Pass a message to the other thread to grab
    //the cookie and extract the pointer.

    //From now on we will fire events to the debugger from the other thread, so that it 
    //can happen asynchronously.  The only exception will be breakpoints, which we will handle
    //synchronously, so that the debugger can call back into the process while we wait.  (Will
    //this actually _WORK_ ?)

    CComPtr<IDebugger> p;
    p.CoCreateInstance(L"Debugger.Debugger.1", NULL, CLSCTX_LOCAL_SERVER);

    if(p)
       {
       //Stick our pointer in the GIT...
       HRESULT hr = g_pGIT->RegisterInterfaceInGlobal(p, IID_IDebugger, &m_dwDebugger);
       ATLASSERT(SUCCEEDED(hr));

       USES_CONVERSION;
    
       TCHAR szModule[1024];
       ::GetModuleFileName(GetModuleHandle(NULL), szModule, 1024);
   
       BSTR bstrProcessName = T2BSTR(szModule);
       
       //This pointer has been GITized
       //m_pDebugger = p.Detach();  //we'll hold onto this pointer, but we'll have to let it go
       //                           //when our module count goes to zero.

       IDebuggerPrivate * pUnk;
       m_Accessor.QueryInterface(IID_IDebuggerPrivate,(void**)&pUnk);
       
       //Now we marshal up a pointer to give to the debugger manager.  
       IStream * pStm;

       HGLOBAL h = ::GlobalAlloc(GPTR | GMEM_SHARE,0);
       hr = ::CreateStreamOnHGlobal(h,TRUE, &pStm);
       if(SUCCEEDED(hr))
            {
            //If I marshal this as TABLESTRONG then someone unmarshals and releases, the stub will die and
            //further unmarshals will fail.  On the other hand, if I marshal tablestrong, then the packet itself
            //represents an addref (and thus a lock).  Should we get forcibly unloaded the stub will be 
            //stranded.  I suppose that's why we do CoDisconnectObject.
            hr = ::CoMarshalInterface(pStm, IID_IDebuggerPrivate, pUnk, MSHCTX_LOCAL, NULL,MSHLFLAGS_TABLESTRONG);

            STATSTG stat;
            hr = pStm->Stat(&stat,STATFLAG_NONAME);
            ATLASSERT(SUCCEEDED(hr));

            if(SUCCEEDED(hr))
                {
                p->AdviseProcess(bstrProcessName, ::GetCurrentProcessId(), IID_IDebuggerPrivate, pStm);
                }
            pStm->Release();
            }


       pUnk->Release();
       }

    }


void CSpyModule::TeardownDebuggerConnection()
    {
    //NOTE: COUninitialize has probably already been called here!!!!
    }

STDMETHODIMP CSpyModule::WrapObject(IUnknown * pUnkOuter, IUnknown * pObjRaw, long OptionFlags,
                        LPOLESTR lpszName, REFIID riid, void ** ppObjWrapped)
    {
    CModuleLock lock;

    //first add the object
    CoDelegator * pDelegator = new CoDelegator(pUnkOuter, pObjRaw, 0, 0, lpszName);

    //Once we give it to the delegator it will addref the inner, too.  So we can release
    //our reference to the raw object.
    pObjRaw->Release();

    //Save this identity in our list of wrapped identities.
    AddIdentity(pDelegator, OptionFlags);

    //We call InternalQI to avoid firing any events just yet.  We want to
    //fire the ObjectAdded event before we fire the QI event.
    ULONG count=0;
    HRESULT hr = pDelegator->InternalQueryInterface(riid, ppObjWrapped,&count);

    if(m_dwEventClient != NULL)
        {
        CComPtr<IObjectEvents> pEventClient;
        if(SUCCEEDED(g_pGIT->GetInterfaceFromGlobal(m_dwEventClient, IID_IObjectEvents, (void**)&pEventClient)))
            {
            IDENTITY_INFO info;
            info.dwCookie = this->GetCookie(pDelegator);
            info.dwFlags = OptionFlags;
            info.bstrFriendlyName = ::SysAllocString(pDelegator->GetName());
            pEventClient->ObjCreated(&info);
            pEventClient->ObjQI(info.dwCookie, riid, count, hr);
            }
        }
    return hr;
    }


//GetClassObject fakes up a class factory that will ultimately return an object with the
//desired CLSID, but wrapped with the spy.
HRESULT CSpyModule::GetClassObject(REFCLSID rclsid, REFIID riid, void ** ppv)
    {
    CModuleLock lock;
    HRESULT hr = REGDB_E_CLASSNOTREG;

    //Now we start up the debugger manager if we haven't already. We do it from here rather
    //than init in case the calling process loaded us to do registration or something.  Once
    //the access DllGetClassObject we know they're serious.
    if(m_dwDebugger == 0)
        {
        SetupDebuggerConnection();
        }


    //First concoct a string to the right registry key
    LPOLESTR lpszGuid;
    TCHAR szRegKey[96];
    ::StringFromCLSID(rclsid, &lpszGuid);
    sprintf(szRegKey,"CLSID\\%S\\InProcServer32",lpszGuid);
    ::CoTaskMemFree(lpszGuid);

    //now open that key...
    HKEY hkPath = NULL;
    if(::RegOpenKey(HKEY_CLASSES_ROOT, szRegKey, &hkPath) == ERROR_SUCCESS)
        {      
        //...and extract the path                   
        TCHAR szActualPath[1024]; DWORD dwType; DWORD dwSize=1024;
        if(::RegQueryValueEx(hkPath, _T("HookSavedPath"), 0, &dwType,(LPBYTE)szActualPath, &dwSize) == ERROR_SUCCESS) 
            {
            //create a class factory which will create a wrapped object.
            CComObjectNoLock<CSpyClassFactory>* pCF = new CComObjectNoLock<CSpyClassFactory>;
            pCF->Init(rclsid, szActualPath);

            hr = pCF->QueryInterface(riid, ppv);
            if(FAILED(hr))
                delete pCF;
            }
        
        ::RegCloseKey(hkPath);
        }

    if((FAILED(hr)) && m_dwDebugger != 0)
        {
        g_pGIT->RevokeInterfaceFromGlobal(m_dwDebugger);
        m_dwDebugger = 0;
        //m_pDebugger->Release();
        //m_pDebugger = NULL;
        }
      
    return hr;
    }

HRESULT CSpyModule::Preprocess(CoDelegator * pDelegator, REFIID riid, long nVtblIndex, void * pArgs, void ** ppHookData)
{
return S_OK;
}

HRESULT CSpyModule::PostProcess(CoDelegator * pDelegator, REFIID riid, long nVtblIndex, HRESULT hrFromInner, void * pHookData)
{
CModuleLock lock;

//ATLTRACE("Activity on object %x\n", pDelegator);
if(m_dwEventClient != 0)
    {
    //If we try to call out during an input_sync call we get this.  Not much we can do in
    //a situation like this -- it just shows up that we'll have to use some other method (like
    //shared mem) to throw these calls.
    //RPC_E_CANTCALLOUT_INEXTERNALCALL (0x80010005L)

    CComPtr<IObjectEvents> pEventClient;
    if(SUCCEEDED(g_pGIT->GetInterfaceFromGlobal(m_dwEventClient, IID_IObjectEvents, (void**)&pEventClient)))
        {

        //TODO: m_pEventClient needs to come from the GIT
        HRESULT hr = pEventClient->ObjMethodCalled(GetCookie(pDelegator), riid, nVtblIndex, 0, hrFromInner);
        if(FAILED(hr))
           switch(hr)
            {
            case RPC_E_CANTCALLOUT_INEXTERNALCALL:
                {
                char szMsg[256];
                char szIface[64];
                ::GetInterfaceName(riid, szIface, 64);
                sprintf(szMsg,"Warning: Got RPC_E_CANTCALLOUT_INEXTERNALCALL on call %s::%d\n", szIface, nVtblIndex);
                ATLTRACE(szMsg);
                break;
                }
            default: 
                {
                g_pGIT->RevokeInterfaceFromGlobal(m_dwEventClient);
                m_dwEventClient = 0;
                }
            }
        }
    }
return hrFromInner;
}

HRESULT CSpyModule::AddIdentity(CoDelegator * p, DWORD dwOptions)
{
CModuleLock lock;

p->Init(0, dwOptions);

m_Identities.push_back(p);

//TODO: notify everybody that we added this.
return S_OK;
}

HRESULT CSpyModule::RemoveIdentity(CoDelegator * p)
{
//TODO: If the GIT cookie is non-null then revoke it.
CModuleLock lock;


if(this->m_dwEventClient != NULL)
    {
    CComPtr<IObjectEvents> pEventClient;
    if(SUCCEEDED(g_pGIT->GetInterfaceFromGlobal(m_dwEventClient, IID_IObjectEvents, (void**)&pEventClient)))
        {
        pEventClient->ObjDestroyed(GetCookie(p));
        }
    }


m_Identities.remove(p);


return S_OK;
}

void CSpyModule::OnQI(CoDelegator * p, REFIID riid, ULONG newCount, HRESULT hrFromQI)
{

CModuleLock lock;

//m_WorkerThread.PostMessage(WM_REFCOUNTCHANGED,0,0);
if(this->m_dwEventClient != 0)
    {
    CComPtr<IObjectEvents> pEventClient;
    if(SUCCEEDED(g_pGIT->GetInterfaceFromGlobal(m_dwEventClient, IID_IObjectEvents, (void**)&pEventClient)))
        {
        HRESULT hr =  pEventClient->ObjQI(GetCookie(p), riid, newCount, hrFromQI);
        if(FAILED(hr)) 
            {
            //TODO: ???
            //::CoDisconnectObject(m_pEventClient,0);
            //m_pEventClient = 0;
            g_pGIT->RevokeInterfaceFromGlobal(m_dwEventClient);
            m_dwEventClient = 0;
            }
        }
    }

}


void CSpyModule::OnRefcountChanged(CoDelegator * p, REFIID riid, ULONG oldCount, ULONG newCount)
{

CModuleLock lock;

if(this->m_dwEventClient != 0)
    {
    
    CComPtr<IObjectEvents> pEventClient;
    if(SUCCEEDED(g_pGIT->GetInterfaceFromGlobal(m_dwEventClient, IID_IObjectEvents, (void**)&pEventClient)))
        {
        HRESULT hr =  pEventClient->ObjRefCountChanged(GetCookie(p), riid, oldCount, newCount);
        if(FAILED(hr)) 
            {
            //TODO: ???
            //::CoDisconnectObject(m_pEventClient,0);
            g_pGIT->RevokeInterfaceFromGlobal(m_dwEventClient);
            m_dwEventClient = 0;
            }
        }
    }

}


HRESULT CSpyModule::GetIdentityInfo(long Cookie, struct IDENTITY_INFO * pInfo)
    {
    CModuleLock lock;

    CoDelegator * pD = FindDelegator(Cookie);
    ATLASSERT(pD != NULL);
    
    pInfo->dwCookie = Cookie;
    pInfo->dwFlags = pD->GetIdentityFlags();
    pInfo->bstrFriendlyName = ::SysAllocString(pD->GetName());

    return S_OK;
    }

HRESULT CSpyModule::GetIdentitySnapshot(long * pCount,  struct IDENTITY_INFO ** ppObjs)
    {
    //TODO: lock so that noone modifies the data structure while we're sucking the info out.
    CModuleLock lock;

    ATLTRACE("Getting identity snapshot:...");
    *pCount = m_Identities.size();
    *ppObjs = (IDENTITY_INFO*)::CoTaskMemAlloc(*pCount * sizeof(IDENTITY_INFO));

    long i=0;
    for(IdentityIterator it = m_Identities.begin(); it != m_Identities.end(); it++, i++)
        {
        GetIdentityInfo((long)(*it), (*ppObjs)+i);
        }

    ATLTRACE("Identity snapshot ok:...");
    return S_OK;
    }


HRESULT CSpyModule::Advise(/*in*/long ObjCookie, /*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie)
    {
    CModuleLock lock;

    HRESULT hr = CONNECT_E_ADVISELIMIT;
    *pCookie = 0;
    
    if(riid == IID_IObjectEvents)
        {
        if(m_dwEventClient == 0)
            {
            g_pGIT->RegisterInterfaceInGlobal(pSink,riid,&m_dwEventClient);
            *pCookie = m_dwEventClient;
            pSink->AddRef();
            hr = S_OK;
            }
        }
     /*
     else if(riid == IID_IObjectBreakpointEvents)
        {
        if(m_pBreakpointClient == 0)
            {
            m_pBreakpointClient = (IObjectBreakpointEvents*)pSink;
            *pCookie = (long)&m_pBreakpointClient;
            pSink->AddRef();
            hr = S_OK;
            }
        }
     */
    return hr;
    }


HRESULT CSpyModule::Unadvise(long Cookie)
    {
    CModuleLock lock;

    HRESULT hr = CONNECT_E_NOCONNECTION;
    if(Cookie == (long)&m_dwEventClient)
        if(m_dwEventClient != 0)
            {
            g_pGIT->RevokeInterfaceFromGlobal(m_dwEventClient);
            m_dwEventClient = 0;
            hr = S_OK;
            }
    //else if(Cookie == (long)&m_pBreakpointClient)
    //    if(m_pBreakpointClient != 0)
    //        {
    //        m_pBreakpointClient->Release();
    //        m_pEventClient = 0;
    //        hr = S_OK;
    //        }
    return hr;
    }


/////////////////////////////////////////////////////////////////////////////////////////
//Everything below this line is done in the context of the worker thread
/////////////////////////////////////////////////////////////////////////////////////////

LRESULT CSpyModule::Fire_ObjectAdded(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
    //TODO: Fire an event to the debugger
    return 0;
    }

LRESULT CSpyModule::Fire_ObjectDeleted(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
    //TODO: Fire an event to the debugger
    return 0;
    }

LRESULT CSpyModule::Fire_RefcountChanged(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
    //TODO: Fire an event to the debugger
    return 0;
    }
