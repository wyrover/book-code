; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CNBTSTATDlg
LastTemplate=CSocket
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "NBTSTAT.h"

ClassCount=3
Class1=CNBTSTATApp
Class2=CNBTSTATDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CUDP
Resource2=IDD_NBTSTAT_DIALOG

[CLS:CNBTSTATApp]
Type=0
HeaderFile=NBTSTAT.h
ImplementationFile=NBTSTAT.cpp
Filter=N

[CLS:CNBTSTATDlg]
Type=0
HeaderFile=NBTSTATDlg.h
ImplementationFile=NBTSTATDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_LIST3



[DLG:IDD_NBTSTAT_DIALOG]
Type=1
Class=CNBTSTATDlg
ControlCount=12
Control1=IDC_BTN_SEND,button,1342242816
Control2=IDC_IPADDRESS1,SysIPAddress32,1342242816
Control3=IDC_LIST3,SysListView32,1350631424
Control4=IDC_IPADDRESS2,SysIPAddress32,1342242816
Control5=IDC_LIST1,listbox,1352728835
Control6=IDC_BTN_EXIT,button,1342242816
Control7=IDC_BTN_SAVE,button,1342242816
Control8=IDC_BTN_ABOUT,button,1342242816
Control9=IDC_EDIT1,edit,1350631552
Control10=IDC_SPIN1,msctls_updown32,1342177330
Control11=IDC_STATIC,static,1342308352
Control12=IDC_STATIC,static,1342308352

[CLS:CUDP]
Type=0
HeaderFile=UDP.h
ImplementationFile=UDP.cpp
BaseClass=CSocket
Filter=N
VirtualFilter=uq

