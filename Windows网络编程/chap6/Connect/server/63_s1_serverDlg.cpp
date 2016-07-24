// 63_s1_serverDlg.cpp : implementation file
//

#include "stdafx.h"
#include "63_s1_server.h"
#include "63_s1_serverDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMy63_s1_serverDlg dialog

CMy63_s1_serverDlg::CMy63_s1_serverDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMy63_s1_serverDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CMy63_s1_serverDlg)
	m_szSend = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMy63_s1_serverDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMy63_s1_serverDlg)
	DDX_Text(pDX, IDC_EDIT1, m_szSend);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CMy63_s1_serverDlg, CDialog)
	//{{AFX_MSG_MAP(CMy63_s1_serverDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMy63_s1_serverDlg message handlers

BOOL CMy63_s1_serverDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	CSocket sockListen;
	sockListen.Create(6802,SOCK_STREAM,"127.0.0.1");
	sockListen.Bind(6801,"127.0.0.1");
	sockListen.Listen(5);
	sockListen.Accept(m_sockSend);
	sockListen.Close();
	SetTimer(1,3000,NULL);
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CMy63_s1_serverDlg::OnPaint() 
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
HCURSOR CMy63_s1_serverDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CMy63_s1_serverDlg::OnTimer(UINT nIDEvent) 
{
	static iIndex=0;	
	char szSend[20];	
	sprintf(szSend,"%010d",iIndex++);	
	//·¢ËÍTCPÊý¾Ý
	int iSend= m_sockSend.Send(szSend,10,0);
	TRACE("sent %d byte\n",iSend);
	m_szSend=szSend;
	UpdateData(FALSE);

	CDialog::OnTimer(nIDEvent);
}
