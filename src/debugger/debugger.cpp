// debugger.cpp : Implementation of WinMain


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f debuggerps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "debugger.h"
#include "debugdialog.h"

#include "debugger_i.c"
#include "DebuggerImpl.h"

#include "ClientAccessor.h"

#include "ctlspysrv.h"
#include "ctlspysrv_i.c"

#include <map>
#include <set>

const DWORD dwTimeOut = 5000; // time for EXE to be idle before shutting down
const DWORD dwPause = 1000; // time to wait for threads to finish up

static CDebugDialog g_dlg;

//We keep an array of "ProcessTracker" objects around.  We also peek at
//each process every few seconds and yank one if it disappears.  This way if
//a process disappears we can figure it out pretty quickly, and notify clients,
//who may take a lot longer to discover the fact (i.e. clients on remote machines).
struct ProcessTracker 
    {
    PROCESS_INFO info;
    //BSTR bstrImageName;
    //DWORD info.dwCookie;
    IStream * pMshStream;
    long displayidx;
    bool bOk;

    ProcessTracker(BSTR bstrName, DWORD dwPID, IStream * pStm)
        {
        bOk = true;
        static LARGE_INTEGER zero; zero.QuadPart = 0;
        info.bstrProcessName = ::SysAllocString(bstrName);
        info.dwCookie = dwPID;


        //Copy the marshal packet into our own stream
        HGLOBAL h = ::GlobalAlloc(0,GPTR);
        HRESULT hr = ::CreateStreamOnHGlobal(h, TRUE, &pMshStream);

        if(SUCCEEDED(hr))
            {
            STATSTG stat;
            hr = pStm->Stat(&stat,STATFLAG_NONAME);
            ATLASSERT(SUCCEEDED(hr));
           
            pMshStream->Seek(zero,0,0);
            pStm->Seek(zero,0,0);
            hr = pStm->CopyTo(pMshStream, stat.cbSize,NULL,NULL);
            ATLASSERT(SUCCEEDED(hr));
            }
        }
    ~ProcessTracker()
        {
        ::SysFreeString(info.bstrProcessName);
        info.dwCookie=0;
        ::CoReleaseMarshalData(pMshStream);
        pMshStream->Release();
        }
    };

typedef std::map<long, ProcessTracker*> ProcessMap;
ProcessMap g_Processes;

//This seems extremely silly, but after we delete an item out of a 
//set our iterators are invalid. This class helps with garbage collection.
struct client_holder
{
BOOL bOk;
IUnknown * pClient;

client_holder(IUnknown * pUnk){bOk = TRUE; pClient = pUnk;}
int operator==(IUnknown * pArg){return pClient == pArg;}
int operator==(client_holder arg){return pClient == arg.pClient;}
//The STL is so twisted.  Drop 'const' from the next line and the compiler
//is clueless (I mean the _second_ const).
bool operator<(const client_holder& arg) const {return (pClient < arg.pClient);}
};
std::set<client_holder> g_Clients;
typedef std::set<client_holder> ClientSet;


//The function deals with processes that appear to have died.
void OnDebugeeDied(ProcessMap::iterator& it, HRESULT hr)
    {
    Fire_OnProcessClosed(&(it->second->info));

    long pid = it->second->info.dwCookie;
    delete it->second;
    it->second = NULL;
    g_Processes.erase(pid);
    }


HRESULT AdviseProcess(BSTR bstrProcessName,long ProcessId, REFIID iidPkt, IStream * pMarshaledPacket)
    {
    ATLASSERT(iidPkt == IID_IDebuggerPrivate);

    
    ProcessMap::iterator it = g_Processes.find(ProcessId);
    if(it != g_Processes.end())
        {
        //Hmm... the process must've died and come back...
        OnDebugeeDied(it, E_FAIL);
        }
    ATLTRACE("Adding process %S\n",bstrProcessName);
    ProcessTracker *p = new ProcessTracker(bstrProcessName, ProcessId, pMarshaledPacket);
    p->displayidx =  g_dlg.AddItem(bstrProcessName);

    g_Processes[ProcessId] = p;

    Fire_OnProcessAdded(&(p->info));


    return S_OK;
    }


void ScanProcesses()
    {
    ATLTRACE("scanning process map...\n");
    ProcessMap::iterator it = g_Processes.begin();
    while(it != g_Processes.end())
        {
        ATLASSERT(it->second != NULL);
        ProcessMap::iterator next = it;
        next++;

        if(it->second != NULL)
            {
            HANDLE hProcess = NULL;
            if(it->second->bOk)
               hProcess = ::OpenProcess(PROCESS_QUERY_INFORMATION ,FALSE, it->second->info.dwCookie);

            if((hProcess == NULL) || (!(it->second->bOk)))
                {
                //This process disappeared without notifying us.  So we delete our reference to
                //it and tell clients that it's dead.
                //DWORD dw = GetLastError();
                ATLTRACE("removing stale process %d\n", it->second->info.dwCookie);
                ::OnDebugeeDied(it, E_FAIL);
                }

            if(hProcess != NULL) ::CloseHandle(hProcess);
            }
        it = next;
        }    
    //while we're at it we'll garbage collect the client array...
    ClientSet::iterator it2 = g_Clients.begin();
    while(it2 != g_Clients.end())
        {
        if(it2->bOk == FALSE)
            {
            ClientSet::iterator temp = it2;
            temp++;
            ClientUnadvise((long)(it2->pClient));
            it2 = temp;
            }
        else
            {
            it2++;
            }
        }

    if((g_Clients.size() == 0) && (g_Processes.size() == 0) && (_Module.GetLockCount() == 0))
        {
        g_dlg.BeginCountdown();
        ATLTRACE("Debugger:Attempting Shutdown\n");
        }
    }




HRESULT ClientAdvise(/*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie)
    {               
    _Module.Lock();

    if(riid != IID_IDebuggerEvents)
        return CONNECT_E_NOCONNECTION;

    *pCookie = (long)pSink;
    pSink->AddRef();
    client_holder ch(pSink);
    g_Clients.insert(ch);
    return S_OK;
    }

HRESULT ClientUnadvise(/*in*/long Cookie)
    {
    _Module.Unlock();

    ((IUnknown*)Cookie)->Release();
    g_Clients.erase((IUnknown*)Cookie);
    return S_OK;
    }


HRESULT GetProcessList(long * pCount, PROCESS_INFO ** ppInfo)
    {
    if(pCount == NULL) return E_POINTER;
    if(ppInfo == NULL) return E_POINTER;

    *pCount = g_Processes.size();
    ATLTRACE("Giving list of %d processes to client\n", *pCount);
    PROCESS_INFO * p = reinterpret_cast<PROCESS_INFO*>(::CoTaskMemAlloc(*pCount * sizeof(PROCESS_INFO)));

    long i=0;
    for(ProcessMap::iterator it = g_Processes.begin(); it != g_Processes.end(); it++, i++)
        {
        p[i].bstrProcessName = ::SysAllocString(it->second->info.bstrProcessName);
        p[i].dwCookie = it->second->info.dwCookie;
        if(i > 100)
           ATLASSERT(false);
        }

    *ppInfo = p;
    return S_OK;
    }

HRESULT AttachToProcess(long ProcessId, REFIID riid, IUnknown ** ppProcess)
    {
    LARGE_INTEGER zero; zero.QuadPart = 0;
 
    ProcessMap::iterator it = g_Processes.find(ProcessId);
    if(it == g_Processes.end()) return E_INVALIDARG;

    
    //We now attempt to unmarshal the pointer 
   IDebuggerPrivate * pModule=0;
   HRESULT hr;
    //for(int i=0; i<2; i++)
    //    {
        it->second->pMshStream->Seek(zero,0,0);
        hr = ::CoUnmarshalInterface(it->second->pMshStream, IID_IDebuggerPrivate, (void**)&pModule);
    //    }

        if(SUCCEEDED(hr))
            {
            hr = pModule->QueryAttachDebugger(riid, ppProcess);
            //now that we got what the debugee wanted we can trash the iface pointer...
            pModule->Release();
            }
        else
            {
            it->second->bOk = false;
            ATLTRACE("Unable to unmarshal iface ptr, error 0x%08X\n", hr);
            //OnDebugeeDied(it, hr);
            }
    return hr;
    }



// Passed to CreateThread to monitor the shutdown event
static DWORD WINAPI MonitorProc(void* pv)
{
    CExeModule* p = (CExeModule*)pv;
    p->MonitorShutdown();
    return 0;
}


HRESULT Fire_OnProcessAdded(PROCESS_INFO * pinfo)
    {
    ClientSet::iterator it;
 
    for(it = g_Clients.begin(); it != g_Clients.end(); it++)
        {
        if(it->bOk)
            {
            IDebuggerEvents* p = (IDebuggerEvents*)(it->pClient);
            if(FAILED(p->OnProcessAdded(pinfo)))
                it->bOk = FALSE;
            }
        }

    return 0;
    }
HRESULT Fire_OnProcessClosed(PROCESS_INFO * pinfo)
    {
    ClientSet::iterator it;
    
    for(it = g_Clients.begin(); it != g_Clients.end(); it++)
        {
        if(it->bOk)
            {
            IDebuggerEvents* p = (IDebuggerEvents*)(it->pClient);
            if(FAILED(p->OnProcessClosed(pinfo)))
                it->bOk = FALSE;
            }
        }

    return 0;
    };

LONG CExeModule::Lock()
{
ATLTRACE("Debugger::Lock(%d)\n", GetLockCount()+1);
return CComModule::Lock();
}

LONG CExeModule::Unlock()
{
    LONG l = CComModule::Unlock();
    

ATLTRACE("Debugger::Unlock(%d)\n", GetLockCount());

ScanProcesses();
    if(l == 0)
        {
        //Deciding exactly what to do here is tricky.  We don't just shut down because
        //certain processes (VB) will register/unregister (whenever it transitions from
        //design-time back into run-time).  So we set a timer and wait for, say, 10 seconds,
        //and if the count is still zero then we shut down (in AttemptShutdown)
        g_dlg.BeginCountdown();
        ATLTRACE("Debugger:Shutting down in 10 seconds\n");
        }

    //if (l == 0)
    //{
    //    bActivity = true;
    //    SetEvent(hEventShutdown); // tell monitor that we transitioned to zero
    //}
    return l;
}

void AttemptShutdown()
    {
    ScanProcesses();

    //This function will get called a few seconds after module count goes to zero.
    if((_Module.GetLockCount() == 0) && (g_Processes.size() == 0) && (g_Clients.size() == 0))
        PostQuitMessage(0);
    }


//Monitors the shutdown event
void CExeModule::MonitorShutdown()
{
    while (1)
    {
        WaitForSingleObject(hEventShutdown, INFINITE);
        DWORD dwWait=0;
        do
        {
            bActivity = false;
            dwWait = WaitForSingleObject(hEventShutdown, dwTimeOut);
        } while (dwWait == WAIT_OBJECT_0);
        // timed out
        if (!bActivity && m_nLockCnt == 0) // if no activity let's really bail
        {
#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
            CoSuspendClassObjects();
            if (!bActivity && m_nLockCnt == 0)
#endif
                break;
        }
    }
    CloseHandle(hEventShutdown);
    PostThreadMessage(dwThreadID, WM_QUIT, 0, 0);
}

bool CExeModule::StartMonitor()
{
    hEventShutdown = CreateEvent(NULL, false, false, NULL);
    if (hEventShutdown == NULL)
        return false;
    DWORD dwThreadID;
    HANDLE h = CreateThread(NULL, 0, MonitorProc, this, 0, &dwThreadID);
    return (h != NULL);
}

CExeModule _Module;

BEGIN_OBJECT_MAP(ObjectMap)
OBJECT_ENTRY(CLSID_Debugger, CDebugger)
OBJECT_ENTRY(CLSID_ClientAccessor, CClientAccessor)
END_OBJECT_MAP()


LPCTSTR FindOneOf(LPCTSTR p1, LPCTSTR p2)
{
    while (p1 != NULL && *p1 != NULL)
    {
        LPCTSTR p = p2;
        while (p != NULL && *p != NULL)
        {
            if (*p1 == *p)
                return CharNext(p1);
            p = CharNext(p);
        }
        p1 = CharNext(p1);
    }
    return NULL;
}

/////////////////////////////////////////////////////////////////////////////
//This application's job is to quietly manage resources on the local machine.
//The only GUI it usually displays is a taskbar icon.  Clicking it will show a
//dialog box that displays a list of currently debugged processes.
extern "C" int WINAPI _tWinMain(HINSTANCE hInstance, 
    HINSTANCE /*hPrevInstance*/, LPTSTR lpCmdLine, int /*nShowCmd*/)
{
    lpCmdLine = GetCommandLine(); //this line necessary for _ATL_MIN_CRT

//#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
//    HRESULT hRes = CoInitializeEx(NULL, COINIT_MULTITHREADED);
//#else
    HRESULT hRes = CoInitialize(NULL);
//#endif

    _ASSERTE(SUCCEEDED(hRes));
    _Module.Init(ObjectMap, hInstance, &LIBID_DEBUGGERLib);
    _Module.dwThreadID = GetCurrentThreadId();
    TCHAR szTokens[] = _T("-/");

    int nRet = 0;
    BOOL bRun = TRUE;
    LPCTSTR lpszToken = FindOneOf(lpCmdLine, szTokens);
    while (lpszToken != NULL)
    {
        if (lstrcmpi(lpszToken, _T("UnregServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_Debugger, FALSE);
            nRet = _Module.UnregisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        if (lstrcmpi(lpszToken, _T("RegServer"))==0)
        {
            _Module.UpdateRegistryFromResource(IDR_Debugger, TRUE);
            nRet = _Module.RegisterServer(TRUE);
            bRun = FALSE;
            break;
        }
        lpszToken = FindOneOf(lpszToken, szTokens);
    }

    if (bRun)
    {
        _Module.StartMonitor();

//#if _WIN32_WINNT >= 0x0400 & defined(_ATL_FREE_THREADED)
//        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
//            REGCLS_MULTIPLEUSE | REGCLS_SUSPENDED);
//        _ASSERTE(SUCCEEDED(hRes));
//        hRes = CoResumeClassObjects();
//#else
        hRes = _Module.RegisterClassObjects(CLSCTX_LOCAL_SERVER, 
            REGCLS_MULTIPLEUSE);
//#endif
        _ASSERTE(SUCCEEDED(hRes));

        g_dlg.Create(HWND_DESKTOP);
        
        //We start a countdown so that if this is launched interactively it will shut down in 10 seconds.
        g_dlg.BeginCountdown();
        MSG m;
        ATLTRACE("debugger mgr starting up...");
        while(::GetMessage(&m, 0,0,0))
            {
            ::TranslateMessage(&m);
            ::DispatchMessage(&m);
            }

        ATLTRACE("debugger mgr shutting down...");
        g_dlg.DestroyWindow();
    
        _Module.RevokeClassObjects();
        Sleep(dwPause); //wait for any threads to finish
    }

    _Module.Term();
    CoUninitialize();
    ATLTRACE("debug mgr over and out.\n");
    return nRet;
}
#include "ClientAccessor.h"
