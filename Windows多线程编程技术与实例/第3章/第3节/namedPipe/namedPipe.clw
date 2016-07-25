; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNamedPipeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "namedPipe.h"

ClassCount=2
Class1=CNamedPipeApp
Class2=CNamedPipeDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Resource2=IDD_NAMEDPIPE_DIALOG

[CLS:CNamedPipeApp]
Type=0
HeaderFile=namedPipe.h
ImplementationFile=namedPipe.cpp
Filter=N

[CLS:CNamedPipeDlg]
Type=0
HeaderFile=namedPipeDlg.h
ImplementationFile=namedPipeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_RB_SERVER



[DLG:IDD_NAMEDPIPE_DIALOG]
Type=1
Class=CNamedPipeDlg
ControlCount=11
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_PIPENAME,edit,1350631552
Control3=IDC_B_CONNECT,button,1342242816
Control4=IDC_RB_SERVER,button,1342177289
Control5=IDC_RB_CLIENT,button,1342177289
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_SERVER_NAME,edit,1350631552
Control8=IDC_EDIT_SEND,edit,1484849280
Control9=IDC_B_SEND,button,1476460544
Control10=IDCANCEL,button,1342242816
Control11=IDC_STATIC_RX,static,1342308352

