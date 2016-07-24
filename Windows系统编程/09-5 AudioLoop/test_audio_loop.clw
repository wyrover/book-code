; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "test_audio_loop.h"
LastPage=0

ClassCount=7
Class1=CTest_audio_loopApp
Class2=CTest_audio_loopDoc
Class3=CTest_audio_loopView
Class4=CMainFrame

ResourceCount=7
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Class5=CAboutDlg
Resource3=IDD_TEST_AUDIO_LOOP_FORM
Resource4=IDD_TEST_AUDIO_LOOP_FORM (English (U.S.))
Resource5=IDD_ABOUTBOX (English (U.S.))
Resource6=IDD_DIALOG1
Class6=CSetDialog
Class7=A_setDialog
Resource7=IDR_MAINFRAME (English (U.S.))

[CLS:CTest_audio_loopApp]
Type=0
HeaderFile=test_audio_loop.h
ImplementationFile=test_audio_loop.cpp
Filter=N
LastObject=CTest_audio_loopApp

[CLS:CTest_audio_loopDoc]
Type=0
HeaderFile=test_audio_loopDoc.h
ImplementationFile=test_audio_loopDoc.cpp
Filter=N
LastObject=CTest_audio_loopDoc

[CLS:CTest_audio_loopView]
Type=0
HeaderFile=test_audio_loopView.h
ImplementationFile=test_audio_loopView.cpp
Filter=D
BaseClass=CFormView
VirtualFilter=VWC
LastObject=CTest_audio_loopView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
BaseClass=CFrameWnd
VirtualFilter=fWC
LastObject=ID_AUDIO_THREAD_CLOSE



[CLS:CAboutDlg]
Type=0
HeaderFile=test_audio_loop.cpp
ImplementationFile=test_audio_loop.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889
Class=CAboutDlg

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_APP_ABOUT
CommandCount=13
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
CommandCount=13
Command4=ID_EDIT_UNDO
Command13=ID_PREV_PANE

[DLG:IDD_TEST_AUDIO_LOOP_FORM]
Type=1
Class=CTest_audio_loopView

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=CMainFrame
Command1=ID_AUDIO_THREAD_START
Command2=ID_AUDIO_THREAD_CLOSE
Command3=ID_VIEW_TOOLBAR
Command4=ID_VIEW_STATUS_BAR
Command5=ID_APP_EXIT
CommandCount=5

[TB:IDR_MAINFRAME (English (U.S.))]
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

[ACL:IDR_MAINFRAME (English (U.S.))]
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

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_TEST_AUDIO_LOOP_FORM (English (U.S.))]
Type=1
Class=CTest_audio_loopView
ControlCount=0

[DLG:IDD_DIALOG1]
Type=1
Class=CSetDialog
ControlCount=4
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_WORKRATE,combobox,1344340227

[CLS:CSetDialog]
Type=0
HeaderFile=SetDialog.h
ImplementationFile=SetDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=IDC_WORKRATE

[CLS:A_setDialog]
Type=0
HeaderFile=A_setDialog.h
ImplementationFile=A_setDialog.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=A_setDialog

