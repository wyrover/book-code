; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CBallsRaceView
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "BallsRace.h"
LastPage=0

ClassCount=5
Class1=CBallsRaceApp
Class2=CBallsRaceDoc
Class3=CBallsRaceView
Class4=CMainFrame

ResourceCount=3
Resource1=IDR_MAINFRAME
Class5=CAboutDlg
Resource2=IDR_POPMENU
Resource3=IDD_ABOUTBOX

[CLS:CBallsRaceApp]
Type=0
HeaderFile=BallsRace.h
ImplementationFile=BallsRace.cpp
Filter=N

[CLS:CBallsRaceDoc]
Type=0
HeaderFile=BallsRaceDoc.h
ImplementationFile=BallsRaceDoc.cpp
Filter=N

[CLS:CBallsRaceView]
Type=0
HeaderFile=BallsRaceView.h
ImplementationFile=BallsRaceView.cpp
Filter=C
BaseClass=CView
VirtualFilter=VWC
LastObject=ID_BALL1CONTINUE


[CLS:CMainFrame]
Type=0
HeaderFile=MainFrm.h
ImplementationFile=MainFrm.cpp
Filter=T
LastObject=CMainFrame
BaseClass=CFrameWnd
VirtualFilter=fWC




[CLS:CAboutDlg]
Type=0
HeaderFile=BallsRace.cpp
ImplementationFile=BallsRace.cpp
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
Command1=ID_BALL0WAIT
Command2=ID_BALL1WAIT
Command3=ID_BALL2WAIT
Command4=ID_BALL3WAIT
Command5=ID_BALL4WAIT
Command6=ID_TERMINATEALL
Command7=ID_BALL0CONTINUE
Command8=ID_BALL1CONTINUE
Command9=ID_BALL2CONTINUE
Command10=ID_BALL3CONTINUE
Command11=ID_BALL4CONTINUE
CommandCount=11

[ACL:IDR_MAINFRAME]
Type=1
Class=CMainFrame
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_UNDO
Command5=ID_EDIT_CUT
Command6=ID_EDIT_COPY
Command7=ID_EDIT_PASTE
Command8=ID_EDIT_UNDO
Command9=ID_EDIT_CUT
Command10=ID_EDIT_COPY
Command11=ID_EDIT_PASTE
Command12=ID_NEXT_PANE
Command13=ID_PREV_PANE
CommandCount=13

[TB:IDR_MAINFRAME]
Type=1
Class=?
Command1=ID_FILE_NEW
Command2=ID_FILE_OPEN
Command3=ID_FILE_SAVE
Command4=ID_EDIT_CUT
Command5=ID_EDIT_COPY
Command6=ID_EDIT_PASTE
Command7=ID_FILE_PRINT
Command8=ID_APP_ABOUT
CommandCount=8

[MNU:IDR_POPMENU]
Type=1
Class=CBallsRaceView
Command1=ID_BALL0WAIT
Command2=ID_BALL1WAIT
Command3=ID_BALL2WAIT
Command4=ID_BALL3WAIT
Command5=ID_BALL4WAIT
Command6=ID_TERMINATEALL
Command7=ID_BALL0CONTINUE
Command8=ID_BALL1CONTINUE
Command9=ID_BALL2CONTINUE
Command10=ID_BALL3CONTINUE
Command11=ID_BALL4CONTINUE
CommandCount=11

