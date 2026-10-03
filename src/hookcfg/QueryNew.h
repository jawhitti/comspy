// QueryNew.h : Declaration of the CQueryNew

#ifndef __QUERYNEW_H_
#define __QUERYNEW_H_

#include "resource.h"       // main symbols
#include <atlhost.h>

/////////////////////////////////////////////////////////////////////////////
// CQueryNew
class CQueryNew : 
	public CDialogImpl<CQueryNew>
{
public:
    TCHAR m_szGUID[256];

	CQueryNew()
	{
    m_szGUID[0] = 0;
	}

	~CQueryNew()
	{
	}

	enum { IDD = IDD_QUERYNEW };

BEGIN_MSG_MAP(CQueryNew)
	MESSAGE_HANDLER(WM_INITDIALOG, OnInitDialog)
	COMMAND_ID_HANDLER(IDOK, OnOK)
	COMMAND_ID_HANDLER(IDCANCEL, OnCancel)
END_MSG_MAP()
// Handler prototypes:
//  LRESULT MessageHandler(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
//  LRESULT CommandHandler(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled);
//  LRESULT NotifyHandler(int idCtrl, LPNMHDR pnmh, BOOL& bHandled);

	LRESULT OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
	{
		return 1;  // Let the system set the focus
	}

	LRESULT OnOK(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
        USES_CONVERSION;

        TCHAR szEdit[256];
        GetDlgItemText(IDC_EDIT1,szEdit, 256);
        CLSID clsid;
                
        if(::CLSIDFromString(T2OLE(szEdit), &clsid) == S_OK)
            {
            LPOLESTR wszGuid;
            ::StringFromCLSID(clsid, &wszGuid);
            lstrcpy(m_szGUID, OLE2T(wszGuid));
            ::CoTaskMemFree(wszGuid);
            }

		EndDialog(wID);
		return 0;
	}

	LRESULT OnCancel(WORD wNotifyCode, WORD wID, HWND hWndCtl, BOOL& bHandled)
	{
		EndDialog(wID);
		return 0;
	}
};

#endif //__QUERYNEW_H_
