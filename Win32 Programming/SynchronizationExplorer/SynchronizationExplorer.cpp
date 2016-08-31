// SynchronizationExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include <afxmt.h> // multithreading facility
#include "SynchronizationExplorer.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "SynchronizationExplorerDoc.h"
#include "Delta.h"
#include "thread.h"
#include "IntEdit.h"
#include "worker.h"
#include "About.h"
#include "panel.h"
#include "pnlframe.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerApp

BEGIN_MESSAGE_MAP(CSynchronizationExplorerApp, CWinApp)
        //{{AFX_MSG_MAP(CSynchronizationExplorerApp)
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
// CSynchronizationExplorerApp construction

CSynchronizationExplorerApp::CSynchronizationExplorerApp()
{
        // TODO: add construction code here,
        // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSynchronizationExplorerApp object

CSynchronizationExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerApp initialization

BOOL CSynchronizationExplorerApp::InitInstance()
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
                IDR_SYNCHRTYPE,
                RUNTIME_CLASS(CSynchronizationExplorerDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CWorker));
        AddDocTemplate(pDocTemplate);

        // create control panel

        panelTemplate = new CMultiDocTemplate(
                IDR_PANEL,
                RUNTIME_CLASS(CSynchronizationExplorerDoc),
                RUNTIME_CLASS(CPanelFrame),
                RUNTIME_CLASS(CPanel));
        // Note that we do not add the document template as we will never
        // create more than one

        // create main MDI Frame window
        CMainFrame* pMainFrame = new CMainFrame;
        if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
                return FALSE;
        m_pMainWnd = pMainFrame;

        // Now that we have our main window, create the control panel

        panelTemplate->OpenDocumentFile(NULL, TRUE);

        // Parse command line for standard shell commands, DDE, file open
        CCommandLineInfo cmdInfo;
        ParseCommandLine(cmdInfo);

        // Dispatch commands specified on the command line
        if (!ProcessShellCommand(cmdInfo))
                return FALSE;

        // The main window has been initialized, so show and update it.
        pMainFrame->ShowWindow(m_nCmdShow);
        pMainFrame->UpdateWindow();

        return TRUE;
}

// App command to run the dialog
void CSynchronizationExplorerApp::OnAppAbout()
{
        CAbout aboutDlg;
        aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CSynchronizationExplorerApp commands

int CSynchronizationExplorerApp::ExitInstance() 
{
        if(panelTemplate != NULL)
           delete panelTemplate;
        
        return CWinApp::ExitInstance();
}
