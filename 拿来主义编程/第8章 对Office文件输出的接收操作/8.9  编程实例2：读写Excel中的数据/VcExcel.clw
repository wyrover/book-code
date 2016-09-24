; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CVcExcelDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "VcExcel.h"

ClassCount=3
Class1=CVcExcelApp
Class2=CVcExcelDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_VCEXCEL_DIALOG

[CLS:CVcExcelApp]
Type=0
HeaderFile=VcExcel.h
ImplementationFile=VcExcel.cpp
Filter=N

[CLS:CVcExcelDlg]
Type=0
HeaderFile=VcExcelDlg.h
ImplementationFile=VcExcelDlg.cpp
Filter=D
LastObject=IDC_EDIT_Filename
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=VcExcelDlg.h
ImplementationFile=VcExcelDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_VCEXCEL_DIALOG]
Type=1
Class=CVcExcelDlg
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST_Excel,SysListView32,1350631437
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_Filename,edit,1350631552
Control5=IDC_BUTTONBrowser,button,1342242816

