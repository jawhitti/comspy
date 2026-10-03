// LeftView.h : interface of the CLeftView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_LEFTVIEW_H__C532F73C_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_)
#define AFX_LEFTVIEW_H__C532F73C_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "debugger.h"
#include "ctlspysrv.h"

#include "methodnames.h"


class CSpyguiDoc;
class CLeftView;

BOOL GetInterfaceName(REFIID iid, LPTSTR szName, long Size);

void DeleteNode(CLeftView* tree, HTREEITEM item);
void DeleteChildren(CLeftView* tree, HTREEITEM item);


struct ProcessNodeHelper;


//CEventHelper is a simple base class.  Whenever an event happens one of these
//will be created and handed off.  

struct CEventHelper : public CObject
    {
    virtual void Render(CListCtrl& lv) = 0;
    virtual ~CEventHelper(){}
    };

struct CMethodEventHelper : public CEventHelper
    {
    TCHAR szObject[128];
    IID  iid;
    LONG nVtblIndex;
    HRESULT hr;

    void Render(CListCtrl& ListCtrl) 
        {
        long count = ListCtrl.GetItemCount();
        LVITEM lv;
        lv.iItem = count;
        lv.iSubItem = 0;
        lv.mask = LVIF_TEXT | LVIF_IMAGE;
        lv.pszText = szObject;

        //Figure out which image to use...
        if(hr == S_OK) lv.iImage = 0;
        else if(SUCCEEDED(hr)) lv.iImage = 3;
        else lv.iImage = 2;

        int idx = ListCtrl.InsertItem(&lv);

        lv.iSubItem = 1;
        TCHAR szIface[256];
        TCHAR szBuf[256];

        ::GetInterfaceName(iid, szIface, 64);

        //Get the method name
        BSTR bstrMethod;
        ::GetMethodName(iid, nVtblIndex, &bstrMethod);
        sprintf(szBuf,"%s::%S",szIface,bstrMethod);
        ::SysFreeString(bstrMethod);

        lv.pszText = szBuf;
        ListCtrl.SetItem(&lv);

    #define STRINGIZE(hr) case hr: sprintf(szBuf, "(0x%08X) %s", hr, #hr); break;

        switch(hr)
            {
            STRINGIZE(S_OK);
            STRINGIZE(S_FALSE);
            STRINGIZE(E_FAIL);

            default:
                long len = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr, 0, szIface, 256, 0);
                if(len == 0)
                    {
                    sprintf(szBuf,"(0x%08X) ?????", hr);
                    }
                else
                    {
                    //strip the newline...
                    TCHAR * pch = ::strchr(szIface,'\r');
                    if(pch != NULL) *pch = 0;
                    sprintf(szBuf,"(0x%08X) %s", hr, szIface);
                    }
            }
        lv.iSubItem=2;
        ListCtrl.SetItem(&lv);


        ListCtrl.EnsureVisible(count, TRUE);
        }
    };


struct CRefcountEventHelper : public CEventHelper
    {
    TCHAR szObject[128];
    IID  iid;
    LONG oldCount;
    long newCount;
    HRESULT hr;

    void Render(CListCtrl& ListCtrl) 
        {
        long count = ListCtrl.GetItemCount();
        LVITEM lv;
        lv.iItem = count;
        lv.iSubItem = 0;
        lv.mask = LVIF_TEXT | LVIF_IMAGE;
        lv.pszText = szObject;

        //Figure out which image to use...
        if(newCount > oldCount) lv.iImage = 5;
        else lv.iImage = 6;

        int idx = ListCtrl.InsertItem(&lv);

        lv.iSubItem = 1;
        TCHAR szIface[128];
        ::GetInterfaceName(iid, szIface, 64);


        char szBuf[128];
        if(newCount > oldCount)
            sprintf(szBuf,"%s::AddRef()", szIface);
        else
            sprintf(szBuf,"%s::Release()", szIface);

        lv.pszText = szBuf;
        ListCtrl.SetItem(&lv);

        sprintf(szBuf,"%08d", newCount);

        lv.iSubItem = 2;
        ListCtrl.SetItem(&lv);

        ListCtrl.EnsureVisible(count, TRUE);
        }
    };


struct CQIEventHelper : public CEventHelper
    {
    TCHAR szObject[128];
    IID  iid;
    LONG oldCount;
    long newCount;
    HRESULT hr;

    void Render(CListCtrl& ListCtrl) 
        {
        long count = ListCtrl.GetItemCount();
        LVITEM lv;
        lv.iItem = count;
        lv.iSubItem = 0;
        lv.mask = LVIF_TEXT | LVIF_IMAGE;
        lv.pszText = szObject;

        //Figure out which image to use...
        if(SUCCEEDED(hr)) lv.iImage = 8;
        else lv.iImage = 9;

        int idx = ListCtrl.InsertItem(&lv);

        lv.iSubItem = 1;
        TCHAR szIface[128];

        char szBuf[128];
        ::GetInterfaceName(iid, szIface, 64);
        sprintf(szBuf,"IUnknown::QueryInterface(%s)", szIface);

        lv.pszText = szBuf;
        ListCtrl.SetItem(&lv);
        if(SUCCEEDED(hr))
            sprintf(szBuf,"%08d", newCount);
        else
            {
            switch(hr)
                {
                STRINGIZE(S_OK);
                STRINGIZE(S_FALSE);
                STRINGIZE(E_FAIL);

                default:
                    long len = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, hr, 0, szIface, 64, 0);
                    if(len == 0)
                        {
                        sprintf(szBuf,"(0x%08X) ?????", hr);
                        }
                    else
                        {
                        //strip the newline...
                        TCHAR * pch = ::strchr(szIface,'\r');
                        if(pch != NULL) *pch = 0;
                        sprintf(szBuf,"(0x%08X) %s", hr, szIface);
                        }
                }
            } 


        lv.iSubItem = 2;
        ListCtrl.SetItem(&lv);

        ListCtrl.EnsureVisible(count, TRUE);
        }
    };


struct CCreateOrDeleteEventHelper : public CEventHelper
    {
    BOOL bCreate;
    TCHAR szObject[128];

    void Render(CListCtrl& ListCtrl) 
        {
        long count = ListCtrl.GetItemCount();
        LVITEM lv;
        lv.iItem = count;
        lv.iSubItem = 0;
        lv.mask = LVIF_TEXT | LVIF_IMAGE;
        lv.pszText = szObject;

        lv.iImage = 4;

        int idx = ListCtrl.InsertItem(&lv);

        lv.pszText = bCreate ? "Object Created" : "Object Destroyed";
        lv.iSubItem = 1;
        ListCtrl.SetItem(&lv);


        ListCtrl.EnsureVisible(count, TRUE);
        }
    };

struct CGenericMessageHelper : public CEventHelper
    {
    LONG iIcon;
    LONG iIndent;
    TCHAR szObject[128];
    TCHAR szMsg[128];

    CGenericMessageHelper(LPTSTR szObj, LPTSTR Msg, LONG icon, LONG indent = 0)
        {
        iIcon = icon;
        strcpy(szObject, szObj);
        strcpy(szMsg, Msg);
        iIndent = indent;
        }

    void Render(CListCtrl& ListCtrl) 
        {
        long count = ListCtrl.GetItemCount();
        LVITEM lv;
        lv.iItem = count;
        lv.iSubItem = 0;
        lv.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_INDENT;
        lv.pszText = szObject;

        lv.iImage = iIcon;
        lv.iIndent = iIndent;
        lv.iItem = ListCtrl.InsertItem(&lv);

        lv.pszText = szMsg;
        lv.mask =  LVIF_TEXT;
        lv.iSubItem = 1;
        ListCtrl.SetItem(&lv);


        ListCtrl.EnsureVisible(count, TRUE);
        }
    };

///////////////////////////////////////////////////////////////////////////////////
//simple event sink for CLeftView
struct CProcessEventSink : public IDebuggerEvents
    {
    CLeftView * m_pOwner;

    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
        {
        if(ppv == NULL) return E_POINTER;
        *ppv = 0;
        if((riid == IID_IDebuggerEvents) || (riid== IID_IUnknown))
            {
            *ppv = reinterpret_cast<IObjectEvents*>(this);
            }
        if(*ppv == 0)
           return E_NOINTERFACE;
        else 
           return  S_OK;
        }
    STDMETHODIMP_(ULONG) AddRef(){return 8;}
    STDMETHODIMP_(ULONG) Release(){return 5;}


    //IDebuggerEvents
    STDMETHODIMP OnProcessAdded(PROCESS_INFO * pinfo);
    STDMETHODIMP OnProcessClosed(PROCESS_INFO * pinfo);
    };


///////////////////////////////////////////////////////////////////////////////////
//simple event sink for CLeftView
class CObjEventSink : public IObjectEvents
    {
    public:
    CLeftView * m_pOwner;
    ProcessNodeHelper * m_pProcess;
    HTREEITEM hItem;

    STDMETHODIMP QueryInterface(REFIID riid, void ** ppv)
        {
        if(ppv == NULL) return E_POINTER;
        *ppv = 0;
        if((riid == IID_IObjectEvents) || (riid== IID_IUnknown))
            {
            *ppv = reinterpret_cast<IObjectEvents*>(this);
            }
        if(*ppv == 0)
           return E_NOINTERFACE;
        else 
           return  S_OK;
        }
    STDMETHODIMP_(ULONG) AddRef(){return 8;}
    STDMETHODIMP_(ULONG) Release(){return 5;}


    //IObjectEvents
	STDMETHODIMP ObjCreated(/*in*/ struct IDENTITY_INFO  *pif);
	STDMETHODIMP ObjDestroyed(/*in*/long Cookie);
	STDMETHODIMP ObjQI(/*in*/long Cookie, /*in*/REFIID riid, /*in*/ULONG newCount, /*in*/HRESULT hr);
	STDMETHODIMP ObjRefCountChanged(/*in*/long Cookie, /*in*/REFIID riid, /*in*/ULONG oldCount, /*in*/ULONG newCount);
	STDMETHODIMP ObjMethodCalled(/*in*/long Cookie, /*in*/REFIID riid, /*in*/long vtblIndex, /*in*/long reserved, /*in*/HRESULT hr);
    };


enum ObjectType_t
    {
    Process = 1,
    Object  = 2,
    Interface = 3,
    Method  = 4
    };


//The various things that go into the tree control all have a lot of similar attributes.
//GenericThingy acts a base class for them.  As the user manipulates the tree control we
//will spend as much time as possible manipulating them via GenericHelper.
struct GenericHelper
    {
    ObjectType_t ObjType;
    long dwFlags;

    //We use these helper objects to insert the nodes.  They can
    //decide what children to insert.  I decided to have the node objects
    //do that in their constructors instead of explicitly.
    //virtual void InsertNode(CLeftView * view, HTREEITEM hParent) = 0;
    
    //When we insert most nodes for the first time we will insert dummy nodes below
    //so the user knows there is something there, but we won't calculate anything until
    //the node is expanding.  At that point we will ask the node to expand itself.
    virtual void ExpandNode(CLeftView * view, HTREEITEM h) = 0;

    virtual void Destroy(CLeftView * view, HTREEITEM h){DeleteChildren(view, h);}

    //Each object will have its own context menu.
    virtual void OnRClick(CLeftView * view, HTREEITEM h){}
    virtual ~GenericHelper(){};
    };

//ProcessNodeHelper hangs onto a pointer into the process.
//It watches for object creation and destruction, and manages
//its child nodes.  It lets the user set up single step mode.
//ProcessNodeHelper registers for events, and manages the 
//text of the child nodes to reflect interface counts.
struct ProcessNodeHelper : public GenericHelper
    {
    ProcessNodeHelper(CLeftView * pView, IClientAccessor * pAccessor, PROCESS_INFO *pinfo, HTREEITEM hParent);
    ~ProcessNodeHelper();

    //These two vars tell us the machine/process we represent
    CComPtr<IClientAccessor> pDebugManager;
    long ProcessId;

    CObjEventSink m_EventSink;
    long m_EventCookie;

    //If the process is open for debugging then our pointer to
    //it is stored here.
    CComPtr<ISpyAccessor> pProcess;

    //void InsertNode(CLeftView * view, HTREEITEM hParent);
    void ExpandNode(CLeftView * view, HTREEITEM h);
    };

//ObjectNodeHelper manages displaying the list of interfaces. I don't
//think there's much interesting to be done at the object level.
struct ObjectNodeHelper : public GenericHelper
    {
    ObjectNodeHelper(CLeftView * pView, ISpyAccessor * pAccessor, IDENTITY_INFO *pinfo, HTREEITEM hParent);

    //These member vars track which object we represent
    CComPtr<ISpyAccessor> pModule;
    long ObjectId;
    bool bInit;

    //void InsertNode(CLeftView * view, HTREEITEM hParent);
    void ExpandNode(CLeftView * view, HTREEITEM h);
    };

//InterfaceNodeHelper allows the user to enable/disable
//interfaces or break on QI for this interfaces.
struct InterfaceNodeHelper : public GenericHelper
    {
    InterfaceNodeHelper(CLeftView * pView, REFCOUNT* prc, HTREEITEM hParent);

    IID m_IID;
    //void InsertNode(CLeftView * view, HTREEITEM hParent);
    void ExpandNode(CLeftView * view, HTREEITEM h);
    };

//MethodNodeHelper displays the method names and lets the
//user set breakpoints.
struct MethodNodeHelper : public GenericHelper
    {
    //void InsertNode(CLeftView * view, HTREEITEM hParent);
    void ExpandNode(CLeftView * view, HTREEITEM h);
    };



#define WM_OBJCREATED         (WM_APP+1)
#define WM_OBJDESTROYED       (WM_APP+2)
#define WM_OBJREFCOUNTCHANGED (WM_APP+3) 
#define WM_OBJMETHODCALLED    (WM_APP+4) 
#define WM_OBJQI              (WM_APP+5) 
#define WM_PROCESSCREATED     (WM_APP+6) 
#define WM_PROCESSDESTROYED   (WM_APP+7) 


struct METHODCALL
{
LPTSTR szObject;
IID * piid;
LONG nVtblIndex;
HRESULT hr;
};

///////////////////////////////////////////////////////////////////////////////////
//I'll let CLeftView drive most of the show, since this window is where most of the
//interesting info is going to be displayed.  The right view is simply going to display
//messages.
class CLeftView : public CTreeView
{
protected: // create from serialization only
	CLeftView();
	DECLARE_DYNCREATE(CLeftView)

// Attributes
public:
	CSpyguiDoc* GetDocument();
    CImageList m_ImageList;
    CComPtr<IClientAccessor> m_pAccessor;
    long m_DebuggerCookie;

    friend struct CProcessEventSink;
    CProcessEventSink m_ProcessSink;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CLeftView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


public:
//void RegisterForEvents(ISpyAccessor * pProcess, HTREEITEM hThis, ProcessNodeHelper * pHelper);

HRESULT OnProcessAdded(PROCESS_INFO * pinfo);
HRESULT OnProcessClosed(PROCESS_INFO * pinfo);

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CLeftView)
	afx_msg void OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
    //afx_msg LRESULT OnProcessCreated(WPARAM wParam, LPARAM lParam);
    //afx_msg LRESULT OnProcessDestroyed(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnObjCreated(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnObjDestroyed(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnObjQI(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnObjRefCountChanged(WPARAM wParam, LPARAM lParam);
    afx_msg LRESULT OnObjMethodCalled(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in LeftView.cpp
inline CSpyguiDoc* CLeftView::GetDocument()
   { return (CSpyguiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTVIEW_H__C532F73C_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_)
