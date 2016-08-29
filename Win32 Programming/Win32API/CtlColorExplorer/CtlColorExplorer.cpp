// CtlColorExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CtlColorExplorer.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "CtlColorExplorerDoc.h"
#include "CtlColorExplorerView.h"
#include "about.h"
#include "error.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerApp

BEGIN_MESSAGE_MAP(CCtlColorExplorerApp, CWinApp)
        //{{AFX_MSG_MAP(CCtlColorExplorerApp)
        ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
                // NOTE - the ClassWizard will add and remove mapping macros here.
                //    DO NOT EDIT what you see in these blocks of generated code!
        //}}AFX_MSG_MAP
        // Standard file based document commands
        ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
        ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
        // Standard print setup command
        ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerApp construction

CCtlColorExplorerApp::CCtlColorExplorerApp()
{
        // TODO: add construction code here,
        // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCtlColorExplorerApp object

CCtlColorExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerApp initialization

BOOL CCtlColorExplorerApp::InitInstance()
{
        // Standard initialization
        // If you are not using these features and wish to reduce the size
        //  of your final executable, you should remove from the following
        //  the specific initialization routines you do not need.

#ifdef _AFXDLL
        Enable3dControls();                     // Call this when using MFC in a shared DLL
#else
        Enable3dControlsStatic();       // Call this when linking to MFC statically
#endif

        LoadStdProfileSettings();  // Load standard INI file options (including MRU)

        // Register the application's document templates.  Document templates
        //  serve as the connection between documents, frame windows and views.

        CMultiDocTemplate* pDocTemplate;
        pDocTemplate = new CMultiDocTemplate(
                IDR_CTLCOLTYPE,
                RUNTIME_CLASS(CCtlColorExplorerDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CCtlColorExplorerView));
        AddDocTemplate(pDocTemplate);

        // create main MDI Frame window
        CMainFrame* pMainFrame = new CMainFrame;
        if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
                return FALSE;
        m_pMainWnd = pMainFrame;

        // Parse command line for standard shell commands, DDE, file open
        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);

        // Dispatch commands specified on the command line
        if(cmdInfo.m_nShellCommand == CCommandLineInfo::FileNew)
           cmdInfo.m_nShellCommand = CCommandLineInfo::FileNothing; 

        if (!ProcessShellCommand(cmdInfo))
                return FALSE;

        // The main window has been initialized, so show and update it.
        pMainFrame->ShowWindow(m_nCmdShow);
        pMainFrame->UpdateWindow();

        // Force the icon to be the desired icon
        //pMainFrame->SetIcon(AfxGetApp()->LoadIcon(IDR_MAINFRAME), FALSE);

        return TRUE;
}

// App command to run the dialog
void CCtlColorExplorerApp::OnAppAbout()
{
        CAbout aboutDlg;
        aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CCtlColorExplorerApp commands
