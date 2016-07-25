; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CProp3
LastTemplate=CPropertySheet
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "prop.h"
LastPage=0

ClassCount=9
Class1=CMainFrame
Class2=CPropApp
Class3=CAboutDlg
Class4=CProp1
Class5=CProp2
Class6=CProp3
Class7=CPropDoc
Class8=CPropView

ResourceCount=5
Resource1=IDD_ABOUTBOX
Resource2=IDD_PROP2
Resource3=IDR_MAINFRAME
Resource4=IDD_PROP1
Class9=CPropSheet
Resource5=IDD_PROP3

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CPropApp]
Type=0
BaseClass=CWinApp
HeaderFile=Prop.h
ImplementationFile=Prop.cpp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=Prop.cpp
ImplementationFile=Prop.cpp
LastObject=IDOK

[CLS:CProp1]
Type=0
BaseClass=CPropertyPage
HeaderFile=Prop1.h
ImplementationFile=Prop1.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_LIST1

[CLS:CProp2]
Type=0
BaseClass=CPropertyPage
HeaderFile=Prop2.h
ImplementationFile=Prop2.cpp
Filter=D
VirtualFilter=idWC
LastObject=IDC_CHECK1

[CLS:CProp3]
Type=0
BaseClass=CPropertyPage
HeaderFile=Prop3.h
ImplementationFile=Prop3.cpp
Filter=D
VirtualFilter=idWC
LastObject=CProp3

[CLS:CPropDoc]
Type=0
BaseClass=CDocument
HeaderFile=PropDoc.h
ImplementationFile=PropDoc.cpp

[CLS:CPropView]
Type=0
BaseClass=CView
HeaderFile=PropView.h
ImplementationFile=PropView.cpp
Filter=C
VirtualFilter=VWC
LastObject=IDC_RADIO1

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_PROP1]
Type=1
Class=?
ControlCount=6
Control1=IDC_STATIC,button,1342177287
Control2=IDC_RADIO1,button,1342308361
Control3=IDC_RADIO2,button,1342177289
Control4=IDC_RADIO3,button,1342177289
Control5=IDC_LIST1,listbox,1352728835
Control6=IDC_STATIC,static,1342308352

[DLG:IDD_PROP2]
Type=1
Class=?
ControlCount=5
Control1=IDC_STATIC,button,1342177287
Control2=IDC_CHECK1,button,1342242819
Control3=IDC_CHECK2,button,1342242819
Control4=IDC_CHECK3,button,1342242819
Control5=IDC_CHECK4,button,1342242819

[DLG:IDD_PROP3]
Type=1
Class=?
ControlCount=2
Control1=IDC_COMBO3,combobox,1344339970
Control2=IDC_STATIC,static,1342308352

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
Command14=IDM_PROPERTYSHEET
CommandCount=14

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[CLS:CPropSheet]
Type=0
HeaderFile=PropSheet.h
ImplementationFile=PropSheet.cpp
BaseClass=CPropertySheet
Filter=W
LastObject=IDM_PROPERTYSHEET

