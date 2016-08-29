// FontExplorer.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "FontExplorer.h"
#include <dlgs.h>

#include "IntEdit.h"
#include "about.h"
#include "MainFrm.h"

#include "ChildFrm.h"
#include "FontExplorerDoc.h"
#include "FontExplorerView.h"

#include "poplist.h"
#include "legendi.h"
#include "legend.h"
#include "dispopts.h"
#include "fontdesc.h"
#include "fontmet.h"
#include "fontdisp.h"
#include "fontexp.h"
#include "idcombo.h"
#include "degrees.h"
#include "rotate.h"
#include "message.h"
#include "sample.h"
#include "fontname.h"
#include "lfexp.h"
#include "fontobj.h"
#include "fontsel.h"
#include "fontopt.h"
#include "kerning.h"
#include "cplace.h"
#include "raster.h"
#include "stockfnt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

static CFontObj InfoFontObj("Info", MAKEINTRESOURCE(IDS_FONTINFO), NULL);


/////////////////////////////////////////////////////////////////////////////
// CFontExplorerApp

BEGIN_MESSAGE_MAP(CFontExplorerApp, CWinApp)
        //{{AFX_MSG_MAP(CFontExplorerApp)
        ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
        ON_COMMAND(IDM_FONTEXPLORER, OnFontexplorer)
        ON_COMMAND(IDM_LOGFONT, OnLogfont)
        ON_COMMAND(IDM_FONTSETUP, OnFontsetup)
        ON_COMMAND(IDM_KERNEXP, OnKernexp)
        ON_COMMAND(IDM_GETCHARPLACEMENT, OnGetcharplacement)
        ON_UPDATE_COMMAND_UI(IDM_GETCHARPLACEMENT, OnUpdateGetcharplacement)
        ON_COMMAND(ID_RASTER, OnRaster)
        ON_COMMAND(IDM_STOCKFONT, OnStockfont)
        //}}AFX_MSG_MAP
        // Standard file based document commands
        ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
        ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
        // Standard print setup command
        ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerApp construction

CFontExplorerApp::CFontExplorerApp()
{
        // TODO: add construction code here,
        // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CFontExplorerApp object

CFontExplorerApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerApp initialization

BOOL CFontExplorerApp::InitInstance()
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
                IDR_FONTEXTYPE,
                RUNTIME_CLASS(CFontExplorerDoc),
                RUNTIME_CLASS(CChildFrame), // custom MDI child frame
                RUNTIME_CLASS(CFontExplorerView));
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

// App command to run the dialog
void CFontExplorerApp::OnAppAbout()
{
        CAbout aboutDlg;
        aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CFontExplorerApp commands

void CFontExplorerApp::OnFontexplorer() 
{
 CFontExplorer dlg;

 dlg.m_cf.lpTemplateName = MAKEINTRESOURCE(FORMATDLGORD31);
 dlg.m_cf.hInstance = AfxGetInstanceHandle();
 dlg.m_cf.Flags |= CF_ENABLETEMPLATE;
 dlg.DoModal();
}

void CFontExplorerApp::OnLogfont() 
{
 CLogFontExplorer dlg;
 dlg.DoModal();
}

void CFontExplorerApp::OnFontsetup() 
{
 FontOptions(AfxGetMainWnd());  
}

void CFontExplorerApp::OnKernexp() 
{
 CKerning dlg;
 dlg.DoModal();
}

void CFontExplorerApp::OnGetcharplacement() 
{
 CCharacterPlacement dlg;

 dlg.DoModal();
}

void CFontExplorerApp::OnUpdateGetcharplacement(CCmdUI* pCmdUI) 
{
 // Not implemented on Win/NT 3.x
 OSVERSIONINFO info;

 info.dwOSVersionInfoSize = sizeof(info);
 GetVersionEx(&info);

 pCmdUI->Enable(!(info.dwMajorVersion == 3));

 // Well, almost.  We didn't get to complete the GetCharPlacement
 // Explorer in time for the CD-ROM mastering.  Look on the Web site
 // for future releases...

 // But for now...
 pCmdUI->Enable(FALSE);
}

void CFontExplorerApp::OnRaster() 
{
 CRaster dlg;
 dlg.DoModal();
}

void CFontExplorerApp::OnStockfont() 
{
 CStockFont dlg;
 dlg.DoModal();
        
}
