; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CWindowExplorerView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "Window Explorer.h"
LastPage=0

ClassCount=8
Class1=CWindowExplorerApp
Class2=CWindowExplorerDoc
Class3=CWindowExplorerView
Class4=CMainFrame
Class7=CAboutDlg
Class8=CChildFrame

ResourceCount=11
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDR_WINDOWTYPE
Resource8=IDR_CONTEXTMENU
Resource9=IDR_MENU1
Resource10=IDD_CREATEWINDOWEX
Resource11=IDD_STYLES

[CLS:CWindowExplorerApp]
Type=0
HeaderFile=Window Explorer.h
ImplementationFile=Window Explorer.cpp
Filter=N

[CLS:CWindowExplorerDoc]
Type=0
HeaderFile=Window ExplorerDoc.h
ImplementationFile=Window ExplorerDoc.cpp
Filter=N

[CLS:CWindowExplorerView]
Type=0
HeaderFile=Window ExplorerView.h
ImplementationFile=Window ExplorerView.cpp
Filter=C

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
HeaderFile=Window Explorer.cpp
ImplementationFile=Window Explorer.cpp
Filter=D

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
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

[MNU:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=IDM_WNDEXP
Command4=ID_FILE_PRINT_SETUP
Command5=ID_FILE_MRU_FILE1
Command6=ID_APP_EXIT
Command7=ID_VIEW_TOOLBAR
Command8=ID_VIEW_STATUS_BAR
Command9=ID_HELP_FINDER
Command10=ID_APP_ABOUT
CommandCount=10

[TB:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=IDM_WNDEXP
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_FILE_PRINT
Command9=ID_APP_ABOUT
Command10=ID_CONTEXT_HELP
CommandCount=10

[MNU:IDR_WINDOWTYPE]
Type=1
Class=CWindowExplorerView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=IDM_WNDEXP
Command7=ID_FILE_PRINT
Command8=ID_FILE_PRINT_PREVIEW
Command9=ID_FILE_PRINT_SETUP
Command10=ID_FILE_MRU_FILE1
Command11=ID_APP_EXIT
Command12=ID_EDIT_UNDO
Command13=ID_EDIT_CUT
Command14=ID_EDIT_COPY
Command15=ID_EDIT_PASTE
Command16=ID_VIEW_TOOLBAR
Command17=ID_VIEW_STATUS_BAR
Command18=ID_WINDOW_NEW
Command19=ID_WINDOW_CASCADE
Command20=ID_WINDOW_TILE_HORZ
Command21=ID_WINDOW_ARRANGE
Command22=ID_HELP_FINDER
Command23=ID_APP_ABOUT
CommandCount=23

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
Command15=ID_CONTEXT_HELP
Command16=ID_HELP
CommandCount=16

[MNU:IDR_CONTEXTMENU]
Type=1
Command1=IDM_PROPERTIES
Command2=IDM_DESTROY
CommandCount=2

[MNU:IDR_MENU1]
Type=1
Command1=IDM_ROUND_FILE
Command2=IDM_FLAT_FILE
Command3=IDM_HALF_ROUND
Command4=IDM_RAT_TAIL
Command5=IDM_DESTROY
Command6=IDM_EDIT_SNIP
Command7=IDM_PLAGIARIZE
Command8=IDM_GLUE
Command9=IDM_OCEAN_VIEW
Command10=IDM_MOUNTAIN_VIEW
Command11=IDM_BAY_WINDOW
Command12=IDM_PICTURE_WINDOW
Command13=IDM_TRIPLE_GLAZED
Command14=IDM_NOHELP
Command15=IDM_ABOUT_THIS
CommandCount=15

[DLG:IDD_CREATEWINDOWEX]
Type=1
ControlCount=50
Control1=IDC_WS_EX_ACCEPTFILES,button,1342242819
Control2=IDC_WS_EX_APPWINDOW,button,1342242819
Control3=IDC_WS_EX_CLIENTEDGE,button,1342242819
Control4=IDC_WS_EX_CONTEXTHELP,button,1342242819
Control5=IDC_WS_EX_CONTROLPARENT,button,1342242819
Control6=IDC_WS_EX_DLGMODALFRAME,button,1342242819
Control7=IDC_WS_EX_LEFT,button,1342242819
Control8=IDC_WS_EX_LEFTSCROLLBAR,button,1342242819
Control9=IDC_WS_EX_LTRREADING,button,1342242819
Control10=IDC_WS_EX_MDICHILD,button,1342242819
Control11=IDC_WS_EX_NOPARENTNOTIFY,button,1342242819
Control12=IDC_WS_EX_OVERLAPPEDWINDOW,button,1342242819
Control13=IDC_WS_EX_PALETTEWINDOW,button,1342242819
Control14=IDC_WS_EX_RIGHT,button,1342242819
Control15=IDC_WS_EX_RIGHTSCROLLBAR,button,1342242819
Control16=IDC_WS_EX_RTLREADING,button,1342242819
Control17=IDC_WS_EX_STATICEDGE,button,1342242819
Control18=IDC_WS_EX_TOOLWINDOW,button,1342242819
Control19=IDC_WS_EX_TOPMOST,button,1342242819
Control20=IDC_WS_EX_TRANSPARENT,button,1342242819
Control21=IDC_WS_EX_WINDOWEDGE,button,1342242819
Control22=IDC_STATIC,static,1342308352
Control23=IDC_CLASSNAME,combobox,1344340227
Control24=IDC_STATIC,static,1342308352
Control25=IDC_STATIC,button,1342177287
Control26=IDC_WINDOWNAME,edit,1350631552
Control27=IDC_STATIC,static,1342308352
Control28=IDC_CREATE,button,1342242816
Control29=IDC_DESTROY,button,1342242816
Control30=IDC_X,edit,1350631552
Control31=IDC_XSPIN,msctls_updown32,1342177330
Control32=IDC_STATIC,static,1342308352
Control33=IDC_Y,edit,1350631552
Control34=IDC_YSPIN,msctls_updown32,1342177330
Control35=IDC_STATIC,static,1342308352
Control36=IDC_W,edit,1350631552
Control37=IDC_WSPIN,msctls_updown32,1342177330
Control38=IDC_STATIC,static,1342308352
Control39=IDC_H,edit,1350631552
Control40=IDC_HSPIN,msctls_updown32,1342177330
Control41=IDC_STATIC,static,1342308352
Control42=IDC_PARENT,combobox,1344340227
Control43=IDC_STATIC,static,1342308352
Control44=IDC_MESSAGE,edit,1350633600
Control45=IDC_SHOW,edit,1350633600
Control46=IDC_WINDOWS,SysListView32,1350631441
Control47=IDC_API3_ONLY,button,1342242851
Control48=IDC_MENU,combobox,1344340227
Control49=IDC_ID,edit,1350631552
Control50=IDC_SPINID,msctls_updown32,1342177330

[DLG:IDD_STYLES]
Type=1
ControlCount=23
Control1=IDC_WS_BORDER,button,1342242819
Control2=IDC_WS_CAPTION,button,1342242819
Control3=IDC_WS_CHILD,button,1342242819
Control4=IDC_WS_CHILDWINDOW,button,1342242819
Control5=IDC_WS_CLIPCHILDREN,button,1342242819
Control6=IDC_WS_CLIPSIBLINGS,button,1342242819
Control7=IDC_WS_DLGFRAME,button,1342242819
Control8=IDC_WS_GROUP,button,1342242819
Control9=IDC_WS_HSCROLL,button,1342242819
Control10=IDC_WS_MAXIMIZE,button,1342242819
Control11=IDC_WS_MAXIMIZEBOX,button,1342242819
Control12=IDC_WS_MINIMIZE,button,1342242819
Control13=IDC_WS_MINIMIZEBOX,button,1342242819
Control14=IDC_WS_OVERLAPPED,button,1342242819
Control15=IDC_WS_OVERLAPPEDWINDOW,button,1342242819
Control16=IDC_WS_POPUP,button,1342242819
Control17=IDC_WS_POPUPWINDOW,button,1342242819
Control18=IDC_WS_SYSMENU,button,1342242819
Control19=IDC_WS_TABSTOP,button,1342242819
Control20=IDC_WS_THICKFRAME,button,1342242819
Control21=IDC_WS_VISIBLE,button,1476460547
Control22=IDC_WS_VSCROLL,button,1342242819
Control23=IDC_FLAGS,edit,1350633600

