; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSnifferDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Sniffer.h"

ClassCount=4
Class1=CSnifferApp
Class2=CSnifferDlg
Class3=CAboutDlg

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_SNIFFER_DIALOG
Class4=CDialogSelectIP
Resource4=IDD_DIALOG_IP

[CLS:CSnifferApp]
Type=0
HeaderFile=Sniffer.h
ImplementationFile=Sniffer.cpp
Filter=N

[CLS:CSnifferDlg]
Type=0
HeaderFile=SnifferDlg.h
ImplementationFile=SnifferDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_EDIT_IPFilter

[CLS:CAboutDlg]
Type=0
HeaderFile=SnifferDlg.h
ImplementationFile=SnifferDlg.cpp
Filter=D
LastObject=IDOK

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_SNIFFER_DIALOG]
Type=1
Class=CSnifferDlg
ControlCount=18
Control1=IDCANCEL,button,1342242816
Control2=IDC_LIST_Receive,SysListView32,1350631437
Control3=IDC_STATIC,button,1342177287
Control4=IDC_BUTTON_Start,button,1342242816
Control5=IDC_BUTTON_Stop,button,1342242816
Control6=IDC_EDIT_Content,edit,1350631492
Control7=IDC_STATIC,button,1342177287
Control8=IDC_RADIO_ShowMode,button,1342308361
Control9=IDC_RADIO_ShowMode2,button,1342177289
Control10=IDC_STATIC,button,1342177287
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_Comment,edit,1350631552
Control13=IDC_STATIC,static,1342181390
Control14=IDC_BUTTON_ClearResult,button,1342242816
Control15=IDC_STATIC,static,1342308352
Control16=IDC_EDIT_IPFilter,edit,1350631552
Control17=IDC_STATIC,static,1073872896
Control18=IDC_EDIT_PortFilter,edit,1082196096

[DLG:IDD_DIALOG_IP]
Type=1
Class=CDialogSelectIP
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_IP,listbox,1352728833
Control4=IDC_STATIC,static,1342308352

[CLS:CDialogSelectIP]
Type=0
HeaderFile=DialogSelectIP.h
ImplementationFile=DialogSelectIP.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_LIST_IP

