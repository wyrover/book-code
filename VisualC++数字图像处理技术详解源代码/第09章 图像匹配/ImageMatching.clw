; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageMatchingView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "imagematching.h"
LastPage=0

ClassCount=11
Class1=CDlgImageMatchDis
Class2=CDlgImageMatchGYH
Class3=CDlgImageMatchMoment
Class4=CDlgImageMatchSSDA
Class5=CImageMatchingApp
Class6=CAboutDlg
Class7=CImageMatchingDoc
Class8=CImageMatchingView
Class9=CMainFrame

ResourceCount=8
Resource1=IDR_MAINFRAME
Resource2=IDD_DIALOG_GYH
Resource3=IDD_DIALOG_LSC
Resource4=IDD_DIALOG_SSDA
Resource5=IDD_DIALOG_MOMENT
Resource6=IDD_ABOUTBOX
Class10=CDlgImageMatchLSC
Resource7=IDD_DIALOG_DIS
Class11=CDlgImageMatchYG
Resource8=IDD_DIALOG_YG

[CLS:CDlgImageMatchDis]
Type=0
BaseClass=CDialog
HeaderFile=DlgImageMatchDis.h
ImplementationFile=DlgImageMatchDis.cpp

[CLS:CDlgImageMatchGYH]
Type=0
BaseClass=CDialog
HeaderFile=DlgImageMatchGYH.h
ImplementationFile=DlgImageMatchGYH.cpp
Filter=D
VirtualFilter=dWC
LastObject=IDC_IMAGEMATCH

[CLS:CDlgImageMatchMoment]
Type=0
BaseClass=CDialog
HeaderFile=DlgImageMatchMoment.h
ImplementationFile=DlgImageMatchMoment.cpp

[CLS:CDlgImageMatchSSDA]
Type=0
BaseClass=CDialog
HeaderFile=DlgImageMatchSSDA.h
ImplementationFile=DlgImageMatchSSDA.cpp

[CLS:CImageMatchingApp]
Type=0
BaseClass=CWinApp
HeaderFile=ImageMatching.h
ImplementationFile=ImageMatching.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=ImageMatching.cpp
ImplementationFile=ImageMatching.cpp
LastObject=CAboutDlg

[CLS:CImageMatchingDoc]
Type=0
BaseClass=CDocument
HeaderFile=ImageMatchingDoc.h
ImplementationFile=ImageMatchingDoc.cpp

[CLS:CImageMatchingView]
Type=0
BaseClass=CView
HeaderFile=ImageMatchingView.h
ImplementationFile=ImageMatchingView.cpp
Filter=C
VirtualFilter=VWC
LastObject=CImageMatchingView

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[DLG:IDD_DIALOG_DIS]
Type=1
Class=CDlgImageMatchDis
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INIIMAGE,static,1350569991
Control4=IDC_TEMPLATE,static,1350569991
Control5=IDC_RESULTIMAGE,static,1350569991
Control6=IDC_OPENTEMPLATE,button,1342242816
Control7=IDC_IMAGEMATCH,button,1342242816

[DLG:IDD_DIALOG_GYH]
Type=1
Class=CDlgImageMatchGYH
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INIIMAGE,static,1350569991
Control4=IDC_TEMPLATE,static,1350569991
Control5=IDC_RESULTIMAGE,static,1350569991
Control6=IDC_OPENTEMPLATE,button,1342242816
Control7=IDC_IMAGEMATCH,button,1342242816

[DLG:IDD_DIALOG_MOMENT]
Type=1
Class=CDlgImageMatchMoment
ControlCount=31
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INIIMAGE,static,1350569991
Control4=IDC_TEMPLATE,static,1350569991
Control5=IDC_OPENTEMPLATE,button,1342242816
Control6=IDC_CALMOMENT,button,1342242816
Control7=IDC_STATIC,static,1342308352
Control8=IDC_A1_INIIMAGE,edit,1350633600
Control9=IDC_A1_TEMPLATE,edit,1350633600
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352
Control13=IDC_A2_INIIMAGE,edit,1350633600
Control14=IDC_A2_TEMPLATE,edit,1350633600
Control15=IDC_A3_INIIMAGE,edit,1350633600
Control16=IDC_A3_TEMPLATE,edit,1350633600
Control17=IDC_A4_INIIMAGE,edit,1350633600
Control18=IDC_A4_TEMPLATE,edit,1350633600
Control19=IDC_A5_INIIMAGE,edit,1350633600
Control20=IDC_A5_TEMPLATE,edit,1350633600
Control21=IDC_A6_INIIMAGE,edit,1350633600
Control22=IDC_A7_TEMPLATE,edit,1350633600
Control23=IDC_A7_INIIMAGE,edit,1350633600
Control24=IDC_A6_TEMPLATE,edit,1350633600
Control25=IDC_STATIC,static,1342308352
Control26=IDC_STATIC,static,1342308352
Control27=IDC_STATIC,static,1342308352
Control28=IDC_STATIC,static,1342308352
Control29=IDC_STATIC,static,1342308352
Control30=IDC_SIMULATE,edit,1350633600
Control31=IDC_STATIC,static,1342308352

[DLG:IDD_DIALOG_SSDA]
Type=1
Class=CDlgImageMatchSSDA
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INIIMAGE,static,1350569991
Control4=IDC_TEMPLATE,static,1350569991
Control5=IDC_RESULTIMAGE,static,1350569991
Control6=IDC_OPENTEMPLATE,button,1342242816
Control7=IDC_IMAGEMATCH,button,1342242816
Control8=IDC_THRESHOLD,edit,1350631552
Control9=IDC_STATIC,static,1342308352

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
Command17=ID_IMAGEMATCH_GYH
Command18=ID_IMAGEMATCH_SSDA
Command19=ID_IMAGEMATCH_MOMENT
Command20=ID_IMAGEMATCH_DIS
Command21=ID_IMAGEMATCH_LSC
Command22=ID_IMAGEMATCH
CommandCount=22

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

[DLG:IDD_DIALOG_LSC]
Type=1
Class=CDlgImageMatchLSC
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INIIMAGE,static,1350569991
Control4=IDC_TEMPLATE,static,1350569991
Control5=IDC_RESULTIMAGE,static,1350569991
Control6=IDC_OPENTEMPLATE,button,1342242816
Control7=IDC_IMAGEMATCH,button,1342242816

[CLS:CDlgImageMatchLSC]
Type=0
HeaderFile=DlgImageMatchLSC.h
ImplementationFile=DlgImageMatchLSC.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_IMAGEMATCH_LSC
VirtualFilter=dWC

[DLG:IDD_DIALOG_YG]
Type=1
Class=CDlgImageMatchYG
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_INIIMAGE,static,1350569991
Control4=IDC_TEMPLATE,static,1350569991
Control5=IDC_RESULTIMAGE,static,1350569991
Control6=IDC_OPENTEMPLATE,button,1342242816
Control7=IDC_IMAGEMATCH,button,1342242816

[CLS:CDlgImageMatchYG]
Type=0
HeaderFile=DlgImageMatchYG.h
ImplementationFile=DlgImageMatchYG.cpp
BaseClass=CDialog
Filter=D
LastObject=ID_IMAGEMATCH
VirtualFilter=dWC

