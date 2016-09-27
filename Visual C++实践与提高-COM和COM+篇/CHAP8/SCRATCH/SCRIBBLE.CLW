; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAboutDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Scribble.h"
LastPage=0

ClassCount=9
Class1=CScribbleApp
Class2=CScribbleDoc
Class3=CScribbleView
Class4=CMainFrame
Class5=CInPlaceFrame
Class7=CChildFrame
Class9=CAboutDlg

ResourceCount=10
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_SCRIBBTYPE
Resource5=IDR_SCRIBBTYPE_SRVR_IP
Resource6=IDR_SCRIBBTYPE_SRVR_EMB
Resource8=IDR_SCRIBBTYPE_SRVR_EMB (English (U.S.))
Resource9=IDR_SCRIBBTYPE_SRVR_IP (English (U.S.))
Resource10=IDR_MAINFRAME (English (U.S.))

[CLS:CScribbleApp]
Type=0
HeaderFile=Scribble.h
ImplementationFile=Scribble.cpp
Filter=N

[CLS:CScribbleDoc]
Type=0
HeaderFile=ScribDoc.h
ImplementationFile=ScribDoc.cpp
Filter=N

[CLS:CScribbleView]
Type=0
HeaderFile=ScribVw.h
ImplementationFile=ScribVw.cpp
Filter=C


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T

[CLS:CInPlaceFrame]
Type=0
HeaderFile=IpFrame.h
ImplementationFile=IpFrame.cpp
Filter=T

[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M


[CLS:CAboutDlg]
Type=0
HeaderFile=Scribble.cpp
ImplementationFile=Scribble.cpp
Filter=D

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
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_FINDER
Command9=ID_APP_ABOUT
CommandCount=9

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command12=ID_APP_ABOUT
Command13=ID_CONTEXT_HELP
CommandCount=13

[MNU:IDR_SCRIBBTYPE]
Type=1
Class=CScribbleView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
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
Command29=ID_VIEW_TOOLBAR
Command30=ID_VIEW_STATUS_BAR
Command31=ID_WINDOW_NEW
Command32=ID_WINDOW_CASCADE
Command33=ID_WINDOW_TILE_HORZ
Command34=ID_WINDOW_ARRANGE
Command35=ID_HELP_FINDER
Command36=ID_APP_ABOUT
CommandCount=36

[MNU:IDR_SCRIBBTYPE_SRVR_EMB]
Type=1
Class=CScribbleView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_UPDATE
Command5=ID_FILE_SAVE_COPY_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command29=ID_VIEW_TOOLBAR
Command30=ID_VIEW_STATUS_BAR
Command31=ID_WINDOW_NEW
Command32=ID_WINDOW_CASCADE
Command33=ID_WINDOW_TILE_HORZ
Command34=ID_WINDOW_ARRANGE
Command35=ID_HELP_FINDER
Command36=ID_APP_ABOUT
CommandCount=36

[MNU:IDR_SCRIBBTYPE_SRVR_IP]
Type=1
Class=CScribbleView
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command29=ID_HELP_FINDER
Command30=ID_APP_ABOUT
CommandCount=30

[TB:IDR_SCRIBBTYPE_SRVR_IP]
Type=1
Class=CScribbleView
Command1=ID_EDIT_CUT
Command2=ID_EDIT_COPY
Command3=ID_EDIT_PASTE
Command8=ID_APP_ABOUT
Command9=ID_CONTEXT_HELP
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
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command12=ID_EDIT_PASTE
Command17=ID_NEXT_PANE
Command18=ID_PREV_PANE
Command19=ID_CONTEXT_HELP
Command20=ID_HELP
CommandCount=21

[ACL:IDR_SCRIBBTYPE_SRVR_IP]
Type=1
Class=CScribbleView
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command17=ID_CONTEXT_HELP
Command18=ID_HELP
CommandCount=18

[ACL:IDR_SCRIBBTYPE_SRVR_EMB]
Type=1
Class=CScribbleView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_UPDATE
Command4=ID_FILE_PRINT
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_EDIT_UNDO
Command10=ID_EDIT_CUT
Command11=ID_EDIT_COPY
Command17=ID_EDIT_PASTE
Command18=ID_NEXT_PANE
Command19=ID_PREV_PANE
Command20=ID_CONTEXT_HELP
Command21=ID_HELP
CommandCount=22


[MNU:IDR_SCRIBBTYPE_SRVR_EMB (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_UPDATE
Command5=ID_FILE_SAVE_COPY_AS
Command6=ID_FILE_PRINT
Command7=ID_FILE_PRINT_PREVIEW
Command8=ID_FILE_PRINT_SETUP
Command9=ID_FILE_MRU_FILE1
Command10=ID_APP_EXIT
Command11=ID_EDIT_UNDO
Command12=ID_EDIT_CUT
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_VIEW_TOOLBAR
Command16=ID_VIEW_STATUS_BAR
Command17=ID_WINDOW_NEW
Command18=ID_WINDOW_CASCADE
Command19=ID_WINDOW_TILE_HORZ
Command20=ID_WINDOW_ARRANGE
Command21=ID_HELP_FINDER
Command22=ID_APP_ABOUT
CommandCount=22

[MNU:IDR_SCRIBBTYPE_SRVR_IP (English (U.S.))]
Type=1
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_VIEW_TOOLBAR
Command6=ID_HELP_FINDER
Command7=ID_APP_ABOUT
CommandCount=7

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_HELP_FINDER
Command9=ID_APP_ABOUT
CommandCount=9

