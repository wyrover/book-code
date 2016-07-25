; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProcessDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "process.h"

ClassCount=3
Class1=CProcessApp
Class2=CProcessDlg

ResourceCount=4
Resource2=IDD_PROCESS_DIALOG
Resource3=IDD_INFORMATION
Resource1=IDR_MAINFRAME
Class3=CInfoDlg
Resource4=IDR_MENU1

[CLS:CProcessApp]
Type=0
HeaderFile=process.h
ImplementationFile=process.cpp
Filter=N

[CLS:CProcessDlg]
Type=0
HeaderFile=processDlg.h
ImplementationFile=processDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CProcessDlg



[DLG:IDD_PROCESS_DIALOG]
Type=1
Class=CProcessDlg
ControlCount=3
Control1=IDC_LIST1,SysListView32,1350631425
Control2=IDC_REFRESH,button,1342242816
Control3=IDC_KILL,button,1342242816

[MNU:IDR_MENU1]
Type=1
Class=CProcessDlg
Command1=ID_MENU_INFO
Command2=IDC_KILL
Command3=IDC_REFRESH
CommandCount=3

[DLG:IDD_INFORMATION]
Type=1
Class=CInfoDlg
ControlCount=20
Control1=IDC_STATIC,static,1342308352
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_DWSIZE,static,1342308352
Control12=IDC_CNTUSAGE,static,1342308352
Control13=IDC_PROCESSID,static,1342308352
Control14=IDC_DEFAULTHEAPID,static,1342308352
Control15=IDC_MODULEID,static,1342308352
Control16=IDC_CNTTHREADS,static,1342308352
Control17=IDC_PARENTPROCESSID,static,1342308352
Control18=IDC_CLASSBASE,static,1342308352
Control19=IDC_DWFLAGS,static,1342308352
Control20=IDC_EXEFILE,static,1342308354

[CLS:CInfoDlg]
Type=0
HeaderFile=InfoDlg.h
ImplementationFile=InfoDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=CInfoDlg
VirtualFilter=dWC

