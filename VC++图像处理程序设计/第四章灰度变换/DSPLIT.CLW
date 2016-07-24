; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDlgZhexianbianhuan
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DSplit.h"
LastPage=0

ClassCount=13
Class1=CDSplitApp
Class2=CDSplitDoc
Class3=CDSplitView
Class4=CMainFrame

ResourceCount=9
Resource1=IDD_DIALOG_Chuangkou
Class5=CAboutDlg
Class6=CDynSplitView2
Resource2=IDD_ABOUTBOX
Class7=chhhh
Resource3=IDD_DIALOG_Shuangyu
Class8=CYuZhi
Resource4=IDD_ABOUTBOX (English (U.S.))
Class9=CDlgZhiFangTu
Resource5=IDD_DIALOG_Guding
Class10=CDlgChuangkoubianhuan
Resource6=IDD_DLG_Zhexian
Class11=CDlgZhexianbianhuan
Resource7=IDR_MAINFRAME
Class12=CDlgGuDingFaZhi
Resource8=IDD_DLG_zhifangtufenbu
Class13=CDialogShuangYu
Resource9=IDR_MAINFRAME (English (U.S.))

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
LastObject=ID_Fenbujunhenghua



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
Command5=ID_Fei0
Command6=ID_GUDINGFA
Command7=ID_ShuanGuDingFa
Command8=ID_FanSeBianHuan
Command9=ID_Chuangkoubianhuan
Command10=ID_ZheXianBianHuan
Command11=ID_Fenbujunhenghua
Command12=ID_Pipeibianhuan
Command13=ID_Shuipingtouying
Command14=ID_Chuizhitouying
Command15=ID_Yuantuzhifangtu
Command16=ID_Zhifangtutongji
Command17=ID_RedZhiFangtu
Command18=ID_BlueZhiFangtu
Command19=ID_GreenZhifangtu
CommandCount=19

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
LastObject=ID_Shuipingtouying
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

[DLG:IDD_DLG_zhifangtufenbu]
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

[CLS:CDlgChuangkoubianhuan]
Type=0
HeaderFile=DlgChuangkoubianhuan.h
ImplementationFile=DlgChuangkoubianhuan.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_Low
VirtualFilter=dWC

[DLG:IDD_DLG_Zhexian]
Type=1
Class=CDlgZhexianbianhuan
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_X1,edit,1350631552
Control8=IDC_EDIT_Y1,edit,1350631552
Control9=IDC_EDIT_X2,edit,1350631552
Control10=IDC_EDIT_Y2,edit,1350631552
Control11=IDC_STATIC,button,1342177287
Control12=IDC_COORD,static,1342308352

[CLS:CDlgZhexianbianhuan]
Type=0
HeaderFile=DlgZhexianbianhuan.h
ImplementationFile=DlgZhexianbianhuan.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_COORD

[DLG:IDD_DIALOG_Chuangkou]
Type=1
Class=CDlgChuangkoubianhuan
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT_Up,edit,1350631552
Control6=IDC_EDIT_Low,edit,1350631552
Control7=IDC_STATIC,button,1342177287
Control8=IDC_COORD,static,1342308352

[CLS:CDlgGuDingFaZhi]
Type=0
HeaderFile=DlgGuDingFaZhi.h
ImplementationFile=DlgGuDingFaZhi.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_EDIT1

[DLG:IDD_DIALOG_Guding]
Type=1
Class=CDlgGuDingFaZhi
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT1,edit,1350631552
Control5=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_Shuangyu]
Type=1
Class=CDialogShuangYu
ControlCount=10
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT1,edit,1350631552
Control6=IDC_EDIT2,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,button,1342177287
Control9=IDC_RADIO1,button,1342308361
Control10=IDC_RADIO2,button,1342177289

[CLS:CDialogShuangYu]
Type=0
HeaderFile=DialogShuangYu.h
ImplementationFile=DialogShuangYu.cpp
BaseClass=CDialog
Filter=D
LastObject=CDialogShuangYu
VirtualFilter=dWC

