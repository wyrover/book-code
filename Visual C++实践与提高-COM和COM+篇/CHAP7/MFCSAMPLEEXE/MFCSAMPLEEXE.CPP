// MFCSampleExe.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MFCSampleExe.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeApp

BEGIN_MESSAGE_MAP(CMFCSampleExeApp, CWinApp)
    //{{AFX_MSG_MAP(CMFCSampleExeApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeApp construction

CMFCSampleExeApp::CMFCSampleExeApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMFCSampleExeApp object

CMFCSampleExeApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeApp initialization

BOOL CMFCSampleExeApp::InitInstance()
{
    // Initialize OLE libraries
    if (!AfxOleInit()) {
        AfxMessageBox(IDP_OLE_INIT_FAILED);
        return FALSE;
    }

    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.

    // Parse the command line to see if launched as OLE server
    if (RunEmbedded() || RunAutomated()) {
        // Register all OLE server (factories) as running.  This enables the
        //  OLE libraries to create objects from other applications.
        COleTemplateServer::RegisterAll();
        return true;
    } else {
        // When a server application is launched stand-alone, it is a good idea
        //  to update the system registry in case it has been damaged.
        COleObjectFactory::UpdateRegistryAll();
        AfxMessageBox("register success");
    }

    return FALSE;
}
