; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CRedirectDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Redirect.h"

ClassCount=3
Class1=CRedirectApp
Class2=CRedirectDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_REDIRECT_DIALOG

[CLS:CRedirectApp]
Type=0
HeaderFile=Redirect.h
ImplementationFile=Redirect.cpp
Filter=N

[CLS:CRedirectDlg]
Type=0
HeaderFile=RedirectDlg.h
ImplementationFile=RedirectDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_Command

[CLS:CAboutDlg]
Type=0
HeaderFile=RedirectDlg.h
ImplementationFile=RedirectDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_REDIRECT_DIALOG]
Type=1
Class=CRedirectDlg
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_EDIT_Command,edit,1350631552
Control3=IDC_EDIT_Result,edit,1350635588
Control4=IDC_BUTTON_Run,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342177294

