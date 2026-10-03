// stdafx.h : include file for standard system include files,
//      or project specific include files that are used frequently,
//      but are changed infrequently

#if !defined(AFX_STDAFX_H__D57DFD91_3311_11D3_8BF2_00105A6DC077__INCLUDED_)
#define AFX_STDAFX_H__D57DFD91_3311_11D3_8BF2_00105A6DC077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define STRICT
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0400
#endif
#define _ATL_APARTMENT_THREADED

#include <atlbase.h>
#include "debugger.h"

//global functions called by CDebugger. 
HRESULT AdviseProcess(BSTR bstrProcessName,long ProcessId, REFIID iidPkt, IStream * pMarshaledPacket);
HRESULT GetProcessList(long * pCount, PROCESS_INFO ** ppInfo);
HRESULT AttachToProcess(long ProcessId, REFIID riid, IUnknown ** ppProcess);
void ScanProcesses();
void AttemptShutdown();

HRESULT ClientAdvise(/*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie);
HRESULT ClientUnadvise(/*in*/long Cookie);

HRESULT Fire_OnProcessAdded(PROCESS_INFO * pinfo);
HRESULT Fire_OnProcessClosed(PROCESS_INFO * pinfo);



//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
class CExeModule : public CComModule
{
public:
    LONG Lock();
	LONG Unlock();
	DWORD dwThreadID;
	HANDLE hEventShutdown;
	void MonitorShutdown();
	bool StartMonitor();
	bool bActivity;

};
extern CExeModule _Module;
#include <atlcom.h>
#include <atlwin.h>

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__D57DFD91_3311_11D3_8BF2_00105A6DC077__INCLUDED)
