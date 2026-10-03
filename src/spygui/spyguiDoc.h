// spyguiDoc.h : interface of the CSpyguiDoc class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPYGUIDOC_H__C532F738_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_)
#define AFX_SPYGUIDOC_H__C532F738_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <map>


////////////////////////////////////////////////////////////////////
//Easier to go with the flow in MFC than to try to fight it.  I'll store
//all of the interesting data here. LeftView will do the navigation, CSpyguiView
//will handle displaying the messages.

class CSpyguiDoc : public CDocument
{
protected: // create from serialization only
	CSpyguiDoc();
	DECLARE_DYNCREATE(CSpyguiDoc)

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpyguiDoc)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpyguiDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSpyguiDoc)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////
//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPYGUIDOC_H__C532F738_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_)
