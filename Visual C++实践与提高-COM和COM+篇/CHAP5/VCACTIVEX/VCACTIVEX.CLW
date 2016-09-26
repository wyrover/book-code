; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVCActiveXDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VCActiveX.h"

ClassCount=3
Class1=CVCActiveXApp
Class2=CVCActiveXDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VCACTIVEX_DIALOG

[CLS:CVCActiveXApp]
Type=0
HeaderFile=VCActiveX.h
ImplementationFile=VCActiveX.cpp
Filter=N

[CLS:CVCActiveXDlg]
Type=0
HeaderFile=VCActiveXDlg.h
ImplementationFile=VCActiveXDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_CALENDARCTRL

[CLS:CAboutDlg]
Type=0
HeaderFile=VCActiveXDlg.h
ImplementationFile=VCActiveXDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VCACTIVEX_DIALOG]
Type=1
Class=CVCActiveXDlg
ControlCount=9
Control1=IDC_CALENDARCTRL,{8E27C92B-1264-101C-8A2F-040224009C02},1342242816
Control2=IDC_YEAR,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_MONTH,edit,1350631552
Control5=IDC_STATIC,static,1342308352
Control6=IDC_DAY,edit,1350631552
Control7=IDC_STATIC,static,1342308352
Control8=IDC_NEXTWEEK,button,1342242816
Control9=IDC_SELECTDATE,button,1342242816

