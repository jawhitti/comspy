// DebuggerImpl.h : Declaration of the CDebugger

#ifndef __DEBUGGER_H_
#define __DEBUGGER_H_

#include "resource.h"       // main symbols


#include "atlthread.h"


/////////////////////////////////////////////////////////////////////////////
// CDebugger
// Whenever the control spy is invoked it will contact us looking for one of these.  It will
// send us its PID and an all-important IUnknown pointer, and then will let go.  It is up
// to use (or other processes) to use this interface pointer.  CDebugger then turns out to
// be nothing more than an accessor object into the data structure being maintained by
// _Module.
class ATL_NO_VTABLE CDebugger : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDebugger, &CLSID_Debugger>,
	public IDebugger,
    public CMessageMap
{
public:
	CDebugger()
	{
	}

DECLARE_REGISTRY_RESOURCEID(IDR_DEBUGGER1)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CDebugger)
	COM_INTERFACE_ENTRY(IDebugger)
END_COM_MAP()

// IDebugger
public:
	STDMETHODIMP AdviseProcess(BSTR bstrProcessName,long ProcessId, REFIID iidPkt, IStream * pMarshaledPacket)
    {
    //TODO: Start a thread that pends on the shutdown of either this process or the 
    //process that just passed us  its PID.  This way if the debugee dies we can detect it
    //very quickly.  This debugger process should _not_ be crashing however, and in general we
    //may not want to let the user close it while there is a debugee open.
    return ::AdviseProcess(bstrProcessName, ProcessId, iidPkt, pMarshaledPacket);
    }

	//STDMETHODIMP UnadviseProcess(long ProcessId)
    //{
    //return ::UnadviseProcess(ProcessId, true);
    //}


//////////////////////////////////////
//Thread msg handlers
BEGIN_MSG_MAP(CDebugger)
END_MSG_MAP()
};

#endif //__DEBUGGER_H_







