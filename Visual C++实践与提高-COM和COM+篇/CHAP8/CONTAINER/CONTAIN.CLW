; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "contain.h"
LastPage=0

ClassCount=6
Class1=CChildFrame
Class2=CContainerApp
Class3=CAboutDlg
Class4=CContainerDoc
Class5=CContainerView
Class6=CMainFrame

ResourceCount=4
Resource1=IDR_MAINFRAME
Resource2=IDR_CONTRTYPE
Resource3=IDR_CONTRTYPE_CNTR_IP
Resource4=IDD_ABOUTBOX

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=childfrm.h
ImplementationFile=childfrm.cpp

[CLS:CContainerApp]
Type=0
BaseClass=CWinApp
HeaderFile=contain.h
ImplementationFile=contain.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=contain.cpp
ImplementationFile=contain.cpp
Filter=D

[CLS:CContainerDoc]
Type=0
BaseClass=COleDocument
HeaderFile=contrdoc.h
ImplementationFile=contrdoc.cpp

[CLS:CContainerView]
Type=0
BaseClass=CView
HeaderFile=contrvw.h
ImplementationFile=contrvw.cpp
Filter=C

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=mainfrm.h
ImplementationFile=mainfrm.cpp
Filter=T

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[TB:IDR_MAINFRAME]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_SAVE
Command3=ID_EDIT_CUT
Command4=ID_EDIT_PASTE
Command5=ID_FILE_PRINT
CommandCount=5
Class=CMainFrame

[MNU:IDR_MAINFRAME]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8
Class=CMainFrame

[MNU:IDR_CONTRTYPE]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_PASTE_SPECIAL
Command16=ID_EDIT_CLEAR
Command17=ID_OLE_INSERT_NEW
Command18=ID_OLE_EDIT_LINKS
Command19=ID_OLE_VERB_FIRST
Command20=ID_VIEW_TOOLBAR
Command21=ID_VIEW_STATUS_BAR
Command22=ID_WINDOW_NEW
Command23=ID_WINDOW_CASCADE
Command24=ID_WINDOW_TILE_HORZ
Command25=ID_WINDOW_ARRANGE
Command26=ID_APP_ABOUT
CommandCount=26
Class=CContainerView

[MNU:IDR_CONTRTYPE_CNTR_IP]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_WINDOW_NEW
Command12=ID_WINDOW_CASCADE
Command13=ID_WINDOW_TILE_HORZ
Command14=ID_WINDOW_ARRANGE
CommandCount=14
Class=CContainerView

[ACL:IDR_MAINFRAME]
Type=1
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
Command15=ID_CANCEL_EDIT_CNTR
CommandCount=15
Class=CMainFrame

[ACL:IDR_CONTRTYPE_CNTR_IP]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_CANCEL_EDIT_CNTR
CommandCount=7
Class=CContainerView

