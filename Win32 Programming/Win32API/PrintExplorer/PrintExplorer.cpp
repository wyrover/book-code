// PrintExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PrintExplorer.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "PrintExplorerDoc.h"
#include "PrintExplorerView.h"

#include "about.h"

#include "HandleEdit.h"
#include "NumericEdit.h"
#include "EnumEdit.h"
#include "commerr.h"
#include "bitcombo.h"
#include "message.h"
#include "hdevmode.h"
#include "devnames.h"
#include "resname.h"
#include "scombo.h"
#include "pdata.h"
#include "pname.h"
#include "psizes.h"
#include "papers.h"

#include "printdlgsetup.h"
#include "printflags.h"
#include "devmode.h"
#include "setdev.h"
#include "devcaps.h"
#include "printit.h"
#include "MsgLog.h"
#include "pgsetup.h"
#include "hdng.h"
#include "prt.h"
#include "devcapa.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerApp

BEGIN_MESSAGE_MAP(CPrintExplorerApp, CWinApp)
        //{{AFX_MSG_MAP(CPrintExplorerApp)
        ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
        //}}AFX_MSG_MAP
        // Standard file based document commands
        ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
        ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
        // Standard print setup command
        ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerApp construction

CPrintExplorerApp::CPrintExplorerApp()
{
 memset(&pd, 0, sizeof(pd));
 pd.lStructSize = sizeof(pd);
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CPrintExplorerApp object

CPrintExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerApp initialization

BOOL CPrintExplorerApp::InitInstance()
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
                IDR_PRINTETYPE,
                RUNTIME_CLASS(CPrintExplorerDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CPrintExplorerView));
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

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About


// App command to run the dialog
void CPrintExplorerApp::OnAppAbout()
{
        CAbout aboutDlg;
        aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CPrintExplorerApp commands

/****************************************************************************
*                            CPrintExplorerApp::DoPrintDlg
* Inputs:
*       CView * view: View to pass in, or NULL
* Result: void
*       
* Effect: 
*       Initiates the Print Explorer, passing in the view whose contents
*       are to be printed, or NULL if called from main frame
* Notes:
*       View is not currently used; must fix this someday
****************************************************************************/

void CPrintExplorerApp::doPrintDlg(CView * view)
    {
     CPropertySheet sheet;
     CPrintDlgSetup dlg;
     CPrintFlags flags;
     CDevMode devmode;
     CSetDev setdev;
     CDevCaps devcaps;
     CPgSetup pgsetup;
     CPrt prt;
     CDeviceCapabilities devcapabilities;

     int result = 0;

     dlg.pd = pd; // make copy of current PRINTDLG values
     flags.pd = &dlg.pd;
     devmode.pd = &dlg.pd;
     setdev.pd = &dlg.pd;
     devcaps.pd = &dlg.pd;
     prt.pd = &dlg.pd;
     devcapabilities.pd = &dlg.pd;
     pgsetup.prt = &prt.prt;

     dlg.result = &result;
     flags.result = &result;

     sheet.AddPage(&dlg);
     sheet.AddPage(&flags);
     sheet.AddPage(&devmode);
     sheet.AddPage(&setdev);
     sheet.AddPage(&devcaps);
     sheet.AddPage(&devcapabilities);
     sheet.AddPage(&pgsetup);
     sheet.AddPage(&prt);

     if(sheet.DoModal() == IDOK)
        pd = dlg.pd;
     
    }

