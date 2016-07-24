; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWinPingDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "winping.h"
LastPage=0

ClassCount=4
Class1=CAutoComplete
Class2=CWinPingApp
Class3=CAboutDlg
Class4=CWinPingDlg

ResourceCount=2
Resource1=IDD_ABOUTBOX
Resource2=IDD_WINPING_DIALOG

[CLS:CAutoComplete]
Type=0
BaseClass=CComboBox
HeaderFile=ComboBoxEx.h
ImplementationFile=ComboBoxEx.cpp

[CLS:CWinPingApp]
Type=0
BaseClass=CWinApp
HeaderFile=WinPing.h
ImplementationFile=WinPing.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=WinPingDlg.cpp
ImplementationFile=WinPingDlg.cpp
LastObject=IDOK

[CLS:CWinPingDlg]
Type=0
BaseClass=CDialog
HeaderFile=WinPingDlg.h
ImplementationFile=WinPingDlg.cpp
Filter=D
VirtualFilter=dWC
LastObject=CWinPingDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=2
Control1=IDOK,button,1342373889
Control2=IDC_STATIC,static,1342308352

[DLG:IDD_WINPING_DIALOG]
Type=1
Class=CWinPingDlg
ControlCount=11
Control1=IDC_SERVERS,combobox,1344340226
Control2=IDC_RETRIES,edit,1350631554
Control3=IDC_SPIN1,msctls_updown32,1342177462
Control4=IDC_LIST,SysListView32,1350664193
Control5=ID_ABOUT,button,1342242816
Control6=ID_PING,button,1342242817
Control7=IDCANCEL,button,1342242816
Control8=IDC_STATIC,static,1342308352
Control9=IDC_FROM,static,1342177283
Control10=IDC_TO,static,1342177283
Control11=IDC_STATIC,static,1342308352

