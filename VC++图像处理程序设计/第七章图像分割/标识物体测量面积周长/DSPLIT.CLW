; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDynSplitView2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DSplit.h"
LastPage=0

ClassCount=11
Class1=CDSplitApp
Class2=CDSplitDoc
Class3=CDSplitView
Class4=CMainFrame

ResourceCount=8
Resource1=IDR_MAINFRAME (English (U.S.))
Class5=CAboutDlg
Class6=CDynSplitView2
Resource2=IDD_ABOUTBOX
Class7=chhhh
Resource3=IDD_LINE_DLG
Class8=CYuZhi
Resource4=IDD_ABOUTBOX (English (U.S.))
Resource5=IDD_DELSMALL_DLG
Class9=SquareDlg
Resource6=IDD_DIALOG2
Class10=DELSMALL
Resource7=IDD_SQUARE_DLG
Class11=LINEDLG
Resource8=IDR_MAINFRAME

[CLS:CDSplitApp]
Type=0
HeaderFile=DSplit.h
ImplementationFile=DSplit.cpp
Filter=N
LastObject=CDSplitApp

[CLS:CDSplitDoc]
Type=0
HeaderFile=DSplitDoc.h
ImplementationFile=DSplitDoc.cpp
Filter=N
LastObject=CDSplitDoc
BaseClass=CDocument
VirtualFilter=DC

[CLS:CDSplitView]
Type=0
HeaderFile=DSplitView.h
ImplementationFile=DSplitView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CDSplitView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_FOLLOWLINE



[CLS:CAboutDlg]
Type=0
HeaderFile=DSplit.cpp
ImplementationFile=DSplit.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILEOPEN
Command2=ID_FILESAVE
Command3=ID_APP_EXIT
Command4=ID_YUANTU
Command5=ID_A
Command6=ID_AREAJISUAN
Command7=ID_FOLLOWLINE
Command8=ID_XIAOCHUSMALL
CommandCount=8

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
Class=CMainFrame
Command1=ID_FILEOPEN
Command2=ID_YUANTU
Command3=ID_FILESAVE
CommandCount=3

[CLS:CDynSplitView2]
Type=0
HeaderFile=DynSplitView2.h
ImplementationFile=DynSplitView2.cpp
BaseClass=CView
Filter=C
LastObject=CDynSplitView2
VirtualFilter=VWC

[ACL:IDR_MAINFRAME (English (U.S.))]
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

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=5
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Control5=IDC_STATIC,static,1342308352

[CLS:chhhh]
Type=0
HeaderFile=chhhh.h
ImplementationFile=chhhh.cpp
BaseClass=CDialog
Filter=D
LastObject=chhhh

[DLG:IDD_DIALOG2]
Type=1
Class=CYuZhi
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITGray,edit,1350631553
Control5=IDC_STATIC,button,1342177287

[CLS:CYuZhi]
Type=0
HeaderFile=YuZhi.h
ImplementationFile=YuZhi.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDITGray
VirtualFilter=dWC

[DLG:IDD_SQUARE_DLG]
Type=1
Class=SquareDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631553
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_SQUARE_EDIT2,edit,1350635718
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_SQUARE_ALL,edit,1350631553

[CLS:SquareDlg]
Type=0
HeaderFile=SquareDlg.h
ImplementationFile=SquareDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ID_APP_EXIT

[DLG:IDD_DELSMALL_DLG]
Type=1
Class=DELSMALL
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITDEL,edit,1350631553
Control5=IDC_STATIC,button,1342177287

[CLS:DELSMALL]
Type=0
HeaderFile=DELSMALL.h
ImplementationFile=DELSMALL.cpp
BaseClass=CDialog
Filter=D
LastObject=DELSMALL
VirtualFilter=dWC

[DLG:IDD_LINE_DLG]
Type=1
Class=LINEDLG
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631553
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,button,1342177287
Control6=IDC_LINE_EDIT2,edit,1350635718
Control7=IDC_STATIC,button,1342177287
Control8=IDC_STATIC,static,1342308352
Control9=IDC_LINE_ALL,edit,1350631553

[CLS:LINEDLG]
Type=0
HeaderFile=LINEDLG.h
ImplementationFile=LINEDLG.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

