; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CSearchLogDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ASC.h"
LastPage=0

ClassCount=6
Class1=CASCApp
Class2=CASCDoc
Class3=CASCView
Class4=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Class5=CAboutDlg
Class6=CSearchLogDlg
Resource3=IDD_SEARCHLOGDIALOG

[CLS:CASCApp]
Type=0
HeaderFile=ASC.h
ImplementationFile=ASC.cpp
Filter=N

[CLS:CASCDoc]
Type=0
HeaderFile=ASCDoc.h
ImplementationFile=ASCDoc.cpp
Filter=N

[CLS:CASCView]
Type=0
HeaderFile=ASCView.h
ImplementationFile=ASCView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=CASCView


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=IDM_SEARCHLOG




[CLS:CAboutDlg]
Type=0
HeaderFile=ASC.cpp
ImplementationFile=ASC.cpp
Filter=D
LastObject=CAboutDlg

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
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
Command17=IDM_SEARCHLOG
CommandCount=17

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
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
CommandCount=14

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

[DLG:IDD_SEARCHLOGDIALOG]
Type=1
Class=CSearchLogDlg
ControlCount=10
Control1=IDB_SERACH,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST1,SysListView32,1350631425
Control4=IDC_STATIC,static,1342308352
Control5=IDC_DATETIMEPICKER1,SysDateTimePick32,1342242848
Control6=IDC_STATIC,static,1342308352
Control7=IDC_DATETIMEPICKER2,SysDateTimePick32,1342242857
Control8=IDC_DATETIMEPICKER3,SysDateTimePick32,1342242848
Control9=IDC_DATETIMEPICKER4,SysDateTimePick32,1342242857
Control10=IDB_SEEMAIL,button,1342242816

[CLS:CSearchLogDlg]
Type=0
HeaderFile=SearchLogDlg.h
ImplementationFile=SearchLogDlg.cpp
BaseClass=CDialog
Filter=D
LastObject=IDB_SEEMAIL
VirtualFilter=dWC

