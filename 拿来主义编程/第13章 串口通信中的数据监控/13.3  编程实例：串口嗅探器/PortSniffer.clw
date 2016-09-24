; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPortSnifferDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "PortSniffer.h"

ClassCount=3
Class1=CPortSnifferApp
Class2=CPortSnifferDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PORTSNIFFER_DIALOG

[CLS:CPortSnifferApp]
Type=0
HeaderFile=PortSniffer.h
ImplementationFile=PortSniffer.cpp
Filter=N

[CLS:CPortSnifferDlg]
Type=0
HeaderFile=PortSnifferDlg.h
ImplementationFile=PortSnifferDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=PortSnifferDlg.h
ImplementationFile=PortSnifferDlg.cpp
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

[DLG:IDD_PORTSNIFFER_DIALOG]
Type=1
Class=CPortSnifferDlg
ControlCount=14
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_BUTTONDStop,button,1342242816
Control4=IDC_BUTTONStart,button,1342242816
Control5=IDC_EDIT_ComReal,edit,1350631552
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_ComVirtual,edit,1350631552
Control8=IDC_LIST_Command,SysListView32,1350631437
Control9=IDC_STATIC,button,1342177287
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342181390
Control12=IDC_STATIC,button,1342177287
Control13=IDC_STATIC,static,1342308352
Control14=IDC_EDIT_Comment,edit,1350631552

