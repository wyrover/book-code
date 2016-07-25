; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CClockCtrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Clock.h"
CDK=Y

ClassCount=2
Class1=CClockCtrl
Class2=CClockPropPage

ResourceCount=2
Resource1=IDD_ABOUTBOX_CLOCK
LastPage=0
Resource2=IDD_PROPPAGE_CLOCK

[CLS:CClockCtrl]
Type=0
HeaderFile=ClockCtl.h
ImplementationFile=ClockCtl.cpp
Filter=W
BaseClass=COleControl
VirtualFilter=wWC
LastObject=CClockCtrl

[CLS:CClockPropPage]
Type=0
HeaderFile=ClockPpg.h
ImplementationFile=ClockPpg.cpp
Filter=D
BaseClass=COlePropertyPage
VirtualFilter=idWC

[DLG:IDD_ABOUTBOX_CLOCK]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PROPPAGE_CLOCK]
Type=1
Class=CClockPropPage
ControlCount=2
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_INTERVAL,edit,1350631552

