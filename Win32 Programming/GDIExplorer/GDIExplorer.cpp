// GDI Explorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "randstat.h"
#include "polypoly.h"
#include "GDIExplorer.h"
#include "is95.h"

#include "MainFrm.h"
#include "ChildFrm.h"
#include "mdisp.h"
#include "xform.h"
#include "transform.h"
#include "IntEdit.h"
#include "Source.h"
#include "dest.h" 
#include "idcombo.h"
#include "gcombo.h"
#include "image.h"
#include "bmselect.h"
#include "bitrop.h"
#include "patrop.h"
#include "strmode.h"
#include "stock.h"
#include "color.h"
#include "syscolor.h"
#include "brstyle.h"
#include "hatchsmp.h"
#include "brdemo.h"

#include "orgext.h"
#include "blt.h"
#include "genblt.h"
#include "box.h"

#include "about.h"

#include "GDIExplorerDoc.h"
#include "GDIExplorerView.h"

#include "ctl.h"
#include "bitblt.h"
#include "patblt.h"
#include "plgblt.h"
#include "strblt.h"
#include "mskblt.h"
#include "brushsel.h"
#include "ropfind.h"
#include "DIBits.h"

#include "rgndisp.h"
#include "rgnint.h"
#include "rgncont.h"
#include "showrgn.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerApp

BEGIN_MESSAGE_MAP(CGDIExplorerApp, CWinApp)
        //{{AFX_MSG_MAP(CGDIExplorerApp)
        ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
        ON_COMMAND(IDM_BLTEXP, OnBltexp)
        ON_COMMAND(IDM_REGION, OnRgnExp)
        ON_UPDATE_COMMAND_UI(IDM_MATEXP, OnUpdateMatexp)
        ON_COMMAND(IDM_MATEXP, OnXform) // Not yet implemented
	//}}AFX_MSG_MAP
        // Standard file based document commands
        ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
        ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
        // Standard print setup command
        ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerApp construction

CGDIExplorerApp::CGDIExplorerApp()
{
        // TODO: add construction code here,
        // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CGDIExplorerApp object

CGDIExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerApp initialization

BOOL CGDIExplorerApp::InitInstance()
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
                IDR_GDIEXPTYPE,
                RUNTIME_CLASS(CGDIExplorerDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CGDIExplorerView));
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
        if (!ProcessShellCommand(cmdInfo))
                return FALSE;

        // The main window has been initialized, so show and update it.
        pMainFrame->ShowWindow(m_nCmdShow);
        pMainFrame->UpdateWindow();

        return TRUE;
}

/////////////////////////////////////////////////////////////////////////////

// App command to run the dialog
void CGDIExplorerApp::OnAppAbout()
{
        CAbout aboutDlg;
        aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CGDIExplorerApp commands


void CGDIExplorerApp::OnXform() 
{
 CTransform dlg;
 dlg.DoModal();
}

void CGDIExplorerApp::OnRgnExp()
{
 CShowRegion dlg;
 dlg.DoModal();
}

void CGDIExplorerApp::OnBltexp() 
{
 CPropertySheet sheet;
 CBitBlt bitblt;
 CPatBlt patblt;
 CPlgBlt plgblt;
 CStretchBlt strblt;
 CMaskBlt maskblt;
 CDIBits dibits;
 CBrushSelector brush;
 CRopFinder ropfinder;

 sheet.AddPage(&bitblt);
 sheet.AddPage(&patblt);

 if(!Is95())                    // only supported on NT
    sheet.AddPage(&plgblt);

 sheet.AddPage(&strblt);

 if(!Is95())
    sheet.AddPage(&maskblt);    // only supported on NT

 sheet.AddPage(&ropfinder);
 sheet.AddPage(&dibits);
 sheet.AddPage(&brush);
 
 sheet.DoModal();
        
}

void CGDIExplorerApp::OnUpdateMatexp(CCmdUI* pCmdUI) 
{
#if 0
	pCmdUI->Enable(!Is95());
#else
	pCmdUI->Enable(FALSE); // not yet implemented
#endif
	
}
