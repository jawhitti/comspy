// AHyperLinkControlImpl.h: interface for the AHyperLinkControlImpl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_AHYPERLINKCONTROLIMPL_H__79CB5086_15B6_11D2_AD60_000000000000__INCLUDED_)
#define AFX_AHYPERLINKCONTROLIMPL_H__79CB5086_15B6_11D2_AD60_000000000000__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include <atlwin.h>

// Very basic hyper link control

namespace PTL {

class CPtlHyperLinkControl : public CWindowImpl<CPtlHyperLinkControl>
                    
{
public:


   typedef CWindowImpl<CPtlHyperLinkControl> base;

	CPtlHyperLinkControl()
   {
      m_szURL[0] = 0;
   }

   void UnderlineFont()
   {
      HFONT font;

      font = GetFont();
      if (font)
      {
         LOGFONT lf;
         GetObject(font, sizeof(LOGFONT), &lf);
         lf.lfUnderline = TRUE;
         lf.lfWeight = 800;

         font = CreateFontIndirect(&lf);
         SetFont(font);
      }
      LoadCursor();
   }

   void LoadCursor()
   {
      char szPath[_MAX_PATH+1];
      GetWindowsDirectory(szPath, MAX_PATH);

      lstrcat( szPath, _T("\\winhlp32.exe"));

      HMODULE hModule = LoadLibrary(szPath);
      if (hModule) 
      {
         HCURSOR hHandCursor = ::LoadCursor(hModule, MAKEINTRESOURCE(106));
         if (hHandCursor)
            m_hCursor = CopyCursor(hHandCursor);
      }
      FreeLibrary(hModule);
   }

	void SetURL(TCHAR* psz)
   {
      ATLASSERT(psz);
      lstrcpy(m_szURL, psz);
   }

	void Navigate()
   {
      ShellExecute(0, _T("open"), m_szURL, 0, 0, SW_SHOWNORMAL);
   }
   

    //+-----------------------------------------------------------------------------
    //+
    //+ Message map                                           
    //+
    //+-----------------------------------------------------------------------------

    BEGIN_MSG_MAP(CPtlHyperLinkControl)
        MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
        MESSAGE_HANDLER(WM_SETCURSOR, OnSetCursor)
    END_MSG_MAP()   

    //+-----------------------------------------------------------------------------
    //+
    //+ Purpose: Message map handlers                         
    //+
    //+-----------------------------------------------------------------------------

   LRESULT OnLButtonDown(UINT nMsg, WPARAM wParam, 
                 LPARAM lParam, BOOL& bHandled)   
   {
      Navigate();
      bHandled = TRUE;
      return 0;
   }
   
   LRESULT OnSetCursor(UINT nMsg, WPARAM wParam, 
                  LPARAM lParam, BOOL& bHandled)
   {
      SetCursor(m_hCursor);
      bHandled = TRUE;
      return 0;
   }

protected:
    TCHAR m_szURL[256];
    HCURSOR m_hCursor;
};

};
#endif // !defined(AFX_AHYPERLINKCONTROLIMPL_H__79CB5086_15B6_11D2_AD60_000000000000__INCLUDED_)
