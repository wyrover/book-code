; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPhoneDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Phone.h"

ClassCount=4
Class1=CPhoneApp
Class2=CPhoneDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_PHONE_DIALOG
Resource2=IDD_IPADDR
Resource3=IDD_ABOUTBOX
Class4=CIPAddress
Resource4=IDR_MAINFRAME

[CLS:CPhoneApp]
Type=0
HeaderFile=Phone.h
ImplementationFile=Phone.cpp
Filter=N
LastObject=CPhoneApp

[CLS:CPhoneDlg]
Type=0
HeaderFile=PhoneDlg.h
ImplementationFile=PhoneDlg.cpp
Filter=D
LastObject=CPhoneDlg
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=PhoneDlg.h
ImplementationFile=PhoneDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PHONE_DIALOG]
Type=1
Class=CPhoneDlg
ControlCount=2
Control1=IDOK,button,1342242817
Control2=IDC_LIST1,SysListView32,1350631424

[TB:IDR_MAINFRAME]
Type=1
Class=CPhoneDlg
Command1=ID_CONNECT
Command2=ID_DISCONNECT
Command3=ID_TEST
CommandCount=3

[MNU:IDR_MAINFRAME]
Type=1
Class=CPhoneDlg
Command1=ID_CONNECT
Command2=ID_DISCONNECT
Command3=ID_OPENSERVER
Command4=ID_TEST
Command5=ID_LOCAL_IP
CommandCount=5

[DLG:IDD_IPADDR]
Type=1
Class=CIPAddress
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_IPADDRESS,SysIPAddress32,1342242816

[CLS:CIPAddress]
Type=0
HeaderFile=IPAddress.h
ImplementationFile=IPAddress.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC

