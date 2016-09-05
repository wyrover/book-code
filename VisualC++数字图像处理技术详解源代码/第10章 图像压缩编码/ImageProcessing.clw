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

ResourceCount=16
Resource1=IDD_DLG_RECOG_MATCH
Resource2=IDR_MAINFRAME
Class5=CChildFrame
Class6=CAboutDlg
Resource3=IDR_IMAGEPTYPE
Resource4=IDD_DIALOGBAR (English (U.S.))
Class7=CDlgHistShow
Resource5=IDD_CODING_BITPLANE
Class8=CDlgSmooth
Resource6=IDD_DLG_REG
Class9=CDlgMedian
Resource7=IDD_DLG_ENHANCE_COLOR
Class10=CDlgEnhColor
Resource8=IDD_DLG_ENHANCE_MEDIAN
Class11=CDlgEhnLinTrans
Resource9=IDD_DLG_AFTREG
Class12=CDlgReg
Resource10=IDD_DLG_ENHANCE_SMOOTH
Class13=CDlgAftReg
Resource11=IDD_CODING_HUFFMAN
Class14=CDlgRecMatch
Resource12=IDD_DLG_ENHANCE_LINTRANS
Resource13=IDD_CODING_SHANFINO
Resource14=IDD_DLG_SHOW_HISTOGRAM
Resource15=IDD_ABOUTBOX
Class15=CDlgHuffman
Class16=CDlgShannon
Class17=CDlgArith
Class18=CDlgBitPlane
Resource16=IDD_CODING_ARITH

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
LastObject=CImageProcessingView


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
Command23=ID_CODING_HUFFMAN
Command24=ID_CODING_SHANFINO
Command25=ID_CODING_ARITH
Command26=ID_CODING_BITPLANE
Command27=ID_CODING_WRITEIMG
Command28=ID_CODING_LOADIMG
Command29=ID_APP_ABOUT
CommandCount=29

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

[DLG:IDD_DLG_SHOW_HISTOGRAM]
Type=1
Class=CDlgHistShow
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_DLG_HIST_SHOW,static,1342308352

[CLS:CDlgHistShow]
Type=0
HeaderFile=DlgHistShow1.h
ImplementationFile=DlgHistShow1.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgHistShow
VirtualFilter=dWC

[DLG:IDD_DLG_ENHANCE_SMOOTH]
Type=1
Class=CDlgSmooth
ControlCount=42
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_RADIO_AVERTEM,button,1342177289
Control4=IDC_STATIC,button,1342177287
Control5=IDC_RADIO_GUASSTEM,button,1342177289
Control6=IDC_RADIO_SELFTEM,button,1342177289
Control7=IDC_STATIC,static,1342308352
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_SELFTEM_COEF,edit,1484849280
Control13=IDC_EDIT_SELFTEM_HEIGHT,edit,1484849280
Control14=IDC_EDIT_SELFTEM_WIDTH,edit,1484849280
Control15=IDC_EDIT_SELFTEM_CEN_X,edit,1484849280
Control16=IDC_EDIT_SELFTEM_CEN_Y,edit,1484849280
Control17=IDC_STATIC,button,1342177287
Control18=IDC_EDIT_SELFTEM_EL0,edit,1484849280
Control19=IDC_EDIT_SELFTEM_EL1,edit,1484849280
Control20=IDC_EDIT_SELFTEM_EL2,edit,1484849280
Control21=IDC_EDIT_SELFTEM_EL3,edit,1484849280
Control22=IDC_EDIT_SELFTEM_EL4,edit,1484849280
Control23=IDC_EDIT_SELFTEM_EL5,edit,1484849280
Control24=IDC_EDIT_SELFTEM_EL6,edit,1484849280
Control25=IDC_EDIT_SELFTEM_EL7,edit,1484849280
Control26=IDC_EDIT_SELFTEM_EL8,edit,1484849280
Control27=IDC_EDIT_SELFTEM_EL9,edit,1484849280
Control28=IDC_EDIT_SELFTEM_EL10,edit,1484849280
Control29=IDC_EDIT_SELFTEM_EL11,edit,1484849280
Control30=IDC_EDIT_SELFTEM_EL12,edit,1484849280
Control31=IDC_EDIT_SELFTEM_EL13,edit,1484849280
Control32=IDC_EDIT_SELFTEM_EL14,edit,1484849280
Control33=IDC_EDIT_SELFTEM_EL15,edit,1484849280
Control34=IDC_EDIT_SELFTEM_EL16,edit,1484849280
Control35=IDC_EDIT_SELFTEM_EL17,edit,1484849280
Control36=IDC_EDIT_SELFTEM_EL18,edit,1484849280
Control37=IDC_EDIT_SELFTEM_EL19,edit,1484849280
Control38=IDC_EDIT_SELFTEM_EL20,edit,1484849280
Control39=IDC_EDIT_SELFTEM_EL21,edit,1484849280
Control40=IDC_EDIT_SELFTEM_EL22,edit,1484849280
Control41=IDC_EDIT_SELFTEM_EL23,edit,1484849280
Control42=IDC_EDIT_SELFTEM_EL24,edit,1484849280

[CLS:CDlgSmooth]
Type=0
HeaderFile=DlgSmooth.h
ImplementationFile=DlgSmooth.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgSmooth
VirtualFilter=dWC

[DLG:IDD_DLG_ENHANCE_MEDIAN]
Type=1
Class=CDlgMedian
ControlCount=15
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,button,1342177287
Control4=IDC_RADIO_MEDIAN_VERTICAL,button,1342373897
Control5=IDC_RADIO_MEDIAN_HORIZON,button,1342242825
Control6=IDC_RADIO_MEDIAN_2DIMEN,button,1342242825
Control7=IDC_RADIO_MEDIAN_SELFDEF,button,1342242825
Control8=IDC_STATIC,static,1342308352
Control9=IDC_STATIC,static,1342308352
Control10=IDC_STATIC,static,1342308352
Control11=IDC_STATIC,static,1342308352
Control12=IDC_EDIT_MEDIAN_FILH,edit,1484849280
Control13=IDC_EDIT_MEDIAN_FILW,edit,1484849280
Control14=IDC_EDIT_MEDIAN_FILCENX,edit,1484849280
Control15=IDC_EDIT_MEDIAN_FILCENY,edit,1484849280

[CLS:CDlgMedian]
Type=0
HeaderFile=DlgMedian.h
ImplementationFile=DlgMedian.cpp
BaseClass=CDialog
Filter=D
LastObject=IDC_EDIT_MEDIAN_FILCENX
VirtualFilter=dWC

[DLG:IDD_DLG_ENHANCE_COLOR]
Type=1
Class=CDlgEnhColor
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_LIST_ENH_COLOR,listbox,1352728835

[CLS:CDlgEnhColor]
Type=0
HeaderFile=DlgEnhColor.h
ImplementationFile=DlgEnhColor.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_DLG_ENHANCE_LINTRANS]
Type=1
Class=CDlgEhnLinTrans
ControlCount=12
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT_LINTRANS_X1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_STATIC,static,1342308352
Control8=IDC_EDIT_LINTRANS_Y1,edit,1350631552
Control9=IDC_EDIT_LINTRANS_X2,edit,1350631552
Control10=IDC_EDIT_LINTRANS_Y2,edit,1350631552
Control11=IDC_STATIC,button,1342177287
Control12=IDC_LINTRANS_SHOWCOR,static,1342308352

[CLS:CDlgEhnLinTrans]
Type=0
HeaderFile=DlgEhnLinTrans.h
ImplementationFile=DlgEhnLinTrans.cpp
BaseClass=CDialog
Filter=D
LastObject=IDOK
VirtualFilter=dWC

[DLG:IDD_DLG_REG]
Type=1
Class=CDlgReg
ControlCount=7
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_REG_INIT_IMAGE,static,1350569991
Control4=IDC_REG_RESLT_IMAGE,static,1350569991
Control5=IDC_REG_OPEN,button,1342242816
Control6=IDC_REG_CHOSE_FEATURE,button,1342242816
Control7=IDC_REG_REG,button,1342242816

[CLS:CDlgReg]
Type=0
HeaderFile=DlgReg.h
ImplementationFile=DlgReg.cpp
BaseClass=CDialog
Filter=D
LastObject=CDlgReg

[DLG:IDD_DLG_AFTREG]
Type=1
Class=CDlgAftReg
ControlCount=3
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_AFTREG_IMAGE,static,1350569991

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

[DLG:IDD_CODING_HUFFMAN]
Type=1
Class=CDlgHuffman
ControlCount=7
Control1=IDC_LST_Table,SysListView32,1350631425
Control2=IDC_STATIC1,static,1342308352
Control3=IDC_STATIC2,static,1342308352
Control4=IDC_STATIC3,static,1342308352
Control5=IDC_EDIT1,edit,1484849280
Control6=IDC_EDIT2,edit,1484849280
Control7=IDC_EDIT3,edit,1484849280

[DLG:IDD_CODING_SHANFINO]
Type=1
Class=CDlgShannon
ControlCount=7
Control1=IDC_LST_Table,SysListView32,1350631425
Control2=IDC_STATIC1,static,1342308352
Control3=IDC_STATIC2,static,1342308352
Control4=IDC_STATIC3,static,1342308352
Control5=IDC_EDIT1,edit,1484849280
Control6=IDC_EDIT2,edit,1484849280
Control7=IDC_EDIT3,edit,1484849280

[DLG:IDD_CODING_BITPLANE]
Type=1
Class=CDlgBitPlane
ControlCount=5
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_BIT,edit,1350631552

[DLG:IDD_CODING_ARITH]
Type=1
Class=CDlgArith
ControlCount=9
Control1=IDCODING,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_EDIT1,edit,1350631552
Control4=IDC_STATIC,static,1342308352
Control5=IDC_EDIT2,edit,1484849280
Control6=IDC_STATIC,static,1342308352
Control7=IDC_EDIT4,edit,1350631552
Control8=IDC_STATIC,static,1342308352
Control9=IDDECODING,button,1342242817

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
LastObject=IDOK

