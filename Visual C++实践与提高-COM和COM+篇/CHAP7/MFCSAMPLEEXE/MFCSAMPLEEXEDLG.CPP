// MFCSampleExeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MFCSampleExe.h"
#include "MFCSampleExeDlg.h"
#include "DlgProxy.h"

#ifdef _DEBUG
    #define new DEBUG_NEW
    #undef THIS_FILE
    static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeDlg dialog

IMPLEMENT_DYNAMIC(CMFCSampleExeDlg, CDialog);

CMFCSampleExeDlg::CMFCSampleExeDlg(CWnd* pParent /*=NULL*/)
    : CDialog(CMFCSampleExeDlg::IDD, pParent)
{
    //{{AFX_DATA_INIT(CMFCSampleExeDlg)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
    // Note that LoadIcon does not require a subsequent DestroyIcon in Win32
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_pAutoProxy = NULL;
}

CMFCSampleExeDlg::~CMFCSampleExeDlg()
{
    // If there is an automation proxy for this dialog, set
    //  its back pointer to this dialog to NULL, so it knows
    //  the dialog has been deleted.
    if (m_pAutoProxy != NULL)
        m_pAutoProxy->m_pDialog = NULL;
}

void CMFCSampleExeDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CMFCSampleExeDlg)
    // NOTE: the ClassWizard will add DDX and DDV calls here
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMFCSampleExeDlg, CDialog)
    //{{AFX_MSG_MAP(CMFCSampleExeDlg)
    ON_WM_CLOSE()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    //}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMFCSampleExeDlg message handlers

BOOL CMFCSampleExeDlg::OnInitDialog()
{
    CDialog::OnInitDialog();
    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);         // Set big icon
    SetIcon(m_hIcon, FALSE);        // Set small icon
    // TODO: Add extra initialization here
    return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMFCSampleExeDlg::OnPaint()
{
    if (IsIconic()) {
        CPaintDC dc(this); // device context for painting
        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);
        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;
        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    } else {
        CDialog::OnPaint();
    }
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CMFCSampleExeDlg::OnQueryDragIcon()
{
    return (HCURSOR) m_hIcon;
}

// Automation servers should not exit when a user closes the UI
//  if a controller still holds on to one of its objects.  These
//  message handlers make sure that if the proxy is still in use,
//  then the UI is hidden but the dialog remains around if it
//  is dismissed.

void CMFCSampleExeDlg::OnClose()
{
    if (CanExit())
        CDialog::OnClose();
}

void CMFCSampleExeDlg::OnOK()
{
    if (CanExit())
        CDialog::OnOK();
}

void CMFCSampleExeDlg::OnCancel()
{
    if (CanExit())
        CDialog::OnCancel();
}

BOOL CMFCSampleExeDlg::CanExit()
{
    // If the proxy object is still around, then the automation
    //  controller is still holding on to this application.  Leave
    //  the dialog around, but hide its UI.
    if (m_pAutoProxy != NULL) {
        ShowWindow(SW_HIDE);
        return FALSE;
    }

    return TRUE;
}
