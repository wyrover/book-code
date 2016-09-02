// ServerSocket.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "ServerSocket.h"
#include "ServerSocketDlg.h"
#include "ClientSocketDlg.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

#define WSA_VERSION  MAKEWORD(2,0)
#define MAX_HOSTNAME 256
#define MAX_HOSTADDR 40

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp

BEGIN_MESSAGE_MAP(CServerSocketApp, CWinApp)
    //{{AFX_MSG_MAP(CServerSocketApp)
    // NOTE - the ClassWizard will add and remove mapping macros here.
    //    DO NOT EDIT what you see in these blocks of generated code!
    //}}AFX_MSG
    ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp construction

CServerSocketApp::CServerSocketApp()
{
    // TODO: add construction code here,
    // Place all significant initialization in InitInstance
    m_nLinkMode = 0; // server
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CServerSocketApp object

CServerSocketApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CServerSocketApp initialization

BOOL CServerSocketApp::InitInstance()
{
    WSADATA     WSAData = { 0 };

    if (0 != WSAStartup(WSA_VERSION, &WSAData)) {
        // Tell the user that we could not find a usable
        // WinSock DLL.
        if (LOBYTE(WSAData.wVersion) != LOBYTE(WSA_VERSION) ||
            HIBYTE(WSAData.wVersion) != HIBYTE(WSA_VERSION))
            ::MessageBox(NULL, _T("Incorrect version of WS2_32.dll found"), _T("Error"), MB_OK);

        WSACleanup();
        return FALSE;
    }

    AfxEnableControlContainer();
    // Standard initialization
    // If you are not using these features and wish to reduce the size
    //  of your final executable, you should remove from the following
    //  the specific initialization routines you do not need.
#if _MSC_VER < 1200
#ifdef _AFXDLL
    Enable3dControls();         // Call this when using MFC in a shared DLL
#else
    Enable3dControlsStatic();   // Call this when linking to MFC statically
#endif
#endif
    ParseCommandLineArgs();
    CServerSocketDlg dlg1;
    CClientSocketDlg dlg2;

    switch (m_nLinkMode) {
    default:
    case 0:
        m_pMainWnd = &dlg1; // Server
        break;

    case 1:
        m_pMainWnd = &dlg2; // Client
        break;
    }

    int nResponse = ((CDialog*)m_pMainWnd)->DoModal();

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

int CServerSocketApp::ExitInstance()
{
    // Terminate use of the WS2_32.DLL
    WSACleanup();
    return CWinApp::ExitInstance();
}

void CServerSocketApp::ParseCommandLineArgs()
{
    CString strCmdLine = m_lpCmdLine;

    if (!strCmdLine.IsEmpty()) {
        strCmdLine.MakeUpper();

        while (!strCmdLine.IsEmpty()) {
            CString strCurrent = strCmdLine;
            int nNextPos = strCmdLine.Find(TCHAR(' '));

            if (nNextPos > 0) {
                strCurrent = strCmdLine.Left(nNextPos);
                strCmdLine.Delete(0, nNextPos + 1);
            } else {
                strCurrent = strCmdLine;
                strCmdLine = _T("");
            }

            if (strCurrent == _T("/SERVER") || strCurrent == _T("/S"))
                m_nLinkMode = 0;
            else if (strCurrent == _T("/CLIENT") || strCurrent == _T("/C"))
                m_nLinkMode = 1;
        }
    }
}


