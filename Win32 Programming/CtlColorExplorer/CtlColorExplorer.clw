; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CAbout
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ctlcolorexplorer.h"
LastPage=0

ClassCount=11
Class1=CAbout
Class2=CCBTMsgBox
Class3=CChildFrame
Class4=CColor
Class5=CCtlColorParms
Class6=CCtlColorExplorerApp
Class7=CCtlColorExplorerDoc
Class8=CCtlColorExplorerView
Class9=CMainFrame
Class10=CRGB
Class11=CSampler

ResourceCount=5
Resource1=IDR_CTLCOLTYPE
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDD_CTLCOLOR
Resource5=IDD_SAMPLE

[CLS:CAbout]
Type=0
BaseClass=CDialog
HeaderFile=About.h
ImplementationFile=About.cpp
LastObject=CAbout

[CLS:CCBTMsgBox]
Type=0
BaseClass=CWnd
HeaderFile=CBTMsg.h
ImplementationFile=CBTMsg.cpp

[CLS:CChildFrame]
Type=0
BaseClass=CMDIChildWnd
HeaderFile=ChildFrm.h
ImplementationFile=ChildFrm.cpp

[CLS:CColor]
Type=0
BaseClass=CComboBox
HeaderFile=Color.h
ImplementationFile=Color.cpp

[CLS:CCtlColorParms]
Type=0
BaseClass=CPropertyPage
HeaderFile=CtlColor.h
ImplementationFile=CtlColor.cpp

[CLS:CCtlColorExplorerApp]
Type=0
BaseClass=CWinApp
HeaderFile=CtlColorExplorer.h
ImplementationFile=CtlColorExplorer.cpp

[CLS:CCtlColorExplorerDoc]
Type=0
BaseClass=CDocument
HeaderFile=CtlColorExplorerDoc.h
ImplementationFile=CtlColorExplorerDoc.cpp

[CLS:CCtlColorExplorerView]
Type=0
BaseClass=CView
HeaderFile=CtlColorExplorerView.h
ImplementationFile=CtlColorExplorerView.cpp

[CLS:CMainFrame]
Type=0
BaseClass=CMDIFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CRGB]
Type=0
BaseClass=CStatic
HeaderFile=RGB.h
ImplementationFile=RGB.cpp

[CLS:CSampler]
Type=0
BaseClass=CPropertyPage
HeaderFile=Sampler.h
ImplementationFile=Sampler.cpp

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAbout
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

[DLG:IDD_CTLCOLOR]
Type=1
Class=CCtlColorParms
ControlCount=19
Control1=IDC_TEXTCOLOR,combobox,1344340755
Control2=IDC_TEXTCOLOR_CAPTION,static,1342308354
Control3=IDC_TRANSPARENT,button,1342177289
Control4=IDC_OPAQUE,button,1342177289
Control5=IDC_STATIC,button,1342177287
Control6=IDC_BKCOLOR,combobox,1344340755
Control7=IDC_BKCOLOR_CAPTION,static,1342308354
Control8=IDC_STATIC,button,1342177287
Control9=IDC_CHANGEFONT,button,1342242816
Control10=IDC_FONTGROUP,button,1342177287
Control11=IDC_FACENAME,static,1342308352
Control12=IDC_BKBRUSH,combobox,1344340755
Control13=IDC_BKBRUSH_CAPTION,static,1342308354
Control14=IDC_RGBTEXT,static,1342308352
Control15=IDC_RGBBKG,static,1342308352
Control16=IDC_RGBBR,static,1342308352
Control17=IDC_NOSETFONT,button,1342242819
Control18=IDC_CLEARFONT,button,1342242816
Control19=IDC_SAMPLE,static,1350696961

[DLG:IDD_SAMPLE]
Type=1
Class=CSampler
ControlCount=20
Control1=IDC_EDIT_NORMAL,edit,1350631552
Control2=IDC_NORMAL_CAPTION,static,1342308354
Control3=IDC_EDIT_READONLY,edit,1350633600
Control4=IDC_READONLY_CAPTION,static,1342308354
Control5=IDC_EDIT_DISABLED,edit,1484851328
Control6=IDC_DISABLED_CAPTION,static,1342308354
Control7=IDC_COMBO1,combobox,1344340226
Control8=IDC_COMBO2,combobox,1344340227
Control9=IDC_LIST1,listbox,1352728835
Control10=IDC_RADIO1,button,1342373897
Control11=IDC_RADIO2,button,1342177289
Control12=IDC_RADIO3,button,1342177289
Control13=IDC_GROUP,button,1342177287
Control14=IDC_BUTTON,button,1342242816
Control15=IDC_CHECK1,button,1342242819
Control16=IDC_SLIDER1,msctls_trackbar32,1342242855
Control17=IDC_PROGRESS1,msctls_progress32,1350565888
Control18=IDC_SCROLLBAR1,scrollbar,1342177280
Control19=IDC_COMBO3,combobox,1344340225
Control20=IDC_MULTILINE,edit,1352732868

[MNU:IDR_CTLCOLTYPE]
Type=1
Class=?
Command1=IDM_CTLCOLOR
Command2=ID_FILE_PRINT
Command3=ID_FILE_PRINT_PREVIEW
Command4=ID_FILE_PRINT_SETUP
Command5=ID_FILE_MRU_FILE1
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

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=IDM_CTLCOLOR
Command2=ID_FILE_PRINT_SETUP
Command3=ID_FILE_MRU_FILE1
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_HELP_FINDER
Command8=ID_APP_ABOUT
CommandCount=8

[ACL:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_EDIT_COPY
Command2=IDM_CTLCOLOR
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

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=IDM_CTLCOLOR
Command2=ID_FILE_PRINT
Command3=ID_APP_ABOUT
Command4=ID_CONTEXT_HELP
CommandCount=4

