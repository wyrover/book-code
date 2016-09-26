; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CFormXCtrl
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "$$root$$.h"
CDK=Y

ClassCount=3
Class1=CFormXCtrl
Class2=CFormXPropPage

ResourceCount=2
LastPage=0
Class3=CUserDialog
Resource1=IDD_PROPPAGE_FORMX
Resource2=IDD_FORMX

[CLS:CFormXCtrl]
Type=0
HeaderFile=FormXCtl.h
ImplementationFile=FormXCtl.cpp
Filter=W
LastObject=CFormXCtrl
BaseClass=CFormControl
VirtualFilter=wWC

[CLS:CFormXPropPage]
Type=0
HeaderFile=FormXPpg.h
ImplementationFile=FormXPpg.cpp
Filter=D

[DLG:IDD_PROPPAGE_FORMX]
Type=1
Class=CFormXPropPage
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CUserDialog]
Type=0
HeaderFile=UserDialog.h
ImplementationFile=UserDialog.cpp
BaseClass=CDialog
Filter=D
LastObject=CUserDialog

[DLG:IDD_FORMX]
Type=1
Class=CFormXCtrl
ControlCount=0

