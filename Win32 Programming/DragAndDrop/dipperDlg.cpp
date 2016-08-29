// dipperDlg.cpp : implementation file
//

#include "stdafx.h"
#include "dipper.h"
#include "bucket.h"
#include "glass.h"
#include "status.h"
#include "about.h"

#include "dipperDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDipperDlg dialog

CDipperDlg::CDipperDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDipperDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDipperDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDipperDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDipperDlg)
	DDX_Control(pDX, IDC_STATUS, c_Status);
	DDX_Control(pDX, IDC_GLASS5, c_Glass5);
	DDX_Control(pDX, IDC_GLASS4, c_Glass4);
	DDX_Control(pDX, IDC_GLASS3, c_Glass3);
	DDX_Control(pDX, IDC_GLASS2, c_Glass2);
	DDX_Control(pDX, IDC_GLASS1, c_Glass1);
	DDX_Control(pDX, IDC_BUCKET, c_Bucket);
	DDX_Control(pDX, IDOK, c_Close);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDipperDlg, CDialog)
	//{{AFX_MSG_MAP(CDipperDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SETCURSOR()
	ON_REGISTERED_MESSAGE(UWM_Status, doStatus)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDipperDlg message handlers

BOOL CDipperDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	dipper = AfxGetApp()->LoadCursor(IDC_EMPTY_DIPPER);

	SendMessage(UWM_Status, IDS_READY);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	CString strAboutMenu;
	strAboutMenu.LoadString(IDS_ABOUTBOX);
	if (!strAboutMenu.IsEmpty())
	{
		pSysMenu->AppendMenu(MF_SEPARATOR);
		pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDipperDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAbout dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDipperDlg::OnPaint() 
{
	if (IsIconic())
	{
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
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDipperDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

BOOL CDipperDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
 	if(nHitTest == HTCLIENT)
	   { /* use dipper */
	    POINT pt;
	    ::GetCursorPos(&pt);
	    HWND hwnd = ::WindowFromPoint(pt);
	    if(hwnd != c_Close.m_hWnd)
	       { /* not in close button */
		SetCursor(dipper);
		return 0;
	       } /* not in close button */
	   } /* use dipper */
	
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

/****************************************************************************
*                            CDipperDlg::doStatus
* Inputs:
*       WPARAM id: Message id
*	LPARAM: ignored
* Result: LRESULT
*       0, always
* Effect: 
*       Displays the message in the status line.  Handles caching so that
*	we don't get message flicker
****************************************************************************/

LRESULT CDipperDlg::doStatus(WPARAM id, LPARAM)
    {
     if(id == (WPARAM)::GetWindowLong(this->m_hWnd, GWL_USERDATA))
	return 0;

     CString s;
     s.LoadString(id);
     c_Status.SetWindowText(s);
     ::SetWindowLong(this->m_hWnd, GWL_USERDATA, id);
     return 0;
    }
