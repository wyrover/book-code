; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CCircPropPage
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MfcCircCtrl.h"
CDK=Y

ClassCount=2
Class1=CCircCtrl
Class2=CCircPropPage

ResourceCount=2
LastPage=0
Resource1=IDD_ABOUTBOX_CIRC
Resource2=IDD_PROPPAGE_CIRC

[CLS:CCircCtrl]
Type=0
HeaderFile=CircCtl.h
ImplementationFile=CircCtl.cpp
Filter=W
BaseClass=COleControl
VirtualFilter=wWC
LastObject=CCircCtrl

[CLS:CCircPropPage]
Type=0
HeaderFile=CircPpg.h
ImplementationFile=CircPpg.cpp
Filter=D
LastObject=CCircPropPage
BaseClass=COlePropertyPage
VirtualFilter=idWC

[DLG:IDD_ABOUTBOX_CIRC]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PROPPAGE_CIRC]
Type=1
Class=CCircPropPage
ControlCount=5
Control1=IDC_STATIC,static,1342308352
Control2=IDC_CAPTION,edit,1350631552
Control3=IDC_STATIC,static,1342308352
Control4=IDC_CIRCLEOFFSET,edit,1350631552
Control5=IDC_CIRCLESHAPE,button,1342242851

