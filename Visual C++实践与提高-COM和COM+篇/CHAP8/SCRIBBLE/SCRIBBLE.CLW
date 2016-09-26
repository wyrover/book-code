; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CChildFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Scribble.h"
LastPage=0

ClassCount=7
Class1=CScribbleApp
Class2=CScribbleDoc
Class3=CScribbleView
Class4=CMainFrame

ResourceCount=4
Resource1=IDD_ABOUTBOX
Resource2=IDR_SCRIBBTYPE
Resource3=IDD_PEN_WIDTHS
Class5=CAboutDlg
Class6=CChildFrame
Class7=CPenWidthsDlg
Resource4=IDR_MAINFRAME

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

[CLS:CScribbleView]
Type=0
HeaderFile=ScribVw.h
ImplementationFile=ScribVw.cpp
Filter=C
LastObject=CScribbleView
VirtualFilter=VWC

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

