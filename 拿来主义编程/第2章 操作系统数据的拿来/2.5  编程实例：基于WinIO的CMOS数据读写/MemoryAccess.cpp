// MemoryAccess.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "MemoryAccess.h"
#include "MemoryAccessDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMemoryAccessApp

BEGIN_MESSAGE_MAP(CMemoryAccessApp, CWinApp)
    //{{AFX_MSG_MAP(CMemoryAccessApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMemoryAccessApp construction

CMemoryAccessApp::CMemoryAccessApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CMemoryAccessApp object

CMemoryAccessApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMemoryAccessApp initialization

BOOL CMemoryAccessApp::InitInstance()
{
    AfxEnableControlContainer();
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
    SetDialogBkColor(RGB(0x2F, 0x4F, 0x4F), RGB(0xFF, 0xFF, 0xFF));
    CMemoryAccessDlg dlg;
    m_pMainWnd = &dlg;
    int nResponse = dlg.DoModal();

    if (nResponse == IDOK) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with OK
    } else if (nResponse == IDCANCEL) {
        // TODO: Place code here to handle when the dialog is
        //  dismissed with Cancel
    }

    // Since the dialog has been closed, return FALSE so that we exit the
    //  application, rather than start the application's message pump.
    return FALSE;
}
