; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDynSplitView2
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DSplit.h"
LastPage=0

ClassCount=12
Class1=CDSplitApp
Class2=CDSplitDoc
Class3=CDSplitView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_DIALOG5
Class5=CAboutDlg
Class6=CDynSplitView2
Resource2=IDD_ABOUTBOX
Class7=chhhh
Resource3=IDD_ABOUTBOX (English (U.S.))
Class8=CYuZhi
Resource4=IDD_DIALOG3
Class9=CShuangYuDlg
Class10=LightDlg
Resource5=IDD_DIALOG2
Class11=ContrastDlg
Resource6=IDR_MAINFRAME
Class12=PaintColorDlg
Resource7=IDR_MAINFRAME (English (U.S.))

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
LastObject=CMainFrame



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
Command5=ID_MakegGray
Command6=ID_PaintColor
Command7=ID_LightAlter
Command8=ID_ContrastAlter
Command9=ID_Smoothness
Command10=ID_Low_LVBO
Command11=ID_Low_LVBObig
Command12=ID_ShuiPing_GROW
Command13=ID_ChuiZhi_GROW
Command14=ID_ShuangXiang_GROW
Command15=ID_Sharp
Command16=ID_High_LVBO
Command17=ID_High_LVBOnormal
Command18=ID_High_LVBOexcessive
Command19=ID_NeonLight
Command20=ID_LightReverse
Command21=ID_Exposal
Command22=ID_Embossment
Command23=ID_Spread
Command24=ID_Mosaic
CommandCount=24

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
LastObject=ID_ShuiPing_GROW
VirtualFilter=VWC

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT
Command4=ID_FILE_SAVE
Command5=ID_EDIT_PASTE
Command6=ID_EDIT_UNDO
CommandCount=6

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
VirtualFilter=dWC

[CLS:CShuangYuDlg]
Type=0
HeaderFile=ShuangYuDlg.h
ImplementationFile=ShuangYuDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_DIALOG2]
Type=1
Class=LightDlg
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITLight,edit,1350631553
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352

[CLS:LightDlg]
Type=0
HeaderFile=LightDlg.h
ImplementationFile=LightDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_APP_EXIT
VirtualFilter=dWC

[DLG:IDD_DIALOG3]
Type=1
Class=ContrastDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDITContrast,edit,1350631553
Control5=IDC_STATIC,static,1342308352

[CLS:ContrastDlg]
Type=0
HeaderFile=ontrastDlg.h
ImplementationFile=ontrastDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=ContrastDlg

[DLG:IDD_DIALOG5]
Type=1
Class=PaintColorDlg
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_Red,edit,1350631553
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_Green,edit,1350631553
Control8=IDC_STATIC,static,1342308352
Control9=IDC_EDIT_Blue,edit,1350631553
Control10=IDC_STATIC,static,1342308352

[CLS:PaintColorDlg]
Type=0
HeaderFile=PaintColorDlg.h
ImplementationFile=PaintColorDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

