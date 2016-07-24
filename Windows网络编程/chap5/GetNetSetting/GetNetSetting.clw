; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCardInfoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "GetNetSetting.h"

ClassCount=4
Class1=CGetNetSettingApp
Class2=CGetNetSettingDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_GETNETSETTING_DIALOG
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Class4=CCardInfoDlg
Resource4=IDD_CARDINFO

[CLS:CGetNetSettingApp]
Type=0
HeaderFile=GetNetSetting.h
ImplementationFile=GetNetSetting.cpp
Filter=N

[CLS:CGetNetSettingDlg]
Type=0
HeaderFile=GetNetSettingDlg.h
ImplementationFile=GetNetSettingDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CGetNetSettingDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=GetNetSettingDlg.h
ImplementationFile=GetNetSettingDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_GETNETSETTING_DIALOG]
Type=1
Class=CGetNetSettingDlg
ControlCount=4
Control1=IDC_QUERYINFO,button,1342242816
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_NETCARDLIST,listbox,1352728835

[DLG:IDD_CARDINFO]
Type=1
Class=CCardInfoDlg
ControlCount=17
Control1=IDC_STATIC,static,1342308352
Control2=IDC_HOSTNAME,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_DOMAIN,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_ISDNS,static,1342308352
Control8=IDC_ISROUTE,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_IP,SysListView32,1350631425
Control11=IDC_STATIC,static,1342308352
Control12=IDC_DNSSERIAL,listbox,1352745219
Control13=IDC_STATIC,static,1342308352
Control14=IDC_NETGATE,listbox,1352728835
Control15=IDC_STATIC,static,1342308352
Control16=IDC_MACADDR,edit,1350631552
Control17=IDC_IPADDRESS,static,1342308352

[CLS:CCardInfoDlg]
Type=0
HeaderFile=CardInfoDlg.h
ImplementationFile=CardInfoDlg.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CCardInfoDlg

