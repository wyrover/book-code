; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProcessMonitorDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ProcessMonitor.h"

ClassCount=3
Class1=CProcessMonitorApp
Class2=CProcessMonitorDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PROCESSMONITOR_DIALOG

[CLS:CProcessMonitorApp]
Type=0
HeaderFile=ProcessMonitor.h
ImplementationFile=ProcessMonitor.cpp
Filter=N

[CLS:CProcessMonitorDlg]
Type=0
HeaderFile=ProcessMonitorDlg.h
ImplementationFile=ProcessMonitorDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_Frequence

[CLS:CAboutDlg]
Type=0
HeaderFile=ProcessMonitorDlg.h
ImplementationFile=ProcessMonitorDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PROCESSMONITOR_DIALOG]
Type=1
Class=CProcessMonitorDlg
ControlCount=19
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON_Enum,button,1342242816
Control3=IDC_TREE_Process,SysTreeView32,1350631431
Control4=IDC_BUTTON_TerminateProcess,button,1342242816
Control5=IDC_BUTTON_EnumWindow,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342181390
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_Frequence,edit,1350631552
Control11=IDC_LIST_Monitor,SysListView32,1350631425
Control12=IDC_STATIC,static,1342308352
Control13=IDC_CHECK_RunOrNot,button,1342242819
Control14=IDC_EDIT_Pathname,edit,1484849280
Control15=IDC_BUTTON_Browser,button,1342242816
Control16=IDC_BUTTON_Add,button,1342242816
Control17=IDC_BUTTON_Delete,button,1342242816
Control18=IDC_EDIT_Program,edit,1082196096
Control19=IDC_BUTTON_Monitor,button,1342242816

