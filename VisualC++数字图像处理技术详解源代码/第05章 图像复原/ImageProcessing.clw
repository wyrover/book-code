; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CImageProcessingView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "ImageProcessing.h"
LastPage=0

ClassCount=18
Class1=CImageProcessingApp
Class2=CImageProcessingDoc
Class3=CImageProcessingView
Class4=CMainFrame

ResourceCount=5
Resource1=IDR_MAINFRAME
Resource2=IDD_ABOUTBOX
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_IMAGEPTYPE
Resource4=IDD_DIALOGBAR (English (U.S.))
Class7=CDlgHistShow
Class8=CDlgSmooth
Class9=CDlgMedian
Class10=CDlgEnhColor
Class11=CDlgEhnLinTrans
Class12=CDlgReg
Class13=CDlgAftReg
Class14=CDlgRecMatch
Class15=CDlgHuffman
Class16=CDlgShannon
Class17=CDlgArith
Class18=CDlgBitPlane
Resource5=IDD_DLG_RECOG_MATCH

[CLS:CImageProcessingApp]
Type=0
HeaderFile=ImageProcessing.h
ImplementationFile=ImageProcessing.cpp
Filter=N
BaseClass=CWinApp
VirtualFilter=AC
LastObject=CImageProcessingApp

[CLS:CImageProcessingDoc]
Type=0
HeaderFile=ImageProcessingDoc.h
ImplementationFile=ImageProcessingDoc.cpp
Filter=N
BaseClass=CDocument
VirtualFilter=DC
LastObject=CImageProcessingDoc

[CLS:CImageProcessingView]
Type=0
HeaderFile=ImageProcessingView.h
ImplementationFile=ImageProcessingView.cpp
Filter=C
BaseClass=CScrollView
VirtualFilter=VWC
LastObject=ID_DEGENERATION_INVERSE


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
HeaderFile=ImageProcessing.cpp
ImplementationFile=ImageProcessing.cpp
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
Command3=ID_FILE_PRINT_SETUP
Command4=ID_FILE_MRU_FILE1
Command5=ID_APP_EXIT
Command6=ID_VIEW_TOOLBAR
Command7=ID_VIEW_STATUS_BAR
Command8=ID_APP_ABOUT
CommandCount=8

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
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_IMAGEPTYPE]
Type=1
Class=CImageProcessingView
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_CLOSE
Command4=ID_FILE_SAVE
Command5=ID_FILE_SAVE_AS
Command6=ID_FILE_RELOAD
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
Command18=ID_VIEW_HISTOGRAM
Command19=ID_WINDOW_NEW
Command20=ID_WINDOW_CASCADE
Command21=ID_WINDOW_TILE_HORZ
Command22=ID_WINDOW_ARRANGE
Command23=ID_DEGENERATION_MOTION
Command24=ID_RESTORE_MOTION
Command25=ID_DEGENERATION_INVERSE
Command26=ID_RESTORE_INVERSE
Command27=ID_RESTORE_WINNER
Command28=ID_RESTORE_MINDBL
Command29=ID_RESTORE_MAXPRO
Command30=ID_RESTORE_MAXENTR
Command31=ID_RESTORE_PROJECT
Command32=ID_RESTORE_BLIND
Command33=ID_RESTORE_GEO
Command34=ID_APP_ABOUT
CommandCount=34

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

[DLG:IDD_DIALOGBAR (English (U.S.))]
Type=1
Class=?
ControlCount=1
Control1=IDC_STATIC,static,1342308352

[CLS:CDlgHistShow]
Type=0
HeaderFile=DlgHistShow1.h
ImplementationFile=DlgHistShow1.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgHistShow
VirtualFilter=dWC

[CLS:CDlgSmooth]
Type=0
HeaderFile=DlgSmooth.h
ImplementationFile=DlgSmooth.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgSmooth
VirtualFilter=dWC

[CLS:CDlgMedian]
Type=0
HeaderFile=DlgMedian.h
ImplementationFile=DlgMedian.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_MEDIAN_FILCENX
VirtualFilter=dWC

[CLS:CDlgEnhColor]
Type=0
HeaderFile=DlgEnhColor.h
ImplementationFile=DlgEnhColor.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[CLS:CDlgEhnLinTrans]
Type=0
HeaderFile=DlgEhnLinTrans.h
ImplementationFile=DlgEhnLinTrans.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[CLS:CDlgReg]
Type=0
HeaderFile=DlgReg.h
ImplementationFile=DlgReg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgReg

[CLS:CDlgAftReg]
Type=0
HeaderFile=DlgAftReg.h
ImplementationFile=DlgAftReg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgAftReg

[DLG:IDD_DLG_RECOG_MATCH]
Type=1
Class=CDlgRecMatch
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RECOG_INIIMAGE,static,1350569991
Control4=IDC_RECOG_MODIMAGE,static,1350569991
Control5=IDC_RECOG_RESLTIMAGE,static,1350569991
Control6=IDC_RECOG_OPEN_MODEL,button,1342242816
Control7=IDC_RECOG_MATCH,button,1342242816

[CLS:CDlgRecMatch]
Type=0
HeaderFile=DlgRecMatch.h
ImplementationFile=DlgRecMatch.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgRecMatch

[CLS:CDlgHuffman]
Type=0
HeaderFile=dlgcoding.h
ImplementationFile=DlgHuffman.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgHuffman

[CLS:CDlgShannon]
Type=0
HeaderFile=dlgcoding.h
ImplementationFile=DlgShannon.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgShannon
VirtualFilter=dWC

[CLS:CDlgArith]
Type=0
HeaderFile=dlgcoding.h
ImplementationFile=dlgarith.cpp
BaseClass=CDialog

[CLS:CDlgBitPlane]
Type=0
HeaderFile=dlgcoding.h
ImplementationFile=DlgBitPlane.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDlgBitPlane

