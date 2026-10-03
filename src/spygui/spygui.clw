; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "spygui.h"
LastPage=0

ClassCount=6
Class1=CLeftView
Class2=CMainFrame
Class3=CSpyguiApp
Class4=CAboutDlg
Class5=CSpyguiDoc
Class6=CSpyguiView

ResourceCount=2
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX

[CLS:CLeftView]
Type=0
BaseClass=CTreeView
HeaderFile=LeftView.h
ImplementationFile=LeftView.cpp
Filter=C
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CSpyguiApp]
Type=0
BaseClass=CWinApp
HeaderFile=spygui.h
ImplementationFile=spygui.cpp
LastObject=CSpyguiApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=spygui.cpp
ImplementationFile=spygui.cpp
LastObject=CAboutDlg
Filter=D
VirtualFilter=dWC

[CLS:CSpyguiDoc]
Type=0
BaseClass=CDocument
HeaderFile=spyguiDoc.h
ImplementationFile=spyguiDoc.cpp

[CLS:CSpyguiView]
Type=0
BaseClass=CListView
HeaderFile=spyguiView.h
ImplementationFile=spyguiView.cpp
Filter=C
VirtualFilter=VWC
LastObject=ID_FILE_CLEAR

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=7
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LINK,static,1342373889
Control5=IDOK,button,1342373889
Control6=IDC_STATIC,button,1342177287
Control7=IDC_TAB1,SysTabControl32,1342177280

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_CLEAR
Command2=ID_APP_EXIT
Command3=ID_APP_ABOUT
CommandCount=3

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

