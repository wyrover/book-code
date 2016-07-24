// WinPingDlg.cpp : implementation file
//

#include "stdafx.h"
#include "WinPing.h"
#include "WinPingDlg.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPingDlg dialog

CWinPingDlg::CWinPingDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CWinPingDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CWinPingDlg)
	m_strRetry = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_nDir = 0;
	m_nPos = 0;
	m_bProgress = FALSE;
	
}

void CWinPingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CWinPingDlg)
	DDX_Control(pDX, IDC_SPIN1, m_spinner);
	DDX_Control(pDX, IDC_LIST, m_list);
	DDX_Text(pDX, IDC_RETRIES, m_strRetry);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CWinPingDlg, CDialog)
	//{{AFX_MSG_MAP(CWinPingDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_PING, OnPing)
	ON_WM_TIMER()
	ON_BN_CLICKED(ID_ABOUT, OnAbout)
	ON_MESSAGE(WM_MSG_STATUS,OnStatus)
	ON_MESSAGE(WM_PING_END,OnPingEnd)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWinPingDlg message handlers

BOOL CWinPingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	CString strHost;
	CString str;


	int nEntrys = AfxGetApp()->GetProfileInt("MRU","Entries",0);
	for (int n=0;n < nEntrys;n++)
	{
		str.Format("Host#%d",n);
		strHost = AfxGetApp()->GetProfileString("MRU",str,"");
		m_cboServers.AddString(strHost);
	}

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	m_img.Create(IDB_LOG,16,1,RGB(255,0,255));



	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	CRect rc;
	CWnd* pWnd;

	pWnd= GetDlgItem(IDC_FROM);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	m_ptFrom = rc.BottomRight();
	m_ptFrom.x+=5;

	pWnd= GetDlgItem(IDC_TO);
	pWnd->GetWindowRect(rc);
	ScreenToClient(rc);

	m_ptTo = CPoint(rc.left,rc.bottom);

	m_list.InsertColumn(0,"Status",LVCFMT_LEFT,800);

	m_list.SetImageList(&m_img, LVSIL_SMALL);

	m_list.GetClientRect(rc);
	m_list.SetColumnWidth(0,rc.Width());

	m_rcRect.left = m_ptFrom.x;
	m_rcRect.right = m_ptTo.x;
	m_rcRect.top = m_rcRect.bottom = m_ptFrom.y;
	m_rcRect.InflateRect(0,3);


	m_spinner.SetRange(1,99);
	m_spinner.SetPos(5);

	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CWinPingDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
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

void CWinPingDlg::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	if (IsIconic())
	{

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
		dc.MoveTo(m_ptFrom);
		dc.LineTo(m_ptTo);
		if (m_bProgress)
		{

			CRect rc(m_nPos,m_ptTo.y,m_nPos,m_ptTo.y);
			rc.InflateRect(3,3);
			CBrush br(RGB(255,0,0));
			CBrush* pBrush = dc.SelectObject(&br);
			dc.Rectangle(rc);
			dc.SelectObject(pBrush);
		}

		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CWinPingDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CWinPingDlg::OnPing() 
{
	UpdateData(TRUE);

	EnableControls(FALSE);

	int nRetries = m_spinner.GetPos();


	int nCnt = m_list.GetItemCount();
	if (nCnt > 0)
	{
		m_list.InsertItem(nCnt,"",4);

	}

	CString strHost;
	
	 GetDlgItem(IDC_SERVERS)->GetWindowText(strHost);
	
	m_pingThread.StartPing(nRetries,strHost,m_hWnd);	

	m_nPos = m_ptFrom.x;
	m_nDir = 20;
	SetTimer(0x1001,100,NULL);
	m_bProgress = TRUE;

}


LRESULT CWinPingDlg::OnStatus(WPARAM wParam, LPARAM lParam)
{
	char* pMsg = (char*) lParam;

	int nCnt = m_list.GetItemCount();
	
	int i = m_list.InsertItem(nCnt,pMsg,wParam);
	m_list.EnsureVisible(i,FALSE);

	delete pMsg;

	return 0;
}

LRESULT CWinPingDlg::OnPingEnd(WPARAM wParam, LPARAM lParam)
{
	EnableControls(TRUE);
	m_bProgress = FALSE;

	InvalidateRect(m_rcRect,TRUE);
	KillTimer(0x1001);
	if (lParam == 1)
	{
		CString strHost;
		GetDlgItem(IDC_SERVERS)->GetWindowText(strHost);
		m_cboServers.AddString(strHost);
		
	}

	return 0;
}

void CWinPingDlg::OnTimer(UINT nIDEvent) 
{
	m_nPos += m_nDir;
	InvalidateRect(m_rcRect,TRUE);
	if (m_nPos > m_ptTo.x)
	{
		m_nDir = -20;
		m_nPos = m_ptTo.x - 5;
	}
	
	if (m_nPos < m_ptFrom.x)
	{
		m_nDir = +20;
		m_nPos = m_ptFrom.x + 5;
	}

	CDialog::OnTimer(nIDEvent);
}

void CWinPingDlg::OnAbout() 
{
	CAboutDlg dlg;

	if (dlg.DoModal())
	{
	

	}
}

void CWinPingDlg::EnableControls(BOOL bEnable)
{
	GetDlgItem(IDC_SERVERS)->EnableWindow(bEnable);
	GetDlgItem(IDC_RETRIES)->EnableWindow(bEnable);
	GetDlgItem(ID_PING)->EnableWindow(bEnable);	
}

void CWinPingDlg::OnCancel() 
{
	CString strHost;
	CString str;

	int nCnt = m_cboServers.GetCount();
	AfxGetApp()->WriteProfileInt("MRU","Entries",nCnt);
	
	for (int n=0;n < nCnt;n++)
	{
		m_cboServers.GetLBText(n,strHost);
		str.Format("Host#%d",n);
		AfxGetApp()->WriteProfileString("MRU",str,strHost);
	}
			
	CDialog::OnCancel();
}
