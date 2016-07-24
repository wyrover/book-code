; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CIpmonDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ipmon.h"

ClassCount=3
Class1=CIpmonApp
Class2=CIpmonDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_IPMON_DIALOG

[CLS:CIpmonApp]
Type=0
HeaderFile=ipmon.h
ImplementationFile=ipmon.cpp
Filter=N

[CLS:CIpmonDlg]
Type=0
HeaderFile=ipmonDlg.h
ImplementationFile=ipmonDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_IPADDRESS2

[CLS:CAboutDlg]
Type=0
HeaderFile=ipmonDlg.h
ImplementationFile=ipmonDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889

[DLG:IDD_IPMON_DIALOG]
Type=1
Class=CIpmonDlg
ControlCount=6
Control1=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control2=IDC_IPADDRESS2,SysIPAddress32,1342242816
Control3=IDC_CHECK1,button,1342242819
Control4=IDC_BUTTON1,button,1342242816
Control5=IDOK,button,1342242817
Control6=IDC_LIST1,listbox,1352728833

