// ClientAccessor.cpp : Implementation of CClientAccessor
#include "stdafx.h"
#include "Debugger.h"
#include "ClientAccessor.h"

/////////////////////////////////////////////////////////////////////////////
// CClientAccessor

STDMETHODIMP CClientAccessor::GetProcessList(/*out*/long * pCount, /*out, size_is(,*pCount) */struct  PROCESS_INFO **ppInfo)
    {
    return ::GetProcessList(pCount, ppInfo);
    }

STDMETHODIMP CClientAccessor::AttachToProcess(long ProcessId, /*in*/REFIID riid, /*out, iid_is(riid)*/IUnknown ** ppProcess)
    {
    return ::AttachToProcess(ProcessId, riid, ppProcess);
    }

STDMETHODIMP CClientAccessor::Advise(/*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie)
    {
    return ::ClientAdvise(riid, pSink, pCookie);
    }

STDMETHODIMP CClientAccessor::Unadvise(/*in*/long Cookie)
    {
    return ::ClientUnadvise(Cookie);
    }
