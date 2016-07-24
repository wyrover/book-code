; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CMainFrame
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "xiongftp.h"
LastPage=0

ClassCount=11
Class1=CMainFrame
Class2=CLocFileView
Class3=CServFileView
Class4=CFtpInfoView
Class5=CMyAppThread
Class6=CInputEdit
Class7=CXiongFtpApp
Class8=CAboutDlg
Class9=CXiongFtpDoc
Class10=CXiongFtpView

ResourceCount=7
Resource1=CG_IDD_MYDIALOGBAR
Resource2=IDR_MAINFRAME
Resource3=IDD_ABOUTBOX
Resource4=IDR_LOC_SERV_MENU
Resource5=IDD_SERV_FORMVIEW
Resource6=IDD_LOC_FORMVIEW
Class11=CDialDlg
Resource7=IDD_DIALPROCDIG

[CLS:CMainFrame]
Type=0
BaseClass=CFrameWnd
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
VirtualFilter=fWC
LastObject=IDC_QUICKCONNECT

[CLS:CLocFileView]
Type=0
BaseClass=CFormView
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CLocFileView

[CLS:CServFileView]
Type=0
BaseClass=CFormView
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=IDC_SERV_DIR
Filter=D
VirtualFilter=VWC

[CLS:CFtpInfoView]
Type=0
BaseClass=CEditView
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CMyAppThread]
Type=0
BaseClass=CWinThread
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp

[CLS:CInputEdit]
Type=0
BaseClass=CEdit
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
LastObject=CInputEdit

[CLS:CXiongFtpApp]
Type=0
BaseClass=CWinApp
HeaderFile=xiongFtp.h
ImplementationFile=xiongFtp.cpp
LastObject=CXiongFtpApp

[CLS:CAboutDlg]
Type=0
BaseClass=CDialog
HeaderFile=xiongFtp.cpp
ImplementationFile=xiongFtp.cpp
LastObject=CAboutDlg

[CLS:CXiongFtpDoc]
Type=0
BaseClass=CDocument
HeaderFile=xiongFtpDoc.h
ImplementationFile=xiongFtpDoc.cpp

[CLS:CXiongFtpView]
Type=0
BaseClass=CListView
HeaderFile=xiongFtpView.h
ImplementationFile=xiongFtpView.cpp

[DLG:IDD_LOC_FORMVIEW]
Type=1
Class=CLocFileView
ControlCount=2
Control1=IDC_LOC_DIR,ComboBoxEx32,1344274691
Control2=IDC_LOC_FILE,SysListView32,1350632009

[DLG:IDD_SERV_FORMVIEW]
Type=1
Class=CServFileView
ControlCount=3
Control1=IDC_SERV_DIR,ComboBoxEx32,1344274691
Control2=IDC_SERV_FILE,SysListView32,1350632009
Control3=IDC_BACK,button,1342242944

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=3
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308352
Control3=IDOK,button,1342373889

[MNU:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_FILE_SAVE_AS
Command5=ID_FILE_PRINT
Command6=ID_FILE_PRINT_PREVIEW
Command7=ID_FILE_PRINT_SETUP
Command8=ID_FILE_MRU_FILE1
Command9=ID_APP_EXIT
Command10=ID_EDIT_UNDO
Command11=ID_EDIT_CUT
Command12=ID_EDIT_COPY
Command13=ID_EDIT_PASTE
Command14=ID_VIEW_TOOLBAR
Command15=ID_VIEW_STATUS_BAR
Command16=ID_APP_ABOUT
CommandCount=16

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_CONNECT
Command2=ID_FILE_DISCONNECT
Command3=IDC_FILE_RECONNECT
Command4=ID_FILE_STOP
Command5=ID_FILE_REFUSE
Command6=ID_FILE_DELETE
Command7=ID_FILE_RENAME
Command8=ID_FILE_FTP_DOWNLOAD
Command9=ID_FILE_SEND
CommandCount=9

[MNU:IDR_LOC_SERV_MENU]
Type=1
Class=?
Command1=ID_FILE_SEND
Command2=ID_FILE_DELETE
Command3=ID_FILE_RENAME
Command4=ID_FILE_EXECUTE
CommandCount=4

[ACL:IDR_MAINFRAME]
Type=1
Class=?
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

[DLG:CG_IDD_MYDIALOGBAR]
Type=1
Class=?
ControlCount=9
Control1=IDC_STATIC,static,1342308865
Control2=IDC_FTPNAME,edit,1350631552
Control3=IDC_STATIC,static,1342308864
Control4=IDC_FTPPORT,edit,1350568065
Control5=IDC_STATIC,static,1342308864
Control6=IDC_FTPUSER,edit,1350631552
Control7=IDC_STATIC,static,1342308864
Control8=IDC_FTPPASSWORD,edit,1350631584
Control9=IDC_QUICKCONNECT,button,1342242816

[DLG:IDD_DIALPROCDIG]
Type=1
Class=CDialDlg
ControlCount=9
Control1=IDOK,button,1342242817
Control2=IDCANCEL,button,1342242816
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=IDC_STATIC,static,1342308352
Control6=IDC_STATIC,static,1342308352
Control7=IDC_INETNUMBER,combobox,1344339970
Control8=IDC_INETUSER,edit,1350631552
Control9=IDC_INETPASSWORD,edit,1350631584

[CLS:CDialDlg]
Type=0
HeaderFile=mainfrm.h
ImplementationFile=mainfrm.cpp
BaseClass=CDialog
Filter=D
VirtualFilter=dWC
LastObject=CDialDlg

