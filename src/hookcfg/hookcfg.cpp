// hookcfg.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#include "MainDlg.h"

CComModule _Module;

BEGIN_OBJECT_MAP(ObjMap)
END_OBJECT_MAP()

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
_Module.Init(ObjMap, hInstance);

::INITCOMMONCONTROLSEX icc;

icc.dwSize = sizeof(INITCOMMONCONTROLSEX);
icc.dwICC = ICC_LISTVIEW_CLASSES;
::InitCommonControlsEx(&icc);

CMainDlg dlg;
//dlg.DoModal();

dlg.Create(::GetActiveWindow());
dlg.ShowWindow(SW_SHOW);

MSG msg;
while(::GetMessage(&msg,0,0,0))
    {
    ::TranslateMessage(&msg);
    ::DispatchMessage(&msg);
    }

dlg.DestroyWindow();

_Module.Term();

	return 0;
}



