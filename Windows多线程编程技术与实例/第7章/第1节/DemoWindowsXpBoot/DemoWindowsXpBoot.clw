; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDemoWindowsXpBootDlg
LastTemplate=CButton
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "DemoWindowsXpBoot.h"

ClassCount=4
Class1=CDemoWindowsXpBootApp
Class2=CDemoWindowsXpBootDlg

ResourceCount=2
Resource1=IDR_MAINFRAME
Class3=CDynBar
Class4=CMyButton
Resource2=IDD_DEMOWINDOWSXPBOOT_DIALOG

[CLS:CDemoWindowsXpBootApp]
Type=0
HeaderFile=DemoWindowsXpBoot.h
ImplementationFile=DemoWindowsXpBoot.cpp
Filter=N

[CLS:CDemoWindowsXpBootDlg]
Type=0
HeaderFile=DemoWindowsXpBootDlg.h
ImplementationFile=DemoWindowsXpBootDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=ID_BUT_BOOT



[DLG:IDD_DEMOWINDOWSXPBOOT_DIALOG]
Type=1
Class=CDemoWindowsXpBootDlg
ControlCount=4
Control1=ID_BUT_BOOT,button,1342242827
Control2=ID_BUT_STOP,button,1342242827
Control3=IDC_BOOTBAR,static,1342177294
Control4=IDC_STATIC,static,1342177294

[CLS:CDynBar]
Type=0
HeaderFile=DynBar.h
ImplementationFile=DynBar.cpp
BaseClass=CStatic
Filter=W
LastObject=CDynBar

[CLS:CMyButton]
Type=0
HeaderFile=MyButton.h
ImplementationFile=MyButton.cpp
BaseClass=CButton
Filter=W
VirtualFilter=BWC
LastObject=CMyButton

