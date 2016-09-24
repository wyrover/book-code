; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPipeDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Pipe.h"

ClassCount=3
Class1=CPipeApp
Class2=CPipeDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PIPE_DIALOG

[CLS:CPipeApp]
Type=0
HeaderFile=Pipe.h
ImplementationFile=Pipe.cpp
Filter=N

[CLS:CPipeDlg]
Type=0
HeaderFile=PipeDlg.h
ImplementationFile=PipeDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_strCommand

[CLS:CAboutDlg]
Type=0
HeaderFile=PipeDlg.h
ImplementationFile=PipeDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PIPE_DIALOG]
Type=1
Class=CPipeDlg
ControlCount=7
Control1=IDCANCEL,button,1342242816
Control2=IDC_EDIT_Command,edit,1350631552
Control3=IDC_EDIT_Result,edit,1350635588
Control4=IDC_BUTTON_Run,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC,button,1342177287
Control7=IDC_STATIC,static,1342177294

