; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CDipperDlg
LastTemplate=CStatic
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "dipper.h"

ClassCount=5
Class1=CDipperApp
Class2=CDipperDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_DIPPER_DIALOG
Resource2=IDR_MAINFRAME
Class4=CBucket
Class5=CGlass
Resource3=IDD_ABOUTBOX

[CLS:CDipperApp]
Type=0
HeaderFile=dipper.h
ImplementationFile=dipper.cpp
Filter=N

[CLS:CDipperDlg]
Type=0
HeaderFile=dipperDlg.h
ImplementationFile=dipperDlg.cpp
Filter=D
LastObject=IDC_STATUS
BaseClass=CDialog
VirtualFilter=dWC

[CLS:CAboutDlg]
Type=0
HeaderFile=dipperDlg.h
ImplementationFile=dipperDlg.cpp
Filter=D

[DLG:IDD_DIPPER_DIALOG]
Type=1
Class=CDipperDlg
ControlCount=8
Control1=IDOK,button,1342242817
Control2=IDC_GLASS1,static,1342177283
Control3=IDC_GLASS2,static,1342177283
Control4=IDC_GLASS3,static,1342177283
Control5=IDC_GLASS4,static,1342177283
Control6=IDC_GLASS5,static,1342177283
Control7=IDC_BUCKET,static,1342177283
Control8=IDC_STATUS,static,1342308352

[CLS:CBucket]
Type=0
HeaderFile=Bucket.h
ImplementationFile=Bucket.cpp
BaseClass=CStatic
Filter=W
VirtualFilter=WC
LastObject=CBucket

[CLS:CGlass]
Type=0
HeaderFile=Glass.h
ImplementationFile=Glass.cpp
BaseClass=CStatic
Filter=W
LastObject=CGlass
VirtualFilter=WC

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=13
Control1=IDOK,button,1342373889
Control2=IDC_ABOUT_FILEDESCRIPTION,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDC_ABOUT_VERSION,static,1342308352
Control5=IDC_ABOUT_LEGALCOPYRIGHT,static,1342308352
Control6=IDC_ABOUT_COMMENTS,static,1342308352
Control7=IDC_STATIC,static,1342177284
Control8=IDC_ABOUT_OSVERSION,static,1342308352
Control9=IDC_ABOUT_PROCESSORVERSION,static,1342308352
Control10=IDC_STATIC,static,1342177284
Control11=IDC_ABOUT_LEGALTRADEMARKS,static,1342308352
Control12=IDC_STATIC,static,1342177283
Control13=IDC_STATIC,static,1342177283

