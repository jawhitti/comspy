/////////////////////////////////////////////////////////////
// CoDelegator.cpp - Generic Delegator Component
//
// Copyright 2000, Jason Whittington
//
// This module serves up the DLL entry points for COM
// Spy.
/////////////////////////////////////////////////////////////


// ctlspysrv.cpp : Implementation of DLL Exports.


// Note: Proxy/Stub Information
//      To build a separate proxy/stub DLL, 
//      run nmake -f ctlspysrvps.mk in the project directory.

#include "stdafx.h"
#include "resource.h"
#include <initguid.h>
#include "ctlspysrv.h"

#include "ctlspysrv_i.c"
//#include "Spy.h"

#include "debugger.h"
#include "debugger_i.c"

#include "delegate.h"
#include "delegate_i.c"

//#include "delegatorsite.h"
//#include "delegatorsite_i.c"


CSpyModule _Module;


//The object map does nothing at all in this server. I do need some way to
//run my RGS file, though.  I do it manually in RegisterServer.
BEGIN_OBJECT_MAP(ObjectMap)
//  OBJECT_ENTRY(CLSID_Spy, CSpy)
END_OBJECT_MAP()

/////////////////////////////////////////////////////////////////////////////
// DLL Entry Point

extern "C"
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID /*lpReserved*/)
{
    if (dwReason == DLL_PROCESS_ATTACH)
    {
        _Module.Init(ObjectMap, hInstance/*, &LIBID_CTLSPYSRVLib*/);
        DisableThreadLibraryCalls(hInstance);
    }
    else if (dwReason == DLL_PROCESS_DETACH)
        {    
        ATLASSERT(_Module.GetLockCount()==0);
        ATLTRACE("SpyModule::DLL_PROCESS_DETACH\n");
        _Module.Term();
        ATLTRACE("SpyModule::Adios, muchacho\n");
        }
    return TRUE;    // ok
}

/////////////////////////////////////////////////////////////////////////////
// Used to determine whether the DLL can be unloaded by OLE

STDAPI DllCanUnloadNow(void)
{
    //We _MUST_ play along with these semantics.
    //ATLTRACE("DllCanUnloadNow\n");

    //I thought for a minute we could do shutdown here, but of course
    //if the host process is shutting down we could get dumped without
    //warning, and this would cause crashes.  So we _have_ to disconnect
    //from the outside world as soon as our lock count goes to zero.  The only
    //way that I can see to fix this would be to do a fixup on the address
    //of CoUninitialize() to point to a function of our own devising.
    //It's unclear how even that could really protect us however, because
    //_Module spans apartments.

    HRESULT hr = (_Module.GetLockCount()==0)? S_OK : S_FALSE;

    ATLTRACE("DllCanUnloadNow returning %08X\n", hr);

    return hr;
}

/////////////////////////////////////////////////////////////////////////////
// Returns a class factory to create an object of the requested type

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID* ppv)
{
    LPOLESTR pszguid;
    ::StringFromCLSID(rclsid, &pszguid);
    ATLTRACE("DllGetClassObject(%S)\n", pszguid);

    //We'll do the MTS trick of clobbering the InprocServer32 key to make this happen.  We'll
    //store the original path in "DebugHook" in parallel with InprocServer32.  Just like MTS,
    //a rebuild will of course clobber this.

    ::CoTaskMemFree(pszguid);
    return _Module.GetClassObject(rclsid, riid, ppv);
}

/////////////////////////////////////////////////////////////////////////////
// DllRegisterServer - Adds entries to the system registry

STDAPI DllRegisterServer(void)
{
    ATLTRACE("DllRegisterServer\n");
    // registers object, typelib and all interfaces in typelib
    return _Module.RegisterServer(TRUE);
}

/////////////////////////////////////////////////////////////////////////////
// DllUnregisterServer - Removes entries from the system registry

STDAPI DllUnregisterServer(void)
{
    ATLTRACE("DllUnregisterServer\n");
    return _Module.UnregisterServer(TRUE);
}


//Miscellany brought in to keep Keith's non-ATL delegator code happy.
void SvcLock()
{
	_Module.Lock();
}

void SvcUnlock()
{
	_Module.Unlock();
}

