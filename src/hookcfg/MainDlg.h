// MainDlg.h : Declaration of the CMainDlg

#ifndef __MAINDLG_H_
#define __MAINDLG_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

#include <atlcontrols.h>

#include <stdio.h>

using namespace ATLControls;

#define UM_FINDEM (WM_USER+4)
#define UM_PROCESSNEXT (WM_USER+5)


inline void ReportRegistryError(HWND hWndParent, LONG err, LONG line)
{
TCHAR buf[256];
char msgbuf[256];
long len = ::FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, err, 0, buf, sizeof(buf), 0);

if(len != 0)
    {
    sprintf(msgbuf,"Registry access failed at line %d: %s", line,  buf);
    }
else
    {
    sprintf(msgbuf,"Registry access failed at line %d, Error %d", line,  err);
    }
MessageBox(hWndParent, msgbuf,"Error", MB_OK);
}

#define CHECKREG(exp) if(exp != ERROR_SUCCESS){ReportRegistryError(m_hWnd, exp, __LINE__);}

/////////////////////////////////////////////////////////////////////////////
// CMainDlg
class CMainDlg : 
	public CDialogImpl<CMainDlg>
{
HKEY m_hkClsid; //HKEY_CLASSES_ROOT/CLSID
DWORD m_CurrentKey;     //current subkey.
DWORD m_TotalKeys;     //current subkey.

UINT m_TimerID;  //lame-o hack to get the GUI up.

BOOL m_Initialized;

public:
	CMainDlg()
	{
    m_Initialized = FALSE;
    m_hkClsid = NULL;
    m_CurrentKey = 0;
	}

	~CMainDlg()
	{
    if(m_hkClsid != NULL) ::RegCloseKey(m_hkClsid);
	}

	enum { IDD = IDD_MAINDLG };

//DECLARE_WND_CLASS("hookcfgwin")

BEGIN_MSG_MAP(CMainDlg)
	MESSAGE_HANDLER(UM_FINDEM, FindHookedItems)
	MESSAGE_HANDLER(WM_TIMER, StartTheShow)
    MESSAGE_HANDLER(UM_PROCESSNEXT, ProcessNextKey)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
	COMMAND_ID_HANDLER(IDC_BUTTON1, OnClearAll)
	COMMAND_ID_HANDLER(IDC_PREVIOUS, OnFindPrevious)
	COMMAND_ID_HANDLER(IDC_NEXT, OnFindNext)
	NOTIFY_HANDLER(IDC_LIST1, LVN_ITEMCHANGED, OnItemchangedList1)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
    //We want a real icon.  This is the only way I see to do it...
    HICON h = LoadIcon(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON1));
    HICON hSmall = (HICON)::LoadImage(_Module.GetResourceInstance(), MAKEINTRESOURCE(IDI_ICON1), IMAGE_ICON, 16, 16, LR_DEFAULTCOLOR);

    ::SetClassLong(m_hWnd, GCL_HICON, (long)h);
    ::SetClassLong(m_hWnd, GCL_HICONSM, (long)hSmall);

    //GCL_HICON Replaces a handle to the icon associated with the class. 
    //GCL_HICONSM Replace a handle to the small icon associated with the class. 
    CenterWindow();

    //Here we paint the controls and then post a message to fill them in....

    CListViewCtrl clvw;
    clvw.Attach(GetDlgItem(IDC_LIST1));
    LV_COLUMN lvclm;
    lvclm.mask = LVCF_TEXT | LVCF_WIDTH;
    lvclm.pszText = "Class";
    lvclm.cx = 150;
    clvw.InsertColumn(0, &lvclm);

    lvclm.pszText = "ProgId";
    clvw.InsertColumn(1, &lvclm);

    lvclm.pszText = "CLSID";
    lvclm.cx=300;
    clvw.InsertColumn(2, &lvclm);

    lvclm.pszText = "Path";
    clvw.InsertColumn(3, &lvclm);


    DWORD styles = LVS_EX_FULLROWSELECT | LVS_EX_CHECKBOXES;
    clvw.SetExtendedListViewStyle(styles, styles);



    //To get the gui up quickly I post a timer notification, and don't start
    //actually computing until the timer goes off.
    m_TimerID = SetTimer(101, 100);


		return 1;  // Let the system set the focus
	}


    LRESULT StartTheShow(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
    {
    //The timer went off.  Let's get this show on the road.
    if(!m_Initialized)
        {
        KillTimer(m_TimerID);
        PostMessage(UM_FINDEM, 0);
        }
    return 0;
    }

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		//EndDialog(wID);
        PostQuitMessage(0);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		//EndDialog(wID);
        PostQuitMessage(0);
		return 0;
	}

	LRESULT OnClearAll(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
    CListViewCtrl clvw;
    clvw.Attach(GetDlgItem(IDC_LIST1));

    long count = clvw.GetItemCount();
    for(int i=0; i< count; i++)
        {
        if(clvw.GetCheckState(i))
            {
            TCHAR szClsid[128];
            clvw.GetItemText(i,2,szClsid,128);

            OLECHAR szwClsid[64];    
            CLSID clsid;
            ::mbstowcs(szwClsid, szClsid, 64);
            ::CLSIDFromString(szwClsid, &clsid);

            UnhookClass(clsid);

            clvw.SetCheckState(i, FALSE);
            }
        }
    return 0;
    }

	LRESULT OnFindPrevious(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
    CListViewCtrl clvw;
    clvw.Attach(GetDlgItem(IDC_LIST1));
    bool bfound = false;

    int start = clvw.GetSelectedIndex()-1;
    if(start > 2)
        {
        long count = clvw.GetItemCount();
        for(int i=start; i>=0; i--)
            {
            if(clvw.GetCheckState(i))
                {
                clvw.EnsureVisible(i, FALSE);
                bfound = true;

                //Set the focus to this one...
                clvw.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED); 

                break;
                }
            }
        }   
    if(!bfound) MessageBeep(0);

    return 0;
    }

	LRESULT OnFindNext(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
    {
    CListViewCtrl clvw;
    clvw.Attach(GetDlgItem(IDC_LIST1));
    bool bfound = false;

    int start = clvw.GetSelectedIndex()+1;

    long count = clvw.GetItemCount();
    if(start < count - 1)
        {
        for(int i=start; i< count; i++)
            {
            if(clvw.GetCheckState(i))
                {
                clvw.EnsureVisible(i, FALSE);
                bfound = true;

                //Set the focus to this one...
                clvw.SetItemState(i,LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED); 

                break;
                }
            }
        }   
    if(!bfound) MessageBeep(0);
    return 0;
    }

   void AddInfoToDialog(LPTSTR szClsid, HKEY hkCLSID, HKEY hkInProcServer32, BOOL bcheckstate)
       {
       CListViewCtrl clvw;
       clvw.Attach(GetDlgItem(IDC_LIST1));
       //long idx = clvw.GetItemCount();

        //We sort based on ProgId
        TCHAR szBuf[1024];
        DWORD dwSize = 1024;
        LONG res;
        LVITEM lvitem;
        lvitem.mask = LVIF_TEXT;
        lvitem.iItem = clvw.GetItemCount();

        //if there's a name on the guid then we stick that in...
        dwSize = 1024;
        szBuf[0] = 0;
        res = RegQueryValueEx(hkCLSID,"",0,0,(BYTE*)szBuf, &dwSize);
        //CHECKREG(res);

        lvitem.iSubItem = 0;
        lvitem.pszText = (strlen(szBuf)==0)? szClsid : szBuf;
        lvitem.iItem = clvw.InsertItem(&lvitem);
        clvw.SetCheckState(lvitem.iItem, bcheckstate);


       //Now put the ProgId in...
       HKEY hkProgId;
       res = RegOpenKeyEx(hkCLSID,"ProgID", 0,KEY_READ,  &hkProgId);
       if(res == ERROR_SUCCESS)
            {
            dwSize = 1024;
            RegQueryValueEx(hkProgId,"",0,0,(BYTE*)szBuf, &dwSize);
            ::RegCloseKey(hkProgId);

            ATLASSERT(strlen(szBuf) > 0);
            lvitem.pszText = szBuf;
            lvitem.iSubItem=1;
            clvw.SetItem(&lvitem);
            }

        //now put the CLSID in...
        lvitem.pszText = szClsid;
        lvitem.iSubItem=2;
        clvw.SetItem(&lvitem);

        //Now put the path...We expect failure here...
        dwSize=1024;
        res = ::RegQueryValueEx(hkInProcServer32,"HookSavedPath",0,NULL,(LPBYTE)szBuf,&dwSize);
        if(res != ERROR_SUCCESS)
            {
            dwSize = 1024;
            res = ::RegQueryValueEx(hkInProcServer32,"",0,NULL,(LPBYTE)szBuf,&dwSize);
            //CHECKREG(res);
            if((res != ERROR_SUCCESS)|| (strlen(szBuf) == 0))
                {
                //This is troubling. It means that the key is there, but the value is not set.  Yikes!
                strcpy(szBuf,"(???)");
                }
            }
        ATLASSERT(strlen(szBuf) > 0);
        lvitem.pszText = szBuf;
        lvitem.iSubItem=3;
        clvw.SetItem(&lvitem);

        }


   LRESULT FindHookedItems(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
   if(::RegOpenKeyEx(HKEY_CLASSES_ROOT,"CLSID",0,  KEY_READ, &m_hkClsid) == ERROR_SUCCESS)
        {
        if(RegQueryInfoKey(m_hkClsid,0,0,0,&m_TotalKeys,NULL,NULL,NULL,NULL,NULL,NULL,NULL) == ERROR_SUCCESS)
            {
            m_CurrentKey = 1;
            PostMessage(UM_PROCESSNEXT,0);
            }
        else
            {
            SetStatus("Unable to open registry");
            }
        }
   else
        {
        SetStatus("Unable to open registry");
        }

   return 0;
   }

   LRESULT ProcessNextKey(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
   {
   TCHAR szsubkey[128];
   DWORD dwIndex=0;
   DWORD dwLength = 64;
   LONG result = ::RegEnumKeyEx(m_hkClsid, m_CurrentKey++,szsubkey,&dwLength,0,NULL,NULL,NULL);

   if(result == ERROR_SUCCESS)
        {
        //Try for inprocserver32 first, so that we don't bother opening
        //keys for EXE servers.
        TCHAR szInProcServer32[128];
        sprintf(szInProcServer32, "%s\\InprocServer32",szsubkey);
        HKEY hkips;

        BOOL bfoundkey = true;
        if(::RegOpenKey(m_hkClsid,szInProcServer32, &hkips) != ERROR_SUCCESS)
            {
            sprintf(szInProcServer32, "%s\\InprocServer32", szsubkey);
            if(::RegOpenKey(m_hkClsid,szInProcServer32, &hkips) != ERROR_SUCCESS)
                bfoundkey=false;
            }
        //we skip anyone who doesn't have inprocserver32
        if(bfoundkey)
            {
            TCHAR szValue[1024]; 
            DWORD dwBytes = 1024;
            
            HKEY hkObjCLSID;
            BOOL bCheckState = FALSE;

            //TODO: make sure that inprocserver32 points to the right thing; the presence of
            //hooksavedpath is not enough, since we leave that littered around.  Better yet,
            //clean up after ourselves a little better.
            //FIXED: We do clean up after ourselves.
            if(::RegQueryValueEx(hkips,"HookSavedPath",0,NULL, (BYTE*)&szValue, &dwBytes) == ERROR_SUCCESS)
                   bCheckState = TRUE;

            if(::RegOpenKey(m_hkClsid,szsubkey, &hkObjCLSID) == ERROR_SUCCESS)
                {                
                AddInfoToDialog(szsubkey, hkObjCLSID, hkips, bCheckState);
                ::RegCloseKey(hkObjCLSID);
                }
            ::RegCloseKey(hkips);
            }

        PostMessage(UM_PROCESSNEXT,0);
        }

   else if(result == ERROR_NO_MORE_ITEMS)
        {
        //We're all done
        OnProcessingComplete();
        }

   return 0;
   }

    void OnProcessingComplete()
        {
        m_Initialized = TRUE;
        ::RegCloseKey(m_hkClsid);
        m_hkClsid = NULL;

        SetDlgItemText(IDC_STATUS,"");

        //Set the first item to be selected.
        CListViewCtrl clvw;
        clvw.Attach(GetDlgItem(IDC_LIST1));

        clvw.SetItemState(0, LVIS_SELECTED | LVIS_FOCUSED, LVIS_SELECTED | LVIS_FOCUSED);
        }


    void SetStatus(LPTSTR szMsg)
        {
        SetDlgItemText(IDC_STATUS, szMsg);
        HWND hw = GetDlgItem(IDC_STATUS);
        ::InvalidateRect(hw,NULL,TRUE);
        MSG msg;
        while(::PeekMessage(&msg, hw, 0,0,PM_REMOVE))
            ::DispatchMessage(&msg);
        }

    void HookCoclass(LPTSTR pszHookPath, LPTSTR pszGuid)
        {
        ATLASSERT(m_Initialized);

        TCHAR szRegKey[256];
        TCHAR szPath[1024];
        //Get the InprocServer32 path for this clsid
        sprintf(szRegKey, "CLSID\\%s\\InprocServer32", pszGuid);

        HKEY hk;
        LONG res;

        //make sure it isn't already hooked...
        res = ::RegOpenKeyEx(HKEY_CLASSES_ROOT, szRegKey, 0, KEY_READ | KEY_SET_VALUE, &hk);
        CHECKREG(res);
        
        if(res == ERROR_SUCCESS)
            {
            DWORD dwSize=1024;
            res = ::RegQueryValueEx(hk,"HookSavedPath", 0,0,(LPBYTE)szPath, &dwSize);
            //CHECKREG(res);
            //If we get ERROR_SUCCESS then we've already hooked, so we do nothing. Otherwise...
            if(res != ERROR_SUCCESS)
                {
                LONG dwSize=1024;
                res = ::RegQueryValue(hk,"", szPath, &dwSize);
                if(res == ERROR_SUCCESS)
                    {
                    //swap out the values.
                    ::RegSetValueEx(hk,"HookSavedPath",0,REG_SZ,(BYTE*)szPath, dwSize);
                    ::RegSetValueEx(hk,"",0,REG_SZ,(BYTE*)pszHookPath, lstrlen(pszHookPath)+1);
                    }

                ::RegCloseKey(hk);
                }
            }
        }

	void UnhookClass(CLSID clsid)
        {
        ATLASSERT(m_Initialized);

        USES_CONVERSION;

        LPOLESTR wszGuid;
        ::StringFromCLSID(clsid, &wszGuid);

        TCHAR szRegKey[256];
        TCHAR szPath[1024];
        //Get the InprocServer32 path for this clsid
        sprintf(szRegKey, "CLSID\\%s\\InprocServer32", OLE2T(wszGuid));

        HKEY hk;

        //make sure it isn't already hooked...
        if(::RegOpenKeyEx(HKEY_CLASSES_ROOT, szRegKey, 0, KEY_READ | KEY_SET_VALUE, &hk) == ERROR_SUCCESS)
            {
            DWORD dwSize=1024;
            if(::RegQueryValueEx(hk,"HookSavedPath", 0,0,(LPBYTE)szPath, &dwSize) == ERROR_SUCCESS)
                {
                LONG dwSize=1024;
                ::RegSetValueEx(hk,"",0,REG_SZ,(BYTE*)szPath, lstrlen(szPath)+1);
                ::RegDeleteValue(hk, "HookSavedPath");
                }
             ::RegCloseKey(hk);
             }
        else
            MessageBox("Error accessing key","Error", MB_OK);


        ::CoTaskMemFree(wszGuid);
        }


	void HookClass(CLSID clsid)
	    {
        USES_CONVERSION;

        LPOLESTR pszCLSID;
        HRESULT hr = ::StringFromCLSID(clsid, &pszCLSID);
        if(SUCCEEDED(hr))
            {
            //First we need to find where the hook is installed...
            TCHAR szHookPath[1024];
            HKEY hk;
    
            if(::RegOpenKey(HKEY_CLASSES_ROOT, "CLSID\\{562BFEB0-3310-11D3-8BF2-00105A6DC077}\\InprocServer32", &hk) == ERROR_SUCCESS)
                {
                LONG dwSize = sizeof(szHookPath);
                if(::RegQueryValue(hk,NULL, szHookPath, &dwSize) == ERROR_SUCCESS)
                    {
                    //Once we know where the hook is installed we can do our thing...
                    HookCoclass(szHookPath, OLE2T(pszCLSID));
                    }
                else
                    {
                    MessageBox("The Hook Spy doesn't appear to be installed", "Error");
                    }
                ::RegCloseKey(hk);
                }
            ::CoTaskMemFree(pszCLSID);
	        }
         else
            {
            MessageBox("unable to open hookspy key","Internal error 1", MB_OK);
            }
         }

        //If the user checks or unchecks an item in the list box we deal with it here.
	    LRESULT OnItemchangedList1(int idCtrl, LPNMHDR pnmh, BOOL& bHandled)
	        {
            //ignore these messages while we set up.
            if(!m_Initialized) return 0;

            LPNMLISTVIEW pnmlv = (LPNMLISTVIEW)pnmh;        
            DWORD dwCurrent = (pnmlv->uNewState >> 12) - 1;
            DWORD dwOld = (pnmlv->uOldState >> 12) - 1;

            if(dwCurrent != dwOld)
                {
                USES_CONVERSION;
                CListViewCtrl clvw;
                clvw.Attach(GetDlgItem(IDC_LIST1));

                TCHAR szClsid[128];
                clvw.GetItemText(pnmlv->iItem,2,szClsid,128);
        
                CLSID clsid;
                ::CLSIDFromString(T2OLE(szClsid), &clsid);

                //LPOLESTR pszCLSID;
                //if(FAILED(::ProgIDFromCLSID(clsid, &pszCLSID)))
                //::StringFromCLSID(clsid, &pszCLSID);

                if(dwCurrent)
                    {
                    ATLTRACE("Hooking class %s\n", szClsid);
                    HookClass(clsid);
                    }
                else 
                    {
                    ATLTRACE("Unhooking class %s\n", szClsid);
                    UnhookClass(clsid);
                    }

                //::CoTaskMemFree(pszCLSID);
                }
	        return 0;
	        }
    };

#endif //__MAINDLG_H_
