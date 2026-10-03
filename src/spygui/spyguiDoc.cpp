// spyguiDoc.cpp : implementation of the CSpyguiDoc class
//

#include "stdafx.h"
#include "spygui.h"

#include "spyguiDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSpyguiDoc

IMPLEMENT_DYNCREATE(CSpyguiDoc, CDocument)

BEGIN_MESSAGE_MAP(CSpyguiDoc, CDocument)
	//{{AFX_MSG_MAP(CSpyguiDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSpyguiDoc construction/destruction

CSpyguiDoc::CSpyguiDoc()
{
	// TODO: add one-time construction code here

}

CSpyguiDoc::~CSpyguiDoc()
{
}

BOOL CSpyguiDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSpyguiDoc serialization

void CSpyguiDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSpyguiDoc diagnostics

#ifdef _DEBUG
void CSpyguiDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CSpyguiDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSpyguiDoc commands
