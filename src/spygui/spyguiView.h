// spyguiView.h : interface of the CSpyguiView class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_SPYGUIVIEW_H__C532F73A_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_)
#define AFX_SPYGUIVIEW_H__C532F73A_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CSpyguiView : public CListView
{
protected: // create from serialization only
	CSpyguiView();
	DECLARE_DYNCREATE(CSpyguiView)

// Attributes
public:
	CSpyguiDoc* GetDocument();
    CImageList m_ImageList;
// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSpyguiView)
	public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSpyguiView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSpyguiView)
	afx_msg void OnFileClear();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in spyguiView.cpp
inline CSpyguiDoc* CSpyguiView::GetDocument()
   { return (CSpyguiDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SPYGUIVIEW_H__C532F73A_3FCB_11D3_8BFA_00105A6DC077__INCLUDED_)
