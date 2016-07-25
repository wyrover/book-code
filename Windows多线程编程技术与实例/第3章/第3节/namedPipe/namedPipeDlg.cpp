// namedPipeDlg.cpp : implementation file
//

#include "stdafx.h"
#include "namedPipe.h"
#include "namedPipeDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeDlg dialog

CNamedPipeDlg::CNamedPipeDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CNamedPipeDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CNamedPipeDlg)
	m_szPipeName = _T("test");
	m_sz2Send = _T("");
	m_szServerName = _T("");
	m_szRX = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CNamedPipeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CNamedPipeDlg)
	DDX_Control(pDX, IDC_EDIT_SERVER_NAME, m_ctrlServerName);
	DDX_Control(pDX, IDC_B_SEND, m_ctrlSend);
	DDX_Control(pDX, IDC_B_CONNECT, m_ctrlConnect);
	DDX_Text(pDX, IDC_EDIT_PIPENAME, m_szPipeName);
	DDX_Text(pDX, IDC_EDIT_SEND, m_sz2Send);
	DDX_Text(pDX, IDC_EDIT_SERVER_NAME, m_szServerName);
	DDX_Text(pDX, IDC_STATIC_RX, m_szRX);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CNamedPipeDlg, CDialog)
	//{{AFX_MSG_MAP(CNamedPipeDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_B_CONNECT, OnBConnect)
	ON_BN_CLICKED(IDC_RB_CLIENT, OnRbClient)
	ON_BN_CLICKED(IDC_RB_SERVER, OnRbServer)
	ON_BN_CLICKED(IDC_B_SEND, OnBSend)
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_USER+1,CallUpdateData)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CNamedPipeDlg message handlers

BOOL CNamedPipeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_ctrlConnect.EnableWindow(TRUE);
	m_ctrlSend.EnableWindow(FALSE);
	m_bReady = false;
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CNamedPipeDlg::OnPaint() 
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
HCURSOR CNamedPipeDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CNamedPipeDlg::OnBConnect() 
{
	if (!m_bReady)
	{
		MessageBox("please select server or client");
		return;
	}
	
	UpdateData(TRUE);

	m_np.SetPipeName(m_szPipeName.GetBuffer(1),m_bIsServer?".":m_szServerName.GetBuffer(1));

	if (!m_np.Initialize(m_bIsServer,"QUIT",&funcCallBack))
	{
		MessageBox("INIT FAILED");
		return;
	}
	m_ctrlConnect.EnableWindow(FALSE);
	if (!m_bIsServer) {
		m_ctrlSend.EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_SEND)->EnableWindow(TRUE);
	}
	
	UpdateData(FALSE);
}

void CNamedPipeDlg::OnRbClient() 
{
	m_bReady = true;
	UpdateData(TRUE);
	m_bIsServer = false;
	m_ctrlServerName.EnableWindow(TRUE);
	m_szServerName = ".";
	UpdateData(FALSE);
}

void CNamedPipeDlg::OnRbServer() 
{
	m_bReady = true;
	UpdateData(TRUE);
	m_bIsServer = true;
	m_ctrlServerName.EnableWindow(FALSE);
	m_szServerName = "";
	UpdateData(FALSE);
}

void CNamedPipeDlg::OnBSend() 
{
	UpdateData(TRUE);	
	bool b = m_np.Send(m_sz2Send.GetBuffer(1));
	if (!b)
		MessageBox("SEND FAIL");
}

void WINAPI CNamedPipeDlg::funcCallBack(string buf)
{
	::SendMessage(*AfxGetMainWnd(),WM_USER+1,0,(LPARAM)buf.c_str());// 
}

afx_msg LRESULT CNamedPipeDlg::CallUpdateData(WPARAM wP, LPARAM lP)
{
	CString str = (char*)lP;
	m_szRX  = m_szRX+"	"+str;
	GetDlgItem(IDC_STATIC_RX)->SetWindowText(m_szRX);
	
	MessageBeep(-1);//The MessageBeep function plays a waveform sound.
	return 0;
}
