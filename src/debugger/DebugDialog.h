// DebugDialog.h : Declaration of the CDebugDialog

#ifndef __DEBUGDIALOG_H_
#define __DEBUGDIALOG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

#define MYWM_NOTIFYICON		(WM_APP+100)


#define IDC_NOTIFY1	4578
#define CM_CLOSE 101

#include "atlcontrols.h"
#include "stdio.h"

using namespace ATLControls;


/////////////////////////////////////////////////////////////////////////////
// CDebugDialog
class CDebugDialog : 
	public CDialogImpl<CDebugDialog>
{    
HICON m_hIcon;
DWORD m_dwTimerID;
DWORD m_dwShutdownTimerID;
CImageList m_ImageList;
public:
	CDebugDialog()
    {
    m_dwTimerID = 0;
    m_dwShutdownTimerID = 0;
	}

	~CDebugDialog()
	{
	}

	enum { IDD = IDD_DEBUGDIALOG };

BEGIN_MSG_MAP(CDebugDialog)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	MESSAGE_HANDLER(MYWM_NOTIFYICON, OnIconMsg)
	MESSAGE_HANDLER(WM_DESTROY, OnDestroyDialog)
	MESSAGE_HANDLER(WM_TIMER, OnTimer)
	MESSAGE_HANDLER(WM_SIZE, OnSize)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);


    long AddItem(BSTR bstrName)
    {
    //USES_CONVERSION;
    //CListViewCtrl clb;
    //clb.Attach(GetDlgItem(IDC_LIST1));
    //return clb.InsertItem(0, OLE2T(bstrName), 0);
    return 0;
    }
    
    void RemoveItem(long item)
    {
    //CListViewCtrl clb;
    //clb.Attach(GetDlgItem(IDC_LIST1));
    //clb.DeleteItem(item);
    }

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
        //Set up our tray icon
     
      m_hIcon = (HICON)LoadImage(_Module.m_hInstResource, MAKEINTRESOURCE(IDI_ICON2),IMAGE_ICON, 16, 16, 0);
      ATLASSERT(m_hIcon != NULL);

      //Set up the tab ctrl
      CTabCtrl tab;
      tab.Attach(GetDlgItem(IDC_TAB1));
      TC_ITEM tcitem;
      tcitem.mask = TCIF_TEXT;
      tcitem.pszText = _T("General");
      tab.InsertItem(0,&tcitem);
     /*
      //Set up our list view
      m_ImageList.Create(IDB_BITMAP1, 16, 1, RGB(255,255,255));

      CListViewCtrl clb;
      clb.Attach(GetDlgItem(IDC_LIST1));
      clb.SetImageList(m_ImageList, LVSIL_SMALL);
      clb.InsertColumn(0,"Processes", LVCFMT_LEFT, 400, 0);

     */
      CenterWindow();
	  TrayMessage(NIM_ADD, IDC_NOTIFY1, m_hIcon, "COM Spy system agent" );
     
      m_dwTimerID = SetTimer(101, 5000);
    
		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
    bool bShutdown = true;

    if(_Module.GetLockCount() != 0)
        {
        char szBuf[128];
        sprintf(szBuf,"There are %d outstanding locks.  Shutting down will disconnect any open spy processes. Shut down?", _Module.GetLockCount());
        
        if(MessageBox(szBuf, "Shut down", MB_YESNO | MB_ICONEXCLAMATION) != IDYES)
            bShutdown = false;
        }

    if(bShutdown)
        PostQuitMessage(0);

	return 0;
    }

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
	ShowWindow(SW_HIDE);
    return 0;
	}

   LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
   if(wParam == SIZE_MINIMIZED)
        ShowWindow(SW_HIDE);
   else
        bHandled = FALSE;  
   return 0;
   }

    BOOL TrayMessage(DWORD dwMessage, UINT uID, HICON hIcon, LPSTR pszTip)
        {
	    BOOL res;

	    NOTIFYICONDATA tnd;

	    tnd.cbSize		= sizeof(NOTIFYICONDATA);
	    tnd.hWnd		= m_hWnd;
	    tnd.uID			= uID;

	    tnd.uFlags		= NIF_MESSAGE|NIF_ICON|NIF_TIP;
	    tnd.uCallbackMessage	= MYWM_NOTIFYICON;
	    tnd.hIcon		= hIcon;
	    if (pszTip)
    	    {
		    lstrcpyn(tnd.szTip, pszTip, sizeof(tnd.szTip));
	        }
	    else
	        {
		    tnd.szTip[0] = '\0';
	        }

	    res = Shell_NotifyIcon(dwMessage, &tnd);

	    return res;
        }


    LRESULT OnDestroyDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
        KillTimer(m_dwTimerID);
        m_ImageList.Destroy();
    	DestroyIcon(m_hIcon);
 		TrayMessage(NIM_DELETE, IDC_NOTIFY1, NULL, NULL);
        return 0;
        }

    LRESULT OnIconMsg(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
		switch (lParam)
            {
			case WM_LBUTTONDOWN:
                {
				switch (wParam)
                    {
					case IDC_NOTIFY1:
                        {
					    ShowWindow(SW_RESTORE);
					    SetForegroundWindow(m_hWnd);	// make us come to the front
                        }
                    }break;
				}
				break;
            }
         return 0;
        }

    LRESULT OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
        {
        //Every few seconds we probe the debugged processes to see if they're still out there...
        if(wParam == m_dwTimerID)
            ScanProcesses();
        else if(wParam == m_dwShutdownTimerID)
            {
            AttemptShutdown();
            KillTimer(m_dwShutdownTimerID);
            m_dwShutdownTimerID = 0;
            }

        return 0;
        }

    void BeginCountdown()
        {
        //Don't start the timer twice if this is called twice within the timeout period.
        if(m_dwShutdownTimerID == 0)
           m_dwShutdownTimerID = SetTimer(102, 5000);
        }
};

#endif //__DEBUGDIALOG_H_
