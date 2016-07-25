; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChatDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Chat.h"

ClassCount=4
Class1=CChatApp
Class2=CChatDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CHAT_DIALOG

[CLS:CChatApp]
Type=0
HeaderFile=Chat.h
ImplementationFile=Chat.cpp
Filter=N

[CLS:CChatDlg]
Type=0
HeaderFile=ChatDlg.h
ImplementationFile=ChatDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ChatDlg.h
ImplementationFile=ChatDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CHAT_DIALOG]
Type=1
Class=CChatDlg
ControlCount=7
Control1=IDC_STATIC,button,1342177287
Control2=IDC_EDIT_RECV,edit,1350631556
Control3=IDC_STATIC,button,1342177287
Control4=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control5=IDC_EDIT_SEND,edit,1350631552
Control6=IDC_BTN_SEND,button,1342242817
Control7=IDC_EDIT_HOSTNAME,edit,1350631552

