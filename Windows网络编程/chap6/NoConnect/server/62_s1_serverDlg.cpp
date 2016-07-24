// 62_s1_serverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62_s1_server.h"
#include "62_s1_serverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy62_s1_serverDlg dialog

CMy62_s1_serverDlg::CMy62_s1_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy62_s1_serverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy62_s1_serverDlg)
	m_szRecv = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy62_s1_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy62_s1_serverDlg)
	DDX_Text(pDX, IDC_EDIT1, m_szRecv);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy62_s1_serverDlg, CDialog)
	//{{AFX_MSG_MAP(CMy62_s1_serverDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy62_s1_serverDlg message handlers

BOOL CMy62_s1_serverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sockRecv.Create(6801,SOCK_DGRAM,"127.0.0.1");
	m_sockRecv.Bind(6801,"127.0.0.1");
	SetTimer(1,3000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy62_s1_serverDlg::OnPaint() 
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
HCURSOR CMy62_s1_serverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy62_s1_serverDlg::OnTimer(UINT nIDEvent) 
{
	char szRecv[20];
	CString szIP("127.0.0.1");
	UINT uPort=6800;
	int iRecv =m_sockRecv.ReceiveFrom(szRecv,10,szIP,uPort,0);
	TRACE("received %d byte\n",iRecv);
	szRecv[iRecv]='\0';
	m_szRecv=szRecv;
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
