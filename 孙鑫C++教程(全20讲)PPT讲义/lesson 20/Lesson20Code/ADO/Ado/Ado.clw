; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAdoDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Ado.h"

ClassCount=4
Class1=CAdoApp
Class2=CAdoDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_ADO_DIALOG

[CLS:CAdoApp]
Type=0
HeaderFile=Ado.h
ImplementationFile=Ado.cpp
Filter=N

[CLS:CAdoDlg]
Type=0
HeaderFile=AdoDlg.h
ImplementationFile=AdoDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=AdoDlg.h
ImplementationFile=AdoDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_ADO_DIALOG]
Type=1
Class=CAdoDlg
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,listbox,1352728835
Control4=IDC_BTN_QUERY,button,1342242816

