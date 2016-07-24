; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNum
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DSplit.h"
LastPage=0

ClassCount=9
Class1=CDSplitApp
Class2=CDSplitDoc
Class3=CDSplitView
Class4=CMainFrame

ResourceCount=5
Resource1=IDR_MAINFRAME (English (U.S.))
Class5=CAboutDlg
Class6=CDynSplitView2
Resource2=IDD_ABOUTBOX
Class7=chhhh
Resource3=IDD_DIALOG3
Class8=CYuZhi
Resource4=IDD_ABOUTBOX (English (U.S.))
Class9=CNum
Resource5=IDR_MAINFRAME

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
LastObject=ID_FILEOPEN
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
LastObject=ID_LOWFILTER



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
Command5=ID_HANGBIANHUAN
Command6=ID_LIEBIANHUAN
Command7=ID_ONCE
Command8=ID_TWICE
Command9=ID_THRICE
Command10=ID_LOWFILTER
Command11=ID_HIGHFILTER
Command12=ID_IDWT
CommandCount=12

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

[CLS:CYuZhi]
Type=0
HeaderFile=YuZhi.h
ImplementationFile=YuZhi.cpp
BaseClass=CDialog
Filter=D
LastObject=CYuZhi

[DLG:IDD_DIALOG3]
Type=1
Class=CNum
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552

[CLS:CNum]
Type=0
HeaderFile=Num.h
ImplementationFile=Num.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CNum

