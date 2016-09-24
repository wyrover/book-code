; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CPlayerDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Player.h"

ClassCount=3
Class1=CPlayerApp
Class2=CPlayerDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_PLAYER_DIALOG

[CLS:CPlayerApp]
Type=0
HeaderFile=Player.h
ImplementationFile=Player.cpp
Filter=N

[CLS:CPlayerDlg]
Type=0
HeaderFile=PlayerDlg.h
ImplementationFile=PlayerDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=CPlayerDlg

[CLS:CAboutDlg]
Type=0
HeaderFile=PlayerDlg.h
ImplementationFile=PlayerDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PLAYER_DIALOG]
Type=1
Class=CPlayerDlg
ControlCount=10
Control1=IDCANCEL,button,1342242816
Control2=IDC_VLCPLUGIN1,{9BE31822-FDAD-461B-AD51-BE1D1C159921},1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_EDIT_Filename,edit,1350633600
Control5=IDC_BUTTONPlay,button,1342242816
Control6=IDC_BUTTONStop,button,1342242816
Control7=IDC_BUTTONPause,button,1342242816
Control8=IDC_STATIC,button,1342177287
Control9=IDC_STATIC,static,1342181390
Control10=IDC_BUTTONBrowser,button,1342242816

