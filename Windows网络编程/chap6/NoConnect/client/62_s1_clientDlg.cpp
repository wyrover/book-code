// 62_s1_clientDlg.cpp : implementation file
//

#include "stdafx.h"
#include "62_s1_client.h"
#include "62_s1_clientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy62_s1_clientDlg dialog

CMy62_s1_clientDlg::CMy62_s1_clientDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy62_s1_clientDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy62_s1_clientDlg)
	m_szSend = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy62_s1_clientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy62_s1_clientDlg)
	DDX_Text(pDX, IDC_EDIT1, m_szSend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy62_s1_clientDlg, CDialog)
	//{{AFX_MSG_MAP(CMy62_s1_clientDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy62_s1_clientDlg message handlers

BOOL CMy62_s1_clientDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_sockSend.Create(6800,SOCK_DGRAM,NULL);
	m_sockSend.Bind(6800,"127.0.0.1");
	SetTimer(1,3000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy62_s1_clientDlg::OnPaint() 
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
HCURSOR CMy62_s1_clientDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy62_s1_clientDlg::OnTimer(UINT nIDEvent) 
{
	static iIndex=0;
	char szSend[20];
	sprintf(szSend,"%010d",iIndex++);
	int iSend= m_sockSend.SendTo(szSend,10,6801,"127.0.0.1",0);
	TRACE("sent %d byte\n",iSend);
	m_szSend=szSend;
	UpdateData(FALSE);
	CDialog::OnTimer(nIDEvent);
}
