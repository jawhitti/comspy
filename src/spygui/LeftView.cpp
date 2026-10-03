// LeftView.cpp : implementation of the CLeftView class
//

#include "stdafx.h"
#include "spygui.h"

#include "spyguiDoc.h"
#include "LeftView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif




void DeleteNode(CLeftView* tree, HTREEITEM item)
    {
    //Hack, hack, hack...

    TVITEM tv;
    tv.hItem = item;
    tv.mask = TVIF_PARAM;
    tv.lParam = 0;
    tree->GetTreeCtrl().GetItem(&tv);

    GenericHelper * p = reinterpret_cast<GenericHelper *>(tv.lParam); 
    ATLASSERT(p!=NULL);
    p->Destroy(tree, item);
    switch(p->ObjType)
        {
        case Process:
           delete reinterpret_cast<ProcessNodeHelper*>(p);
           break;
        case Object:
           delete reinterpret_cast<ObjectNodeHelper*>(p);
           break;
        case Interface:
           delete reinterpret_cast<InterfaceNodeHelper*>(p);
           break;
        case Method:
           delete reinterpret_cast<MethodNodeHelper*>(p);
           break;
        }
    tree->GetTreeCtrl().DeleteItem(item);
    }

void DeleteChildren(CLeftView* tree, HTREEITEM item)
    {
    HTREEITEM h = tree->GetTreeCtrl().GetChildItem(item);
    while(h != NULL)
        {
        HTREEITEM hNext = tree->GetTreeCtrl().GetNextItem(h, TVGN_NEXT);
        DeleteNode(tree, h);
        h = hNext;
        }
    }

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
// CLeftView

IMPLEMENT_DYNCREATE(CLeftView, CTreeView)

BEGIN_MESSAGE_MAP(CLeftView, CTreeView)
	//{{AFX_MSG_MAP(CLeftView)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemExpanding)
	ON_NOTIFY_REFLECT(TVN_GETDISPINFO, OnGetdispinfo)
    ON_MESSAGE(WM_OBJCREATED, OnObjCreated)
    ON_MESSAGE(WM_OBJQI, OnObjQI)
    ON_MESSAGE(WM_OBJDESTROYED, OnObjDestroyed)
    ON_MESSAGE(WM_OBJREFCOUNTCHANGED, OnObjRefCountChanged)
    ON_MESSAGE(WM_OBJMETHODCALLED, OnObjMethodCalled)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLeftView construction/destruction

CLeftView::CLeftView()
{
	// TODO: add construction code here
m_pAccessor.p = 0;
}

CLeftView::~CLeftView()
{
}

BOOL CLeftView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.style |= TVS_HASLINES | TVS_HASBUTTONS | TVS_LINESATROOT;
	return CTreeView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CLeftView drawing

void CLeftView::OnDraw(CDC* pDC)
{
	CSpyguiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}


void CLeftView::OnInitialUpdate()
{
	CTreeView::OnInitialUpdate();

    m_ProcessSink.m_pOwner = this;

    //Set up the image list for our tree control...
    if(m_ImageList.Create(IDB_BITMAP1, 16, 0, RGB(255,255,255)))
        GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);


    //Contact the local machine for debugging...
    HRESULT hr = m_pAccessor.CoCreateInstance(L"Debugger.ClientAccessor", NULL, CLSCTX_LOCAL_SERVER);

    if(SUCCEEDED(hr) &&(m_pAccessor))
        {
        m_pAccessor->Advise(IID_IDebuggerEvents, &m_ProcessSink, &m_DebuggerCookie);
        
        //Now lets fill out our tree view...
        long count;
        PROCESS_INFO * pProcessInfo;
	    if(SUCCEEDED(m_pAccessor->GetProcessList(&count, &pProcessInfo)))
            {
            for(int i=0; i<count; i++)
                {
                OnProcessAdded(pProcessInfo+i);

                ::SysFreeString(pProcessInfo[i].bstrProcessName);
                }
            ::CoTaskMemFree((void*)pProcessInfo);
            }
        }
     else
        {
        MessageBox("Unable to contact debug manager (register agent.exe and debuggerps.dll)", "Whoops", MB_OK | MB_ICONEXCLAMATION);
        }

}

/////////////////////////////////////////////////////////////////////////////
// CLeftView diagnostics

#ifdef _DEBUG
void CLeftView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CLeftView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}

CSpyguiDoc* CLeftView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpyguiDoc)));
	return (CSpyguiDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CLeftView message handlers

void CLeftView::OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
    
    //TODO: Extract the LPARAM from the node, then cast it to a GenericHelper.
    //Call the "ExpandNode" function.
    ATLASSERT(pNMTreeView->itemNew.lParam != NULL);
    GenericHelper * pGH = reinterpret_cast<GenericHelper*>(pNMTreeView->itemNew.lParam);
	
    pGH->ExpandNode(this, pNMTreeView->itemNew.hItem);

	*pResult = 0;
}


void CLeftView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

    //The only reason we should get this call is when the system
    //wants to know if a given node has children.
	
    pTVDispInfo->item.cChildren = 1;
    pTVDispInfo->item.mask |= TVIF_DI_SETITEM;

	*pResult = 0;
}


//void CLeftView::RegisterForEvents(ISpyAccessor * pProcess, HTREEITEM hThis, ProcessNodeHelper * pHelper)
//{
//}

HRESULT CLeftView::OnProcessAdded(PROCESS_INFO * pinfo)
    {
    //We will register a sink per process...
    ProcessNodeHelper * p = new ProcessNodeHelper(this, m_pAccessor,pinfo, TVI_ROOT);


    //Dump the fact that the process appeared:
    USES_CONVERSION;
    CGenericMessageHelper * pmsg = new CGenericMessageHelper( OLE2T(pinfo->bstrProcessName), "Process Opened", 4);
    //strcpy(pmsg->szObject,);
    //strcpy(pmsg->szMsg, "Process opened");
    GetDocument()->UpdateAllViews(this, 1, pmsg);


    return S_OK;
    }
HRESULT CLeftView::OnProcessClosed(PROCESS_INFO * pinfo)
    {
    //TODO: post an informational note that the process died.  If we still show objects
    //outstanding then we should dump any outstanding interfaces as well.

    HTREEITEM hItem = GetTreeCtrl().GetChildItem(TVI_ROOT);
    while(hItem != NULL)
        {
        TCHAR buf[128];
        TVITEM tv;
        tv.hItem = hItem;
        tv.mask = TVIF_PARAM | TVIF_TEXT;
        tv.lParam = 0;
        tv.pszText = buf;
        tv.cchTextMax = 128;

        GetTreeCtrl().GetItem(&tv);

        ProcessNodeHelper * p = reinterpret_cast<ProcessNodeHelper*>(tv.lParam);
        ATLASSERT((p!=NULL)&&(p->ObjType == Process));
        //We got the object
        if(p->ProcessId == pinfo->dwCookie)
            {
            CGenericMessageHelper * pmsg = new CGenericMessageHelper("Process closed", buf, 4);
            //strcpy(pmsg->szMsg, buf);
            //strcpy(pmsg->szObject, );
            GetDocument()->UpdateAllViews(this, 1, pmsg);

            HTREEITEM hObject = GetTreeCtrl().GetChildItem(hItem);
            
            //Enumerate each object that's still alive...
            while(hObject != NULL)
                {
                //Get the object...
                tv.hItem = hObject;
                GetTreeCtrl().GetItem(&tv);
    
                CGenericMessageHelper * pmsg = new CGenericMessageHelper("Object Leaked:", buf, 3,1);
                //strcpy(pmsg->szMsg, buf);
                //strcpy(pmsg->szObject, "Object stranded");
                GetDocument()->UpdateAllViews(this, 1, pmsg);

                //Enumerate the interfaces on the given object...
                HTREEITEM hIface = GetTreeCtrl().GetChildItem(hObject);
                while(hIface != NULL)
                    {
                    tv.hItem = hIface;
                    tv.mask |= TVIF_STATE;
                    GetTreeCtrl().GetItem(&tv);

                    if(tv.state & TVIS_BOLD) 
                        {
                        CGenericMessageHelper * pmsg = new CGenericMessageHelper("Interface Leak:", tv.pszText, 10,2);
                        strcpy(pmsg->szObject, "Outstanding interface:");
                        strcpy(pmsg->szMsg, tv.pszText);
                        GetDocument()->UpdateAllViews(this, 1, pmsg);
                        }
                    hIface = GetTreeCtrl().GetNextItem(hIface, TVGN_NEXT);
                    }
    
                HTREEITEM hNextObject = GetTreeCtrl().GetNextItem(hObject, TVGN_NEXT);
                //First delete the interfaces under this object...
                //DeleteChildren(this, hObject);
                //...then delete this object node
                DeleteNode(this, hObject);
                hObject = hNextObject;
                }


            //At this point we have deleted everything under the process node; now we just delete the
            //process node itself.
            

            //p->pProcess = 0;
            //DeleteChildren(this, hItem);
            DeleteNode(this, hItem);
            //GetTreeCtrl().DeleteItem(hItem);
            break;
            }
        hItem = GetTreeCtrl().GetNextItem(hItem, TVGN_NEXT);
        }

    return S_OK;

    }


//    ON_MESSAGE(WM_OBJCREATED, OnObjCreated)
//    ON_MESSAGE(WM_OBJDESTROYED, OnObjDestroyed)
//    ON_MESSAGE(WM_OBJREFCOUNTCHANGED, OnObjRefCountChanged)
//    ON_MESSAGE(WM_OBJMETHODCALLED, OnObjMethodCalled)

LRESULT CLeftView::OnObjCreated(WPARAM wParam, LPARAM lParam)
    {
    //TODO: insert a new object node.  This will happen 
    //infrequently, so it can be slow.
    GetDocument()->UpdateAllViews(this,WM_OBJCREATED,(CObject*)lParam);
    return 0;
    }
LRESULT CLeftView::OnObjDestroyed(WPARAM wParam, LPARAM lParam)
    {
    //TODO: remove the object node.  This will also be
    //infrequent.
    GetDocument()->UpdateAllViews(this,WM_OBJDESTROYED,(CObject*)lParam);
    return 0;
    }
LRESULT CLeftView::OnObjQI(WPARAM wParam, LPARAM lParam)
    {
    GetDocument()->UpdateAllViews(this,WM_OBJQI,(CObject*)lParam);

    return 0;
    }
LRESULT CLeftView::OnObjRefCountChanged(WPARAM wParam, LPARAM lParam)
    {
    //TODO: modify the tree view.  We need to change the text at least
    //and switch bold and the icon when we transition to/from
    //zero.
    GetDocument()->UpdateAllViews(this,WM_OBJREFCOUNTCHANGED,(CObject*)lParam);

    return 0;
    }
LRESULT CLeftView::OnObjMethodCalled(WPARAM wParam, LPARAM lParam)
    {
    //TODO: insert something into the doc and have
    //the other view render the text.  This is going to
    //be called more frequently than any of the others, although
    //OnObjRefCountChanged will be close in some environments that
    //habitually call AddRef/function/Release.
    GetDocument()->UpdateAllViews(this,WM_OBJMETHODCALLED,(CObject*)lParam);

    return 0;
    }


/////////////////////////////////////////////////////////////////////////////
// HELPERS
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
//ProcessNodeHelper
/////////////////////////////////////////////////////////////////////////////
ProcessNodeHelper::ProcessNodeHelper(CLeftView * pView, IClientAccessor * pDM, PROCESS_INFO *pinfo, HTREEITEM hParent)
    {
    USES_CONVERSION;

    dwFlags = 0;//currently no flags per se.
    ObjType = Process;
    pProcess = NULL;

    ProcessId = pinfo->dwCookie;
    pDebugManager = pDM;
    //pDM->AddRef();  Unnecessary - the owner view is holding this alive.

    TVINSERTSTRUCT tv;
    tv.hParent = hParent;
    tv.hInsertAfter = TVI_LAST;

    OLECHAR * pch = ::wcsrchr(pinfo->bstrProcessName,'\\');
    pch = pch? pch+1 : pinfo->bstrProcessName;

    tv.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_STATE | TVIF_TEXT | TVIF_CHILDREN;
    tv.item.state = TVIS_EXPANDED;
    tv.item.stateMask = 0;
    tv.item.pszText = W2T(pch);
    tv.item.iImage = 0;
    tv.item.iSelectedImage = 0;
    tv.item.cChildren = -1;
    tv.item.lParam = (LPARAM)this;

    HTREEITEM hThis = pView->GetTreeCtrl().InsertItem(&tv);


    //Go ahead and get the objects and sign up for object notifications...
    ATLASSERT(pDebugManager != NULL);
    if(SUCCEEDED(pDebugManager->AttachToProcess(ProcessId, IID_ISpyAccessor, (IUnknown**)&pProcess)))
        {
        //add object nodes for each object.
        long count;
        IDENTITY_INFO * pObjects;
        if(SUCCEEDED(pProcess->GetAllObjects(&count, &pObjects)))
            {
            for(int i=0; i<count; i++)
                {
                //Creating this object node will stick the object and all the interfaces into the tree.
                ObjectNodeHelper * p = new ObjectNodeHelper(pView, pProcess, pObjects+i, hThis);
                }
            ::CoTaskMemFree(reinterpret_cast<void*>(pObjects));
            }


        //Sign up for events...
        m_EventSink.m_pOwner = pView;
        m_EventSink.hItem = hThis;
        m_EventSink.m_pProcess = this;
        pProcess->Advise(0,IID_IObjectEvents,(IObjectEvents*)&m_EventSink, &m_EventCookie);
        }
    }

ProcessNodeHelper::~ProcessNodeHelper()
    {
    //First we disconnect notifications...
    if(pProcess != NULL)
        {
        pProcess->Unadvise(m_EventCookie);
        pProcess.Release();
        }
    ATLASSERT(pDebugManager);
    pDebugManager.Release();
    }


void ProcessNodeHelper::ExpandNode(CLeftView * pView, HTREEITEM hThis)
    {
    }

/////////////////////////////////////////////////////////////////////////////
//ObjectNodeHelper
/////////////////////////////////////////////////////////////////////////////
ObjectNodeHelper::ObjectNodeHelper(CLeftView * pView, ISpyAccessor * pAccessor, IDENTITY_INFO *pinfo, HTREEITEM hParent)
{
USES_CONVERSION;

//set up our object...
ObjType = Object;
dwFlags = pinfo->dwFlags;

pModule = pAccessor;  
ObjectId = pinfo->dwCookie;

bInit = false;

//now insert ourselves into the tree.
char szName[128];
sprintf(szName, "0x%08X - (%S)", pinfo->dwCookie, pinfo->bstrFriendlyName);


TVINSERTSTRUCT tv;
tv.hParent = hParent;
tv.hInsertAfter = TVI_LAST;

tv.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_TEXT | TVIF_CHILDREN;
tv.item.state = 0;
tv.item.stateMask = 0;
tv.item.pszText = szName;
tv.item.iImage = 1;
tv.item.iSelectedImage = 1;
tv.item.cChildren = -1;
tv.item.lParam = (LPARAM)this;

HTREEITEM hThis = pView->GetTreeCtrl().InsertItem(&tv);

//We have to get this information pronto, because if the process dies we want to know what
//interfaces were outstanding.  It takes freaking forever if we do it this way, though - 
//several seconds per object created.  So this should be an option.
//ExpandNode(pView,hThis);
}


void ObjectNodeHelper::ExpandNode(CLeftView * pView, HTREEITEM hThis)
{
//Now suck out the interfaces for the object and stick those in as well.  This could take awhile,
//so we set the cursor to an hourglass while we wait.
if(!bInit)
    {
    HCURSOR h = AfxGetApp()->LoadStandardCursor(IDC_WAIT);
    ::SetCursor(h);

    CComPtr<IDebuggedObject> pObj;
    if(SUCCEEDED(pModule->AccessObject(ObjectId, IID_IDebuggedObject, (IUnknown**)&pObj)))
        {
        long count;
        REFCOUNT * pRefs;

        if(SUCCEEDED(pObj->GetInterfaceCounts(1, &count, &pRefs)))
            {
            for(int i=0; i<count; i++)
                {
                TCHAR szBuf[128];
                ::GetInterfaceName(pRefs[i].iid,szBuf,128);

                InterfaceNodeHelper * p = new InterfaceNodeHelper(pView, pRefs+i, hThis);
                }

            ::CoTaskMemFree((void*)pRefs);
            
            }    
        bInit = true;
        }
    }
}

//Just like the process node we won't add methods to the tree view until the user expands 
//the node.
InterfaceNodeHelper::InterfaceNodeHelper(CLeftView * pView, REFCOUNT* prc, HTREEITEM hParent)
{
ObjType = Interface;
dwFlags = 0;
m_IID = prc->iid;

TCHAR szIID[128];
::GetInterfaceName(m_IID, szIID, 128);

TCHAR szBuf[128];
if(prc->rfcount > 0)
  {      
  sprintf(szBuf,"%s (%d)", szIID, prc->rfcount);
  }
else
  {
  strcpy(szBuf, szIID);
  }

TVINSERTSTRUCT tv;
tv.hParent = hParent;
tv.hInsertAfter = TVI_LAST;

tv.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_PARAM | TVIF_TEXT | TVIF_STATE | TVIF_CHILDREN;
tv.item.state = (prc->rfcount > 0)? TVIS_BOLD : 0;
tv.item.stateMask = TVIS_BOLD;
tv.item.pszText = szBuf;
tv.item.iImage = (prc->rfcount > 0)? 4 : 5;
tv.item.iSelectedImage = (prc->rfcount > 0)? 4 : 5;
tv.item.cChildren = 0;
tv.item.lParam = (LPARAM)this;

HTREEITEM hThis = pView->GetTreeCtrl().InsertItem(&tv);
}

/////////////////////////////////////////////////////////////////////////////
//InterfaceNodeHelper
/////////////////////////////////////////////////////////////////////////////
//When the user expands the interface node for the first time we look up the methods
//and add them.  Finding the method names will be ... um challenging.
void InterfaceNodeHelper::ExpandNode(CLeftView * view, HTREEITEM h)
{
}


/////////////////////////////////////////////////////////////////////////////
//MethodNodeHelper
/////////////////////////////////////////////////////////////////////////////
void MethodNodeHelper::ExpandNode(CLeftView * view, HTREEITEM h)
{
}



void DoctorTreeItem(CTreeCtrl& treectrl, HTREEITEM hItem, LONG oldCount, ULONG newCount)
{
TVITEM tv;

TCHAR szBuf[128];
tv.hItem = hItem;
tv.mask = TVIF_IMAGE |  TVIF_TEXT | TVIF_STATE;
tv.pszText = szBuf;
tv.cchTextMax = 128;

treectrl.GetItem(&tv);

TCHAR * pch = ::strchr(szBuf, '(');
if(pch != NULL) *pch = 0;

TCHAR szBuf2[128];
if(newCount > 0)
   sprintf(szBuf2, "%s(%d)", szBuf, newCount);
else
    strcpy(szBuf2, szBuf);

//now twiddle the fields
tv.stateMask = TVIS_BOLD;
tv.state = (newCount > 0)? TVIS_BOLD : 0;
tv.iImage =         (newCount > 0)? 4 : 5;
tv.iSelectedImage = (newCount > 0)? 4 : 5;
tv.pszText = szBuf2;

treectrl.SetItem(&tv);

}


STDMETHODIMP CObjEventSink::ObjCreated(/*in*/ struct IDENTITY_INFO  *pif)
    {
    ObjectNodeHelper * p = new ObjectNodeHelper(m_pOwner, m_pProcess->pProcess, pif, hItem);

    m_pOwner->GetTreeCtrl().Expand(hItem, TVE_EXPAND);

    CCreateOrDeleteEventHelper * pHelper = new CCreateOrDeleteEventHelper;
    sprintf(pHelper->szObject, "%S", pif->bstrFriendlyName);
    pHelper->bCreate = TRUE;

    m_pOwner->PostMessage(WM_OBJCREATED, 0, (LPARAM)pHelper);

    return S_OK;
    }

STDMETHODIMP CObjEventSink::ObjDestroyed(/*in*/long Cookie)
    {
    HTREEITEM hChildItem = m_pOwner->GetTreeCtrl().GetChildItem(hItem);
    while(hChildItem != NULL)
        {
        char szName[128];
        TVITEM tv;
        tv.mask = TVIF_PARAM | TVIF_TEXT;
        tv.hItem = hChildItem;
        tv.lParam = 0;
        tv.pszText = szName;
        tv.cchTextMax = 128;
       
        m_pOwner->GetTreeCtrl().GetItem(&tv);


        ObjectNodeHelper * p = reinterpret_cast<ObjectNodeHelper*>(tv.lParam);
        ATLASSERT((p != NULL) && (p->ObjType == Object));
        //We got the object
        if(p->ObjectId == Cookie)
            {
            //Throw an event that on object was deleted...
            CCreateOrDeleteEventHelper * pHelper = new CCreateOrDeleteEventHelper;
            strcpy(pHelper->szObject, szName);
            pHelper->bCreate = FALSE;

            m_pOwner->PostMessage(WM_OBJDESTROYED, 0, (LPARAM)pHelper);

            DeleteNode(m_pOwner, hChildItem);
            //delete the interface nodes under this item
            /*
            HTREEITEM hIFace = m_pOwner->GetTreeCtrl().GetChildItem(hChildItem);
            while(hIFace != NULL)
                {

                //Todo: call the destructors for these things...
                HTREEITEM hNext = m_pOwner->GetTreeCtrl().GetNextItem(hIFace, TVGN_NEXT);
                TVITEM tvIFace;
                tvIFace.hItem = hIFace;
                tvIFace.mask = TVIF_PARAM;
                tvIFace.lParam = 0;

                InterfaceNodeHelper * p = reinterpret_cast<InterfaceNodeHelper*>(tvIFace.lParam);
                if((p!=NULL) && (p->ObjType == Interface))
                    {
                    delete p;
                    }

                m_pOwner->GetTreeCtrl().DeleteItem(hIFace);
                hIFace = hNext;
                }
            */
            break;
            }
        hChildItem = m_pOwner->GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
        }

    return S_OK;
    }

STDMETHODIMP CObjEventSink::ObjQI(/*in*/long Cookie, /*in*/REFIID riid, /*in*/ULONG newCount, /*in*/HRESULT hr)
    {
    HTREEITEM hChildItem = m_pOwner->GetTreeCtrl().GetChildItem(hItem);
    while(hChildItem != NULL)
        {
        //Okay, we know hThis.  Now we walk down till we find the object, then we find the interface and modify it.
        //Note that this is way slow; we should post a message.
        char szName[64];
        TVITEM tv;
        tv.mask = TVIF_PARAM | TVIF_TEXT;
        tv.hItem = hChildItem;
        tv.pszText = szName;
        tv.lParam = 0;
        tv.cchTextMax = 64;
        m_pOwner->GetTreeCtrl().GetItem(&tv);

        ObjectNodeHelper * p = reinterpret_cast<ObjectNodeHelper*>(tv.lParam);
        ATLASSERT(p != NULL);

        //We got the object
        if(p->ObjectId == Cookie)
            {
            //Post a message to update all views.  Then we go ahead and process the tree control  This code is _lame_
            CQIEventHelper *pm = new CQIEventHelper;
            strcpy(pm->szObject, tv.pszText);
            pm->iid = riid;//(IID*)pv;
            pm->newCount = newCount;
            pm->hr = hr;

            m_pOwner->PostMessage(WM_OBJQI, 0, (LPARAM)pm);


            //Now if we can find the interface node we deal with it here...
            HTREEITEM hIFace = m_pOwner->GetTreeCtrl().GetChildItem(hChildItem);
            while(hIFace != NULL)
                {
                tv.hItem = hIFace;
                tv.lParam = 0;
                m_pOwner->GetTreeCtrl().GetItem(&tv);

                InterfaceNodeHelper * p = reinterpret_cast<InterfaceNodeHelper*>(tv.lParam);
                ATLASSERT(p != NULL);

                if(p->m_IID == riid)
                    {
                    DoctorTreeItem(m_pOwner->GetTreeCtrl(), hIFace, 0, newCount);

                    break;
                    }
                else
                    hIFace = m_pOwner->GetTreeCtrl().GetNextItem(hIFace, TVGN_NEXT);
                }
            break;
            }
        hChildItem = m_pOwner->GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
        }

    return S_OK;
    }

STDMETHODIMP CObjEventSink::ObjRefCountChanged(/*in*/long Cookie, /*in*/REFIID riid, /*in*/ULONG oldCount, /*in*/ULONG newCount)
    {
    HTREEITEM hChildItem = m_pOwner->GetTreeCtrl().GetChildItem(hItem);
    while(hChildItem != NULL)
        {
        //Okay, we know hThis.  Now we walk down till we find the object, then we find the interface and modify it.
        //Note that this is way slow; we should post a message.
        char szName[64];
        TVITEM tv;
        tv.mask = TVIF_PARAM | TVIF_TEXT;
        tv.hItem = hChildItem;
        tv.pszText = szName;
        tv.lParam = 0;
        tv.cchTextMax = 64;
        m_pOwner->GetTreeCtrl().GetItem(&tv);

        ObjectNodeHelper * p = reinterpret_cast<ObjectNodeHelper*>(tv.lParam);
        ATLASSERT(p != NULL);

        //We got the object
        if(p->ObjectId == Cookie)
            {
            //Post a message to update all views.  Then we go ahead and process the tree control  This code is _lame_
            CRefcountEventHelper *pm = new CRefcountEventHelper;
            strcpy(pm->szObject, tv.pszText);
            pm->iid = riid;//(IID*)pv;
            pm->newCount = newCount;
            pm->oldCount = oldCount;
            pm->hr       = 0;

            m_pOwner->PostMessage(WM_OBJREFCOUNTCHANGED, 0, (LPARAM)pm);


            //Now if we can find the interface node we deal with it here...
            HTREEITEM hIFace = m_pOwner->GetTreeCtrl().GetChildItem(hChildItem);
            while(hIFace != NULL)
                {
                tv.hItem = hIFace;
                tv.lParam = 0;
                m_pOwner->GetTreeCtrl().GetItem(&tv);

                InterfaceNodeHelper * p = reinterpret_cast<InterfaceNodeHelper*>(tv.lParam);
                ATLASSERT(p != NULL);

                if(p->m_IID == riid)
                    {
                    DoctorTreeItem(m_pOwner->GetTreeCtrl(), hIFace, oldCount, newCount);

                    break;
                    }
                else
                    hIFace = m_pOwner->GetTreeCtrl().GetNextItem(hIFace, TVGN_NEXT);
                }
            break;
            }
        hChildItem = m_pOwner->GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
        }

    return S_OK;
    }



STDMETHODIMP CObjEventSink::ObjMethodCalled(/*in*/long Cookie, /*in*/REFIID riid, /*in*/long vtblIndex, /*in*/long reserved, /*in*/HRESULT hr)
    {
    HTREEITEM hChildItem = m_pOwner->GetTreeCtrl().GetChildItem(hItem);
    while(hChildItem != NULL)
        {
        TVITEM tv;
        tv.hItem = hChildItem;
        tv.mask = TVIF_PARAM;
        tv.lParam = 0;
       
        m_pOwner->GetTreeCtrl().GetItem(&tv);

        ObjectNodeHelper * p = reinterpret_cast<ObjectNodeHelper*>(tv.lParam);
        ATLASSERT(p != NULL);

        //We got the object
        if(p->ObjectId == Cookie)
            {
            char szName[64];
            TVITEM tv;
            tv.mask = TVIF_TEXT;
            tv.hItem = hChildItem;
            tv.pszText = szName;
            tv.cchTextMax = 64;

            m_pOwner->GetTreeCtrl().GetItem(&tv);

            CMethodEventHelper *pm = new CMethodEventHelper;
            strcpy(pm->szObject,tv.pszText);

            //Here I'm pissing all over constness, something which gives me
            //great satisfaction.
            //void * pv = (void*)(&riid);
            pm->iid = riid;//(IID*)pv;

            pm->nVtblIndex = vtblIndex;
            pm->hr = hr; 

            //CDocument
            m_pOwner->PostMessage(WM_OBJMETHODCALLED, 0, (LPARAM)pm);

            break;
            }

        hChildItem = m_pOwner->GetTreeCtrl().GetNextItem(hChildItem, TVGN_NEXT);
        }

    return S_OK;
    }



void CLeftView::OnDestroy() 
{
m_pAccessor->Unadvise(m_DebuggerCookie);
m_pAccessor.Release();

DeleteChildren(this, TVI_ROOT);
//DeleteNode(this, TVI_ROOT);

CTreeView::OnDestroy();
}



STDMETHODIMP CProcessEventSink::OnProcessAdded(PROCESS_INFO * pinfo)
    {return m_pOwner->OnProcessAdded(pinfo);}
STDMETHODIMP CProcessEventSink::OnProcessClosed(PROCESS_INFO * pinfo)
    {return m_pOwner->OnProcessClosed(pinfo);}
