; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDynSplitView2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DSplit.h"
LastPage=0

ClassCount=10
Class1=CDSplitApp
Class2=CDSplitDoc
Class3=CDSplitView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_ABOUTBOX (English (U.S.))
Class5=CAboutDlg
Class6=CDynSplitView2
Resource2=IDD_ABOUTBOX
Class7=chhhh
Resource3=IDD_DIALOG_ZhiFangTu
Class8=CYuZhi
Resource4=IDR_MAINFRAME
Class9=CDlgYuZhiFenGe
Resource5=IDD_DIALOG_YuZhiFenGe
Class10=CDlgZhiFangTu
Resource6=IDR_MAINFRAME (English (U.S.))

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
LastObject=ID_BanYuZhi



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
Command5=ID_Diedaifazhi
Command6=ID_Yuzhifenge
Command7=ID_BanYuZhi
Command8=ID_LunKuoTiQu
Command9=ID_Lunkuogenzong
Command10=ID_Zhongzitianchong
Command11=ID_Quyushengzhang
CommandCount=11

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
LastObject=ID_Diedaifazhi
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

[DLG:IDD_DIALOG_YuZhiFenGe]
Type=1
Class=CDlgYuZhiFenGe
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_YUZHI,edit,1350631552

[CLS:CDlgYuZhiFenGe]
Type=0
HeaderFile=DlgYuZhiFenGe.h
ImplementationFile=DlgYuZhiFenGe.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgYuZhiFenGe

[DLG:IDD_DIALOG_ZhiFangTu]
Type=1
Class=CDlgZhiFangTu
ControlCount=1
Control1=IDC_COORD,static,1342308352

[CLS:CDlgZhiFangTu]
Type=0
HeaderFile=DlgZhiFangTu.h
ImplementationFile=DlgZhiFangTu.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgZhiFangTu

