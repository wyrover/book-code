; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDownloadDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Download.h"

ClassCount=4
Class1=CDownloadApp
Class2=CDownloadDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_DOWNLOAD_DIALOG

[CLS:CDownloadApp]
Type=0
HeaderFile=Download.h
ImplementationFile=Download.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC

[CLS:CDownloadDlg]
Type=0
HeaderFile=DownloadDlg.h
ImplementationFile=DownloadDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=DownloadDlg.h
ImplementationFile=DownloadDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_DOWNLOAD_DIALOG]
Type=1
Class=CDownloadDlg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDC_START,button,1342242816
Control3=IDC_CANCEL,button,1342242816

