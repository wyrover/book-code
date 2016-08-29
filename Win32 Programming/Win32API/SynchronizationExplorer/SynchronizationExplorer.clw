; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWorker
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "SynchronizationExplorer.h"
LastPage=0

ClassCount=9
Class1=CSynchronizationExplorerApp
Class2=CSynchronizationExplorerDoc
Class3=CSynchronizationExplorerView
Class4=CMainFrame

ResourceCount=6
Resource1=IDD_ABOUTBOX
Resource2=IDR_PANEL
Resource3=IDR_SYNCHRTYPE
Resource4=IDD_CONTROL
Resource5=IDD_TASK
Class5=CAboutDlg
Class6=CChildFrame
Class7=CWorker
Class8=CPanel
Resource6=IDR_MAINFRAME
Class9=CPanelFrame

[CLS:CSynchronizationExplorerApp]
Type=0
HeaderFile=SynchronizationExplorer.h
ImplementationFile=SynchronizationExplorer.cpp
Filter=N

[CLS:CSynchronizationExplorerDoc]
Type=0
HeaderFile=SynchronizationExplorerDoc.h
ImplementationFile=SynchronizationExplorerDoc.cpp
Filter=N

[CLS:CSynchronizationExplorerView]
Type=0
HeaderFile=SynchronizationExplorerView.h
ImplementationFile=SynchronizationExplorerView.cpp
Filter=C
LastObject=CSynchronizationExplorerView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M

[CLS:CAboutDlg]
Type=0
HeaderFile=SynchronizationExplorer.cpp
ImplementationFile=SynchronizationExplorer.cpp
Filter=D

[MNU:IDR_SYNCHRTYPE]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_CLOSE
Command3=ID_FILE_PRINT
Command4=ID_FILE_PRINT_PREVIEW
Command5=ID_FILE_PRINT_SETUP
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_WINDOW_NEW
Command14=ID_WINDOW_CASCADE
Command15=ID_WINDOW_TILE_HORZ
Command16=ID_WINDOW_ARRANGE
Command17=ID_HELP_FINDER
Command18=ID_APP_ABOUT
CommandCount=18

[ACL:IDR_MAINFRAME]
Type=1
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_PRINT
Command4=ID_EDIT_PASTE
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_HELP
Command8=ID_CONTEXT_HELP
Command9=ID_NEXT_PANE
Command10=ID_PREV_PANE
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command13=ID_EDIT_CUT
Command14=ID_EDIT_UNDO
CommandCount=14

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

[DLG:IDD_CONTROL]
Type=1
Class=CPanel
ControlCount=10
Control1=IDC_SPEED,msctls_trackbar32,1342242855
Control2=IDC_N,edit,1350633600
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_ACTUAL,edit,1350633600
Control6=IDC_ERRORBAR,static,1342177284
Control7=IDC_LOCKING,button,1342242819
Control8=IDC_RANDOM,button,1342242819
Control9=IDC_SPEED_DISPLAY,static,1342308352
Control10=IDC_SYNCHRONOUS,button,1342242819

[DLG:IDD_TASK]
Type=1
Class=CWorker
ControlCount=13
Control1=IDC_LOCK,button,1342308356
Control2=IDC_FETCH,button,1342177284
Control3=IDC_INCREMENT,button,1342177284
Control4=IDC_STORE,button,1342177284
Control5=IDC_UNLOCK,button,1342177284
Control6=IDC_INTERATIONS,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_N,static,1342308352
Control9=IDC_RUNSTOP,button,1342242816
Control10=IDC_STEP,button,1342242816
Control11=IDC_PRIORITY,edit,1350633472
Control12=IDC_SPIN_PRIORITY,msctls_updown32,1342177330
Control13=IDC_STATIC,static,1073741831

[MNU:IDR_MAINFRAME]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_PRINT_SETUP
Command3=ID_APP_EXIT
Command4=ID_VIEW_TOOLBAR
Command5=ID_VIEW_STATUS_BAR
Command6=ID_HELP_FINDER
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_PANEL]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_CLOSE
Command3=ID_FILE_PRINT
Command4=ID_FILE_PRINT_PREVIEW
Command5=ID_FILE_PRINT_SETUP
Command6=ID_APP_EXIT
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_EDIT_COPY
Command10=ID_EDIT_PASTE
Command11=ID_VIEW_TOOLBAR
Command12=ID_VIEW_STATUS_BAR
Command13=ID_WINDOW_CASCADE
Command14=ID_WINDOW_TILE_HORZ
Command15=ID_WINDOW_ARRANGE
Command16=ID_HELP_FINDER
Command17=ID_APP_ABOUT
CommandCount=17

[TB:IDR_MAINFRAME]
Type=1
Command1=ID_FILE_NEW
Command2=ID_APP_ABOUT
Command3=ID_CONTEXT_HELP
CommandCount=3

[CLS:CWorker]
Type=0
HeaderFile=worker.h
ImplementationFile=worker.cpp
BaseClass=CFormView
Filter=W
VirtualFilter=VWC
LastObject=CWorker

[CLS:CPanel]
Type=0
HeaderFile=panel.h
ImplementationFile=panel.cpp
BaseClass=CFormView
Filter=D
VirtualFilter=VWC
LastObject=CPanel

[CLS:CPanelFrame]
Type=0
HeaderFile=pnlframe.h
ImplementationFile=pnlframe.cpp
BaseClass=CChildFrame
Filter=W
VirtualFilter=fWC
LastObject=CPanelFrame

