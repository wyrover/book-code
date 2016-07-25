; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClipboardDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Clipboard.h"

ClassCount=4
Class1=CClipboardApp
Class2=CClipboardDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_CLIPBOARD_DIALOG

[CLS:CClipboardApp]
Type=0
HeaderFile=Clipboard.h
ImplementationFile=Clipboard.cpp
Filter=N

[CLS:CClipboardDlg]
Type=0
HeaderFile=ClipboardDlg.h
ImplementationFile=ClipboardDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=ClipboardDlg.h
ImplementationFile=ClipboardDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_CLIPBOARD_DIALOG]
Type=1
Class=CClipboardDlg
ControlCount=4
Control1=IDC_EDIT_SEND,edit,1350631552
Control2=IDC_EDIT_RECV,edit,1350631552
Control3=IDC_BTN_SEND,button,1342242816
Control4=IDC_BTN_RECV,button,1342242816

