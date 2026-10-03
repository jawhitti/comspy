// spyguiView.cpp : implementation of the CSpyguiView class
//

#include "stdafx.h"
#include "spygui.h"

#include "spyguiDoc.h"
#include "spyguiView.h"

#include "leftview.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSpyguiView

IMPLEMENT_DYNCREATE(CSpyguiView, CListView)

BEGIN_MESSAGE_MAP(CSpyguiView, CListView)
	//{{AFX_MSG_MAP(CSpyguiView)
	ON_COMMAND(ID_FILE_CLEAR, OnFileClear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpyguiView construction/destruction

CSpyguiView::CSpyguiView()
{
	// TODO: add construction code here

}

CSpyguiView::~CSpyguiView()
{
}

BOOL CSpyguiView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

    cs.style |= (LVS_REPORT | LVS_SHOWSELALWAYS);

	return CListView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CSpyguiView drawing

void CSpyguiView::OnDraw(CDC* pDC)
{
	CSpyguiDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	CListCtrl& refCtrl = GetListCtrl();
	refCtrl.InsertItem(0, "Item!");
	// TODO: add draw code for native data here
}

void CSpyguiView::OnInitialUpdate()
{
	CListView::OnInitialUpdate();

	// TODO: You may populate your ListView with items by directly accessing
	//  its list control through a call to GetListCtrl().

    GetListCtrl().InsertColumn(0,"Object", LVCFMT_LEFT, 200);
    GetListCtrl().InsertColumn(1,"Method", LVCFMT_LEFT, 200);
    GetListCtrl().InsertColumn(2,"Result", LVCFMT_LEFT, 200);

    m_ImageList.Create(IDB_BITMAP2,16,1,RGB(255,255,255));
    GetListCtrl().SetImageList(&m_ImageList, LVSIL_SMALL);

    GetListCtrl().SetExtendedStyle(LVS_EX_FULLROWSELECT);
}

/////////////////////////////////////////////////////////////////////////////
// CSpyguiView diagnostics

#ifdef _DEBUG
void CSpyguiView::AssertValid() const
{
//UpdateAllViews
	CListView::AssertValid();
}

void CSpyguiView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSpyguiDoc* CSpyguiView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSpyguiDoc)));
	return (CSpyguiDoc*)m_pDocument;
}
#endif //_DEBUG


void CSpyguiView::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
static long count = 0;
if(lHint != 0)
    {
    CEventHelper * pEvent = reinterpret_cast<CEventHelper*>(pHint);

    pEvent->Render(GetListCtrl());

    delete pEvent;

    count += 1;
    }

}

void CSpyguiView::OnFileClear() 
{
GetListCtrl().DeleteAllItems();
}
