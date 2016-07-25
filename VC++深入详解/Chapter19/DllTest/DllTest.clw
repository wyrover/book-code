; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDllTestDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DllTest.h"

ClassCount=4
Class1=CDllTestApp
Class2=CDllTestDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DLLTEST_DIALOG

[CLS:CDllTestApp]
Type=0
HeaderFile=DllTest.h
ImplementationFile=DllTest.cpp
Filter=N

[CLS:CDllTestDlg]
Type=0
HeaderFile=DllTestDlg.h
ImplementationFile=DllTestDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DllTestDlg.h
ImplementationFile=DllTestDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DLLTEST_DIALOG]
Type=1
Class=CDllTestDlg
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_ADD,button,1342242816
Control4=IDC_BTN_SUBTRACT,button,1342242816
Control5=IDC_BTN_OUTPUT,button,1342242816

