; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPOPCheckerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "POPChecker.h"

ClassCount=3
Class1=CPOPCheckerApp
Class2=CPOPCheckerDlg
Class3=CAboutDlg

ResourceCount=7
Resource1=IDD_ABOUTBOX (English (U.S.))
Resource2=IDR_MAINFRAME
Resource3=IDD_POPCHECKER_DIALOG
Resource4=IDD_ABOUTBOX
Resource5=IDR_POPUP_MENU (English (U.S.))
Resource6=IDD_POPCHECKER_DIALOG (English (U.S.))
Resource7=IDR_POPUP_MENU

[CLS:CPOPCheckerApp]
Type=0
HeaderFile=POPChecker.h
ImplementationFile=POPChecker.cpp
Filter=N
LastObject=CPOPCheckerApp

[CLS:CPOPCheckerDlg]
Type=0
HeaderFile=POPCheckerDlg.h
ImplementationFile=POPCheckerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CPOPCheckerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=POPCheckerDlg.h
ImplementationFile=POPCheckerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_POPCHECKER_DIALOG]
Type=1
Class=CPOPCheckerDlg
ControlCount=14
Control1=IDC_USERNAME,edit,1350631552
Control2=IDC_PASSWORD,edit,1350631584
Control3=IDC_POPSERVER,edit,1350631552
Control4=IDC_DURATION,edit,1350631552
Control5=IDC_SPIN1,msctls_updown32,1342177334
Control6=ID_START,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_NO_MAIL,edit,1350641792

[CLS:CPop]
Type=0
HeaderFile=Pop.h
ImplementationFile=Pop.cpp
BaseClass=CWinApp

[MNU:IDR_POPUP_MENU]
Type=1
Class=?
Command1=ID_CONFIGURATION
Command2=ID_EXIT
CommandCount=2

[DLG:IDD_POPCHECKER_DIALOG (English (U.S.))]
Type=1
Class=CPOPCheckerDlg
ControlCount=14
Control1=IDC_USERNAME,edit,1350631552
Control2=IDC_PASSWORD,edit,1350631584
Control3=IDC_POPSERVER,edit,1350631552
Control4=IDC_DURATION,edit,1350631552
Control5=IDC_SPIN1,msctls_updown32,1342177334
Control6=ID_START,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308864
Control9=IDC_STATIC,static,1342308864
Control10=IDC_STATIC,static,1342308864
Control11=IDC_STATIC,static,1342308864
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,button,1342177287
Control14=IDC_NO_MAIL,edit,1350641792

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_POPUP_MENU (English (U.S.))]
Type=1
Class=?
Command1=ID_CONFIGURATION
Command2=ID_EXIT
CommandCount=2

