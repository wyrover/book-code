; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHttpServerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HttpServer.h"

ClassCount=3
Class1=CHttpServerApp
Class2=CHttpServerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_HTTPSERVER_DIALOG

[CLS:CHttpServerApp]
Type=0
HeaderFile=HttpServer.h
ImplementationFile=HttpServer.cpp
Filter=N

[CLS:CHttpServerDlg]
Type=0
HeaderFile=HttpServerDlg.h
ImplementationFile=HttpServerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CHttpServerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=HttpServerDlg.h
ImplementationFile=HttpServerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_HTTPSERVER_DIALOG]
Type=1
Class=CHttpServerDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_BTN_STOP,button,1342242816
Control4=IDC_LIST_INFO,listbox,1353728257
Control5=IDC_STATIC_STATUS,static,1342308352
Control6=IDC_BTN_SETPATH,button,1342242816
Control7=IDC_STATIC_PATH,static,1342308352

