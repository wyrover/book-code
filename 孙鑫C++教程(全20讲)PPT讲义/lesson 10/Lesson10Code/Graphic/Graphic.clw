; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSettingDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "graphic.h"
LastPage=0

ClassCount=7
Class1=CGraphicApp
Class2=CAboutDlg
Class3=CGraphicDoc
Class4=CGraphicView
Class5=CMainFrame
Class6=CSettingDlg
Class7=CTestBtn

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DLG_SETTING

[CLS:CGraphicApp]
Type=0
BaseClass=CWinApp
HeaderFile=Graphic.h
ImplementationFile=Graphic.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Graphic.cpp
ImplementationFile=Graphic.cpp
LastObject=CAboutDlg

[CLS:CGraphicDoc]
Type=0
BaseClass=CDocument
HeaderFile=GraphicDoc.h
ImplementationFile=GraphicDoc.cpp

[CLS:CGraphicView]
Type=0
BaseClass=CView
HeaderFile=GraphicView.h
ImplementationFile=GraphicView.cpp
Filter=C
VirtualFilter=VWC

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CSettingDlg]
Type=0
BaseClass=CDialog
HeaderFile=SettingDlg.h
ImplementationFile=SettingDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDOK

[CLS:CTestBtn]
Type=0
BaseClass=CButton
HeaderFile=TestBtn.h
ImplementationFile=TestBtn.cpp
Filter=W
VirtualFilter=BWC

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DLG_SETTING]
Type=1
Class=CSettingDlg
ControlCount=11
Control1=IDOK,button,1342242827
Control2=IDCANCEL,button,1342242827
Control3=IDC_STATIC,static,1342308352
Control4=IDC_LINE_WIDTH,edit,1350631552
Control5=IDC_LINE_STYLE,button,1342177287
Control6=IDC_RADIO1,button,1342308361
Control7=IDC_RADIO2,button,1342177289
Control8=IDC_RADIO3,button,1342177289
Control9=IDC_SAMPLE,button,1342177287
Control10=IDC_TEXT,static,1342308352
Control11=IDC_BTN_ST,button,1342242816

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

[MNU:IDR_MAINFRAME]
Type=1
Class=?
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
Command17=IDM_DOT
Command18=IDM_LINE
Command19=IDM_RECTANGLE
Command20=IDM_ELLIPSE
Command21=IDM_SETTING
Command22=IDM_COLOR
Command23=IDM_FONT
CommandCount=23

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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

