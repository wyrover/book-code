; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CTcpSpyDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "TcpSpy.h"

ClassCount=3
Class1=CTcpSpyApp
Class2=CTcpSpyDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_TCPSPY_DIALOG

[CLS:CTcpSpyApp]
Type=0
HeaderFile=TcpSpy.h
ImplementationFile=TcpSpy.cpp
Filter=N

[CLS:CTcpSpyDlg]
Type=0
HeaderFile=TcpSpyDlg.h
ImplementationFile=TcpSpyDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_TIMER

[CLS:CAboutDlg]
Type=0
HeaderFile=TcpSpyDlg.h
ImplementationFile=TcpSpyDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=?
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDOK,button,1342373889

[DLG:IDD_TCPSPY_DIALOG]
Type=1
Class=?
ControlCount=6
Control1=IDC_LIST_TCP,SysListView32,1350631433
Control2=IDC_BTN_REFRESH,button,1342242816
Control3=IDC_BTN_DISCONNECT,button,1342242816
Control4=IDC_BTN_EXIT,button,1342242816
Control5=IDC_CHKAUTO,button,1342242819
Control6=IDC_EDIT_TIMER,edit,1350631552

