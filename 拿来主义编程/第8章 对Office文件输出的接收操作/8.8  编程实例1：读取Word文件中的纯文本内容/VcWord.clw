; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVcWordDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VcWord.h"

ClassCount=3
Class1=CVcWordApp
Class2=CVcWordDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VCWORD_DIALOG

[CLS:CVcWordApp]
Type=0
HeaderFile=VcWord.h
ImplementationFile=VcWord.cpp
Filter=N

[CLS:CVcWordDlg]
Type=0
HeaderFile=VcWordDlg.h
ImplementationFile=VcWordDlg.cpp
Filter=D
LastObject=IDC_EDIT_Filename
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=VcWordDlg.h
ImplementationFile=VcWordDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VCWORD_DIALOG]
Type=1
Class=CVcWordDlg
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON_Browser,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_Filename,edit,1350633600
Control5=IDC_EDIT_Content,edit,1352728580
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342181390

