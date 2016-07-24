; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWuziqiView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "wuziqi.h"
LastPage=0

ClassCount=8
Class1=CWuziqiApp
Class2=CWuziqiDoc
Class3=CWuziqiView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_DIALOG2
Class5=CAboutDlg
Resource2=IDD_ABOUTBOX
Class6=resultwin
Resource3=IDR_MAINFRAME
Class7=askconnect
Resource4=IDD_DIALOG1
Class8=connectto
Resource5=IDD_DIALOG3

[CLS:CWuziqiApp]
Type=0
HeaderFile=wuziqi.h
ImplementationFile=wuziqi.cpp
Filter=N
LastObject=CWuziqiApp

[CLS:CWuziqiDoc]
Type=0
HeaderFile=wuziqiDoc.h
ImplementationFile=wuziqiDoc.cpp
Filter=N
LastObject=CWuziqiDoc

[CLS:CWuziqiView]
Type=0
HeaderFile=wuziqiView.h
ImplementationFile=wuziqiView.cpp
Filter=C
LastObject=CWuziqiView
BaseClass=CView
VirtualFilter=VWC


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=wuziqi.cpp
ImplementationFile=wuziqi.cpp
Filter=D
LastObject=IDOK

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
Command1=ID_FILE_OPEN
Command2=ID_FILE_SAVE
Command3=ID_APP_EXIT
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_TOP
Command6=ID_BACK_WHITE
Command7=ID_BACK_BLACK
Command8=ID_RULE_CHANGE
Command9=ID_RULE_RESTART
Command10=ID_RULE_3344
Command11=ID_RULE_CLEAN
Command12=ID_STARTSERVER
Command13=ID_CONNECT
Command14=ID_DISCONNECT
Command15=ID_APP_ABOUT
Command16=ID_MENU_HELP
CommandCount=16

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
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

[DLG:IDD_DIALOG2]
Type=1
Class=resultwin
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_WHO_WIN,static,1342308352

[CLS:resultwin]
Type=0
HeaderFile=resultwin.h
ImplementationFile=resultwin.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

[DLG:IDD_DIALOG1]
Type=1
Class=askconnect
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_PRO,static,1342308352
Control4=IDC_IP,static,1342308352
Control5=IDC_STATIC,static,1342308352

[CLS:askconnect]
Type=0
HeaderFile=askconnect.h
ImplementationFile=askconnect.cpp
BaseClass=CDialog
Filter=D
LastObject=askconnect
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=connectto
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_WHERE,edit,1350631552
Control4=IDC_STATIC,static,1342308352

[CLS:connectto]
Type=0
HeaderFile=connectto.h
ImplementationFile=connectto.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_WHERE

