; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CReadMemoryDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ReadMemory.h"

ClassCount=3
Class1=CReadMemoryApp
Class2=CReadMemoryDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_READMEMORY_DIALOG

[CLS:CReadMemoryApp]
Type=0
HeaderFile=ReadMemory.h
ImplementationFile=ReadMemory.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CReadMemoryDlg]
Type=0
HeaderFile=ReadMemoryDlg.h
ImplementationFile=ReadMemoryDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_Content

[CLS:CAboutDlg]
Type=0
HeaderFile=ReadMemoryDlg.h
ImplementationFile=ReadMemoryDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_READMEMORY_DIALOG]
Type=1
Class=CReadMemoryDlg
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_BUTTON_DebugStart,button,1342242816
Control3=IDC_BUTTON_DebugStop,button,1342242816
Control4=IDC_LIST_Content,SysListView32,1350631437
Control5=IDC_BUTTONOpen,button,1342242816
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342181390

