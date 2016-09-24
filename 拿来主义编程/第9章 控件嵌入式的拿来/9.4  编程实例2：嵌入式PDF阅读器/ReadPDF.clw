; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CReadPDFDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ReadPDF.h"

ClassCount=3
Class1=CReadPDFApp
Class2=CReadPDFDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_READPDF_DIALOG

[CLS:CReadPDFApp]
Type=0
HeaderFile=ReadPDF.h
ImplementationFile=ReadPDF.cpp
Filter=N

[CLS:CReadPDFDlg]
Type=0
HeaderFile=ReadPDFDlg.h
ImplementationFile=ReadPDFDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=IDC_TREE_PDF

[CLS:CAboutDlg]
Type=0
HeaderFile=ReadPDFDlg.h
ImplementationFile=ReadPDFDlg.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_READPDF_DIALOG]
Type=1
Class=CReadPDFDlg
ControlCount=13
Control1=IDC_STATIC,static,1342308352
Control2=IDC_EDIT_Filename,edit,1350633600
Control3=IDC_BUTTON_Browser,button,1342242816
Control4=IDCANCEL,button,1342242816
Control5=IDC_STATIC,button,1342177287
Control6=IDC_STATIC_PDFVIEW,static,1073741828
Control7=IDC_STATIC,static,1342181390
Control8=IDC_BUTTON_FirstPage,button,1342242816
Control9=IDC_BUTTON_PreviousPage,button,1342242816
Control10=IDC_BUTTON_NextPage,button,1342242816
Control11=IDC_BUTTON_LastPage,button,1342242816
Control12=IDC_BUTTON_ZoomOut,button,1342242816
Control13=IDC_BUTTON_ZoomIn,button,1342242816

