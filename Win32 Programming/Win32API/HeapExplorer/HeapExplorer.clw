; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CHeapExplorerView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "HeapExplorer.h"
LastPage=0

ClassCount=8
Class1=CHeapExplorerApp
Class2=CHeapExplorerDoc
Class3=CHeapExplorerView
Class4=CMainFrame
Class7=CAboutDlg
Class8=CChildFrame

ResourceCount=6
Resource1=IDR_MAINFRAME
Resource2=IDR_HEAPEXTYPE
Resource3=IDD_CMYSTRING
Resource4=IDD_DEFAULTHEAP
Resource5=IDD_HEAP
Resource6=IDD_ABOUTBOX

[CLS:CHeapExplorerApp]
Type=0
HeaderFile=HeapExplorer.h
ImplementationFile=HeapExplorer.cpp
Filter=N

[CLS:CHeapExplorerDoc]
Type=0
HeaderFile=HeapExplorerDoc.h
ImplementationFile=HeapExplorerDoc.cpp
Filter=N

[CLS:CHeapExplorerView]
Type=0
HeaderFile=HeapExplorerView.h
ImplementationFile=HeapExplorerView.cpp
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
HeaderFile=HeapExplorer.cpp
ImplementationFile=HeapExplorer.cpp
Filter=D

[MNU:IDR_HEAPEXTYPE]
Type=1
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=IDM_HEAP
Command7=IDM_STRTEST
Command8=ID_FILE_PRINT
Command9=ID_FILE_PRINT_PREVIEW
Command10=ID_FILE_PRINT_SETUP
Command11=ID_FILE_MRU_FILE1
Command12=ID_APP_EXIT
Command13=ID_EDIT_UNDO
Command14=ID_EDIT_CUT
Command15=ID_EDIT_COPY
Command16=ID_EDIT_PASTE
Command17=ID_VIEW_TOOLBAR
Command18=ID_VIEW_STATUS_BAR
Command19=ID_WINDOW_NEW
Command20=ID_WINDOW_CASCADE
Command21=ID_WINDOW_TILE_HORZ
Command22=ID_WINDOW_ARRANGE
Command23=ID_HELP_FINDER
Command24=ID_APP_ABOUT
CommandCount=24

[MNU:IDR_MAINFRAME]
Type=1
Command1=ID_FILE_PRINT_SETUP
Command2=IDM_HEAP
Command3=IDM_STRTEST
Command4=ID_APP_EXIT
Command5=ID_VIEW_TOOLBAR
Command6=ID_VIEW_STATUS_BAR
Command7=ID_HELP_FINDER
Command8=ID_APP_ABOUT
CommandCount=8

[TB:IDR_MAINFRAME]
Type=1
Command1=IDM_HEAP
Command2=IDM_STRTEST
Command3=ID_EDIT_CUT
Command4=ID_EDIT_COPY
Command5=ID_EDIT_PASTE
Command6=ID_FILE_PRINT
Command7=ID_APP_ABOUT
Command8=ID_CONTEXT_HELP
CommandCount=8

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

[DLG:IDD_CMYSTRING]
Type=1
ControlCount=13
Control1=IDC_STATIC,static,1342308354
Control2=IDC_INPUT,edit,1350631552
Control3=IDC_ADD,button,1342242817
Control4=IDC_STATIC,static,1342308354
Control5=IDC_STRINGS,combobox,1344340259
Control6=IDC_STATIC,static,1342308354
Control7=IDC_INFO,combobox,1344339971
Control8=IDC_DELETE,button,1342242816
Control9=IDCANCEL,button,1342242816
Control10=IDC_LAYOUT,static,1342177287
Control11=IDC_COMPACT,button,1342242816
Control12=IDC_AUTODEALLOC,button,1342242819
Control13=IDC_LEGEND,combobox,1344340003

[DLG:IDD_DEFAULTHEAP]
Type=1
ControlCount=5
Control1=IDCANCEL,button,1342242816
Control2=IDC_HEAP,static,1342177287
Control3=IDC_LEGEND,combobox,1344340003
Control4=IDC_INFO,combobox,1344339971
Control5=IDC_SCALE,static,1342177287

[DLG:IDD_HEAP]
Type=1
ControlCount=53
Control1=IDC_DATA,listbox,1352728865
Control2=IDC_10,button,1342242816
Control3=IDC_20,button,1342242816
Control4=IDC_30,button,1342242816
Control5=IDC_40,button,1342242816
Control6=IDC_50,button,1342242816
Control7=IDC_60,button,1342242816
Control8=IDC_70,button,1342242816
Control9=IDC_80,button,1342242816
Control10=IDC_90,button,1342242816
Control11=IDC_100,button,1342242816
Control12=IDC_ALLOCATOR,button,1342177287
Control13=IDC_FREE_1,button,1342242816
Control14=IDC_FREE_2,button,1342242816
Control15=IDC_FREE_3,button,1342242816
Control16=IDC_FREE_4,button,1342242816
Control17=IDC_FREE_5,button,1342242816
Control18=IDC_FREE_6,button,1342242816
Control19=IDC_FREE_7,button,1342242816
Control20=IDC_FREE_8,button,1342242816
Control21=IDC_FREE_9,button,1342242816
Control22=IDC_FREE_10,button,1342242816
Control23=IDC_FREE_11,button,1342242816
Control24=IDC_FREE_12,button,1342242816
Control25=IDC_FREE_13,button,1342242816
Control26=IDC_FREE_14,button,1342242816
Control27=IDC_FREE_15,button,1342242816
Control28=IDC_FREE_16,button,1342242816
Control29=IDC_FREE_17,button,1342242816
Control30=IDC_FREE_18,button,1342242816
Control31=IDC_FREE_19,button,1342242816
Control32=IDC_FREE_20,button,1342242816
Control33=IDC_FREE_21,button,1342242816
Control34=IDC_FREE_22,button,1342242816
Control35=IDC_FREE_23,button,1342242816
Control36=IDC_FREE_24,button,1342242816
Control37=IDC_FREE_25,button,1342242816
Control38=IDC_FREE_26,button,1342242816
Control39=IDC_FREE_27,button,1342242816
Control40=IDC_FREE_28,button,1342242816
Control41=IDC_FREE_29,button,1342242816
Control42=IDC_FREE_30,button,1342242816
Control43=IDC_FREE,button,1342177287
Control44=IDC_CREATE,button,1342242816
Control45=IDC_DESTROY,button,1342242816
Control46=IDCANCEL,button,1342242816
Control47=IDC_RESULT,edit,1484849280
Control48=IDC_INFO,combobox,1344339971
Control49=IDC_PAGE1,button,1342308361
Control50=IDC_PAGE2,button,1342177289
Control51=IDC_PAGES,button,1342177287
Control52=IDC_COMPACT,button,1342242816
Control53=IDC_LEGEND,combobox,1344340003

[ACL:IDR_MAINFRAME]
Type=1
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

