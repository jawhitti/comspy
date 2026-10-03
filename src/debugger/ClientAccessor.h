// ClientAccessor.h : Declaration of the CClientAccessor

#ifndef __CLIENTACCESSOR_H_
#define __CLIENTACCESSOR_H_

#include "resource.h"       // main symbols

/////////////////////////////////////////////////////////////////////////////
// CClientAccessor
class ATL_NO_VTABLE CClientAccessor : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CClientAccessor, &CLSID_ClientAccessor>,
	public IClientAccessor
{
public:
	CClientAccessor()
	{
    ATLTRACE("Client accessor created\n");
	}

	~CClientAccessor()
	{
    ATLTRACE("Client accessor deleted\n");
	}

DECLARE_REGISTRY_RESOURCEID(IDR_CLIENTACCESSOR)

DECLARE_PROTECT_FINAL_CONSTRUCT()

BEGIN_COM_MAP(CClientAccessor)
	COM_INTERFACE_ENTRY(IClientAccessor)
END_COM_MAP()

// IClientAccessor
public:
	STDMETHODIMP GetProcessList(/*out*/long * pCount, /*out, size_is(,*pCount) */ struct PROCESS_INFO **ppInfo);
	STDMETHODIMP AttachToProcess(long ProcessId, /*in*/REFIID riid, /*out, iid_is(riid)*/IUnknown ** ppProcess);


	STDMETHODIMP Advise(/*in*/REFIID riid, /*in, iid_is(riid)*/IUnknown * pSink, /*out*/long * pCookie);
	STDMETHODIMP Unadvise(/*in*/long Cookie);

};

#endif //__CLIENTACCESSOR_H_
