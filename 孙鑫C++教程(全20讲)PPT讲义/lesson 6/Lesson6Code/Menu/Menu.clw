; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMenuView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Menu.h"
LastPage=0

ClassCount=5
Class1=CMenuApp
Class2=CMenuDoc
Class3=CMenuView
Class4=CMainFrame

ResourceCount=4
Resource1=IDR_MENU1
Class5=CAboutDlg
Resource2=IDR_MAINFRAME
Resource3=CG_IDR_POPUP_MENU_VIEW
Resource4=IDD_ABOUTBOX

[CLS:CMenuApp]
Type=0
HeaderFile=Menu.h
ImplementationFile=Menu.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CMenuDoc]
Type=0
HeaderFile=MenuDoc.h
ImplementationFile=MenuDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC

[CLS:CMenuView]
Type=0
HeaderFile=MenuView.h
ImplementationFile=MenuView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=IDM_SHOW


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=IDM_SHOW




[CLS:CAboutDlg]
Type=0
HeaderFile=Menu.cpp
ImplementationFile=Menu.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
Command17=IDM_TEST
CommandCount=17

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:CG_IDR_POPUP_MENU_VIEW]
Type=1
Class=?
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
CommandCount=3

[MNU:IDR_MENU1]
Type=1
Class=?
Command1=IDM_SHOW
Command2=IDM_EXIT
CommandCount=2

