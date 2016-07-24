; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAddDialog
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "MultiCastChat.h"
LastPage=0

ClassCount=6
Class1=CMultiCastChatApp
Class2=CMultiCastChatDoc
Class3=CMultiCastChatView
Class4=CMainFrame

ResourceCount=5
Resource1=IDD_SENDBAR
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_ABOUTBOX
Resource4=IDR_CNTR_INPLACE
Class6=CAddDialog
Resource5=IDD_ADDRESS

[CLS:CMultiCastChatApp]
Type=0
HeaderFile=MultiCastChat.h
ImplementationFile=MultiCastChat.cpp
Filter=N

[CLS:CMultiCastChatDoc]
Type=0
HeaderFile=MultiCastChatDoc.h
ImplementationFile=MultiCastChatDoc.cpp
Filter=N
BaseClass=CRichEditDoc
VirtualFilter=DC
LastObject=IDC_SEND

[CLS:CMultiCastChatView]
Type=0
HeaderFile=MultiCastChatView.h
ImplementationFile=MultiCastChatView.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame




[CLS:CAboutDlg]
Type=0
HeaderFile=MultiCastChat.cpp
ImplementationFile=MultiCastChat.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=IDC_TEST
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_PASTE_SPECIAL
Command16=ID_EDIT_SELECT_ALL
Command17=ID_EDIT_FIND
Command18=ID_EDIT_REPEAT
Command19=ID_EDIT_REPLACE
Command20=ID_OLE_INSERT_NEW
Command21=ID_OLE_EDIT_LINKS
Command22=ID_OLE_EDIT_PROPERTIES
Command23=ID_OLE_VERB_FIRST
Command24=ID_VIEW_TOOLBAR
Command25=ID_VIEW_STATUS_BAR
Command26=ID_APP_ABOUT
CommandCount=26

[MNU:IDR_CNTR_INPLACE]
Type=1
Class=CMultiCastChatView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
CommandCount=9

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_SELECT_ALL
Command10=ID_EDIT_FIND
Command11=ID_EDIT_REPEAT
Command12=ID_EDIT_REPLACE
Command13=ID_OLE_EDIT_PROPERTIES
Command14=ID_EDIT_UNDO
Command15=ID_EDIT_CUT
Command16=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_NEXT_PANE
Command19=ID_PREV_PANE
Command20=ID_CANCEL_EDIT_CNTR
CommandCount=20

[ACL:IDR_CNTR_INPLACE]
Type=1
Class=CMultiCastChatView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_PRINT
Command5=ID_NEXT_PANE
Command6=ID_PREV_PANE
Command7=ID_CANCEL_EDIT_CNTR
CommandCount=7

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=IDC_TEST
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[DLG:IDD_SENDBAR]
Type=1
Class=?
ControlCount=3
Control1=IDC_TEXT,edit,1350631552
Control2=IDC_SEND,button,1342242816
Control3=IDC_STATIC,static,1342308352

[DLG:IDD_ADDRESS]
Type=1
Class=CAddDialog
ControlCount=6
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ADDRESS,edit,1350631552
Control6=IDC_PORT,edit,1350631552

[CLS:CAddDialog]
Type=0
HeaderFile=AddDialog.h
ImplementationFile=AddDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CAddDialog

