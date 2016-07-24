; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CQueryDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "WebQuery.h"

ClassCount=3
Class1=CQueryApp
Class2=CQueryDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_WEBQUERY_DIALOG

[CLS:CQueryApp]
Type=0
HeaderFile=WebQuery.h
ImplementationFile=WebQuery.cpp
Filter=N

[CLS:CQueryDlg]
Type=0
HeaderFile=QueryDlg.h
ImplementationFile=QueryDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CQueryDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=QueryDlg.h
ImplementationFile=QueryDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_WEBQUERY_DIALOG]
Type=1
Class=CQueryDlg
ControlCount=13
Control1=IDC_EDIT_HOST,edit,1350631552
Control2=IDC_RADIO_TYPE,button,1342308361
Control3=IDC_RADIO_FTP,button,1342177289
Control4=IDC_RADIO_GOPHER,button,1342177289
Control5=IDOK,button,1342242817
Control6=IDC_EDIT_NAME,edit,1484849280
Control7=IDC_EDIT_PWD,edit,1484849312
Control8=IDCANCEL,button,1342242816
Control9=IDC_LIST_FILE,listbox,1353728257
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,button,1342177287
Control12=IDC_STATIC_NAME,static,1476526080
Control13=IDC_STATIC_PWD,static,1476526080

