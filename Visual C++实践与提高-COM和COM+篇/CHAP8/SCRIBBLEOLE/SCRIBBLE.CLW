; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CScribbleView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Scribble.h"
LastPage=0

ClassCount=7
Class1=CScribbleApp
Class2=CScribbleDoc
Class3=CScribbleView
Class4=CMainFrame

ResourceCount=9
Resource1=IDD_ABOUTBOX
Resource2=IDR_SCRIBBTYPE
Resource3=IDD_PEN_WIDTHS
Class5=CAboutDlg
Class6=CChildFrame
Class7=CPenWidthsDlg
Resource4=IDR_SCRIBBTYPE_SRVR_EMB (English (U.S.))
Resource5=IDR_MAINFRAME (English (U.S.))
Resource6=IDD_ABOUTBOX (English (U.S.))
Resource7=IDR_SCRIBBTYPE (English (U.S.))
Resource8=IDR_SCRIBBTYPE_SRVR_IP (English (U.S.))
Resource9=IDD_PEN_WIDTHS (English (U.S.))

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
VirtualFilter=DC
LastObject=CScribbleDoc
BaseClass=COleServerDoc

[CLS:CScribbleView]
Type=0
HeaderFile=ScribVw.h
ImplementationFile=ScribVw.cpp
Filter=C
LastObject=ID_CANCEL_EDIT_SRVR
VirtualFilter=VWC
BaseClass=CScrollView

[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC


[CLS:CChildFrame]
Type=0
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp
Filter=M
VirtualFilter=mfWC
LastObject=CChildFrame

[CLS:CAboutDlg]
Type=0
HeaderFile=Scribble.cpp
ImplementationFile=Scribble.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command0=ID_FILE_NEW
Command1=ID_FILE_SAVE
Command2=ID_PEN_THICK_OR_THIN
Command3=ID_FILE_PRINT
Command4=ID_CONTEXT_HELP
CommandCount=4

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
Command15=ID_EDIT_CLEAR_ALL
Command16=ID_PEN_THICK_OR_THIN
Command17=ID_PEN_WIDTHS
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_HELP_FINDER
Command25=ID_APP_ABOUT
CommandCount=25

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_HELP
Command10=ID_CONTEXT_HELP
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[DLG:IDD_PEN_WIDTHS]
Type=1
Class=CPenWidthsDlg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DEFAULT_PEN_WIDTHS,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_THIN_PEN_WIDTH,edit,1350631552
Control7=IDC_THICK_PEN_WIDTH,edit,1350631552

[CLS:CPenWidthsDlg]
Type=0
HeaderFile=PenDlg.h
ImplementationFile=PenDlg.cpp
Filter=D
LastObject=CPenWidthsDlg
VirtualFilter=dWC

[DLG:IDD_PEN_WIDTHS (English (U.S.))]
Type=1
Class=?
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DEFAULT_PEN_WIDTHS,button,1342242816
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_THIN_PEN_WIDTH,edit,1350631552
Control7=IDC_THICK_PEN_WIDTH,edit,1350631552

[MNU:IDR_SCRIBBTYPE (English (U.S.))]
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
Command9=ID_FILE_SEND_MAIL
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_EDIT_CLEAR_ALL
Command17=ID_PEN_THICK_OR_THIN
Command18=ID_PEN_WIDTHS
Command19=ID_VIEW_TOOLBAR
Command20=ID_VIEW_STATUS_BAR
Command21=ID_WINDOW_NEW
Command22=ID_WINDOW_CASCADE
Command23=ID_WINDOW_TILE_HORZ
Command24=ID_WINDOW_ARRANGE
Command25=ID_HELP_FINDER
Command26=ID_APP_ABOUT
CommandCount=26

[TB:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_PEN_THICK_OR_THIN
Command5=ID_FILE_PRINT
Command6=ID_APP_ABOUT
Command7=ID_CONTEXT_HELP
CommandCount=7

[MNU:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
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

[ACL:IDR_MAINFRAME (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=ID_FILE_NEW
Command3=ID_FILE_OPEN
Command4=ID_FILE_PRINT
Command5=ID_FILE_SAVE
Command6=ID_EDIT_PASTE
Command7=ID_EDIT_UNDO
Command8=ID_EDIT_CUT
Command9=ID_HELP
Command10=ID_CONTEXT_HELP
Command11=ID_NEXT_PANE
Command12=ID_PREV_PANE
Command13=ID_EDIT_COPY
Command14=ID_EDIT_PASTE
Command15=ID_EDIT_CUT
Command16=ID_EDIT_UNDO
CommandCount=16

[DLG:IDD_ABOUTBOX (English (U.S.))]
Type=1
Class=?
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[MNU:IDR_SCRIBBTYPE_SRVR_IP (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_PEN_THICK_OR_THIN
Command6=ID_PEN_WIDTHS
Command7=ID_VIEW_TOOLBAR
Command8=ID_HELP_FINDER
Command9=ID_APP_ABOUT
CommandCount=9

[MNU:IDR_SCRIBBTYPE_SRVR_EMB (English (U.S.))]
Type=1
Class=?
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
Command15=ID_EDIT_CLEAR_ALL
Command16=ID_PEN_THICK_OR_THIN
Command17=ID_PEN_WIDTHS
Command18=ID_VIEW_TOOLBAR
Command19=ID_VIEW_STATUS_BAR
Command20=ID_WINDOW_NEW
Command21=ID_WINDOW_CASCADE
Command22=ID_WINDOW_TILE_HORZ
Command23=ID_WINDOW_ARRANGE
Command24=ID_HELP_FINDER
Command25=ID_APP_ABOUT
CommandCount=25

[TB:IDR_SCRIBBTYPE_SRVR_IP (English (U.S.))]
Type=1
Class=?
Command1=ID_PEN_THICK_OR_THIN
Command2=ID_APP_ABOUT
Command3=ID_CONTEXT_HELP
CommandCount=3

[ACL:IDR_SCRIBBTYPE_SRVR_EMB (English (U.S.))]
Type=1
Class=?
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
Command12=ID_EDIT_PASTE
Command13=ID_NEXT_PANE
Command14=ID_PREV_PANE
Command15=ID_CONTEXT_HELP
Command16=ID_HELP
CommandCount=16

[ACL:IDR_SCRIBBTYPE_SRVR_IP (English (U.S.))]
Type=1
Class=?
Command1=ID_EDIT_UNDO
Command2=ID_EDIT_CUT
Command3=ID_EDIT_COPY
Command4=ID_EDIT_PASTE
Command5=ID_EDIT_UNDO
Command6=ID_EDIT_CUT
Command7=ID_EDIT_COPY
Command8=ID_EDIT_PASTE
Command9=ID_CONTEXT_HELP
Command10=ID_HELP
Command11=ID_CANCEL_EDIT_SRVR
CommandCount=11

