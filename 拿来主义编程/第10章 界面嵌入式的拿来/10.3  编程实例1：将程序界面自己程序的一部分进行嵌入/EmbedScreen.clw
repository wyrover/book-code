; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CEmbedScreenDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "EmbedScreen.h"

ClassCount=3
Class1=CEmbedScreenApp
Class2=CEmbedScreenDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_EMBEDSCREEN_DIALOG

[CLS:CEmbedScreenApp]
Type=0
HeaderFile=EmbedScreen.h
ImplementationFile=EmbedScreen.cpp
Filter=N

[CLS:CEmbedScreenDlg]
Type=0
HeaderFile=EmbedScreenDlg.h
ImplementationFile=EmbedScreenDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_ProgramName

[CLS:CAboutDlg]
Type=0
HeaderFile=EmbedScreenDlg.h
ImplementationFile=EmbedScreenDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_EMBEDSCREEN_DIALOG]
Type=1
Class=CEmbedScreenDlg
ControlCount=15
Control1=IDCANCEL,button,1342242816
Control2=IDC_STATIC,static,1342308352
Control3=IDC_EDIT_ProgramName,edit,1350631552
Control4=IDC_BUTTON_EmbedStart,button,1342242816
Control5=IDC_STATIC_Embed,button,1342177287
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT_OffsetLeft,edit,1350631552
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342308352
Control10=IDC_EDIT_OffsetTop,edit,1350631552
Control11=IDC_STATIC,static,1342308352
Control12=IDC_COMBO_Ratio,combobox,1344339971
Control13=IDC_STATIC,button,1342177287
Control14=IDC_BUTTON_EmbedStop,button,1342242816
Control15=IDC_STATIC,static,1342181390

