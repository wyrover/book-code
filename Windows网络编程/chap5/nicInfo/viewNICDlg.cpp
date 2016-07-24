// viewNICDlg.cpp : implementation file
//

#include "stdafx.h"
#include "viewNIC.h"
#include "viewNICDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewNICDlg dialog

CViewNICDlg::CViewNICDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CViewNICDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CViewNICDlg)
	m_NICInfoDescription = _T("");
	m_NICInfoIP = _T("");
	m_NICInfoMAC = _T("");
	m_NICInfoNum = 0;
	m_NICInfoSubnetMask = _T("");
	m_NICInfoType = 0;
	m_bDialup = FALSE;
	m_bLoopback = FALSE;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewNICDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewNICDlg)
	DDX_Text(pDX, IDC_NIC_INFO_DESCRIPTION, m_NICInfoDescription);
	DDX_Text(pDX, IDC_NIC_INFO_IP, m_NICInfoIP);
	DDX_Text(pDX, IDC_NIC_INFO_MAC, m_NICInfoMAC);
	DDX_Text(pDX, IDC_NIC_INFO_NUM, m_NICInfoNum);
	DDX_Text(pDX, IDC_NIC_INFO_SUBNET_MASK, m_NICInfoSubnetMask);
	DDX_Text(pDX, IDC_NIC_INFO_TYPE, m_NICInfoType);
	DDX_Check(pDX, IDC_CHECK_DIALUP, m_bDialup);
	DDX_Check(pDX, IDC_CHECK_LOOPBACK, m_bLoopback);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CViewNICDlg, CDialog)
	//{{AFX_MSG_MAP(CViewNICDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_NIC_NEXT, OnNicNext)
	ON_BN_CLICKED(IDC_NIC_PREVIOUS, OnNicPrevious)
	ON_BN_CLICKED(IDC_NIC_REFRESH, OnNicRefresh)
	ON_WM_WINDOWPOSCHANGED()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewNICDlg message handlers

BOOL CViewNICDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_NICIndex = 0;
	m_pNICInfo = NULL;
	GetDlgItem(IDC_NIC_REFRESH)->SetFocus();
	
//	return TRUE;  // return TRUE unless you set the focus to a control
	return FALSE;
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CViewNICDlg::OnPaint() 
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
HCURSOR CViewNICDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CViewNICDlg::OnNicNext() 
{
	UpdateData();
	m_NICIndex++;
	DisplayNICInfo(m_NICIndex, &m_pNICInfo[m_NICIndex]);

	if (m_NICIndex == m_NICCount - 1)
		GetDlgItem(IDC_NIC_NEXT)->EnableWindow(FALSE);
	GetDlgItem(IDC_NIC_PREVIOUS)->EnableWindow(TRUE);	
}

void CViewNICDlg::OnNicPrevious() 
{
	UpdateData();
	m_NICIndex--;
	DisplayNICInfo(m_NICIndex, &m_pNICInfo[m_NICIndex]);

	if (m_NICIndex == 0)
		GetDlgItem(IDC_NIC_PREVIOUS)->EnableWindow(FALSE);
	GetDlgItem(IDC_NIC_NEXT)->EnableWindow(TRUE);
}

int CViewNICDlg::GetNICInfo()
{
	MibII		m;

	int rv = m.Init();
	if (rv < 0)
		return rv;

	UpdateData();
	m_NICCount = m.GetNICCount(m_bDialup, m_bLoopback);
	if (m_NICCount > 0)
	{
		if (m_NICCount > 1)
			GetDlgItem(IDC_NIC_NEXT)->EnableWindow(TRUE);
		if (m_pNICInfo)
			delete m_pNICInfo;
		m_pNICInfo = new tSTRUCTNICINFO [m_NICCount];
		m.GetNICInfo(m_pNICInfo);
	}

	return m_NICCount;
}

void CViewNICDlg::DisplayNICInfo(UINT NICIndex, tSTRUCTNICINFO *pNICInfo)
{
	BYTE	*p, buf[128];

	m_NICInfoNum = NICIndex;
	m_NICInfoDescription = pNICInfo->Description;
	m_NICInfoIP.Format("%d.%d.%d.%d", pNICInfo->IP[0], pNICInfo->IP[1], pNICInfo->IP[2], pNICInfo->IP[3]);
	m_NICInfoSubnetMask.Format("%d.%d.%d.%d", pNICInfo->SubnetMask[0], pNICInfo->SubnetMask[1], 
		pNICInfo->SubnetMask[2], pNICInfo->SubnetMask[3]);

	memset(buf, 0, sizeof(buf));
	p = buf;
	for (UINT i = 0; i < pNICInfo->MACLength; i++)
	{
		sprintf((char *) p, "%02X-", pNICInfo->MAC[i]);
		p += 3;
	}
	if (pNICInfo->MACLength > 0)
		*(p - 1) = 0;
	m_NICInfoMAC.Format("%s", buf);

	m_NICInfoType = pNICInfo->type;

	UpdateData(FALSE);
}

void CViewNICDlg::OnNicRefresh() 
{
	GetDlgItem(IDC_NIC_PREVIOUS)->EnableWindow(FALSE);
	GetDlgItem(IDC_NIC_NEXT)->EnableWindow(FALSE);

	int rv = GetNICInfo();
	if (rv > 0)
	{
		m_NICIndex = 0;
		DisplayNICInfo(m_NICIndex, &m_pNICInfo[m_NICIndex]);
	}
}

void CViewNICDlg::OnWindowPosChanged(WINDOWPOS FAR* lpwndpos) 
{
	CDialog::OnWindowPosChanged(lpwndpos);
	
// This case only happens when the main dialog box is first created.
// It's one way to execute some code AFTER the main dialog box has been drawn.
	if (lpwndpos->flags & SWP_SHOWWINDOW)
	{
		int rv = GetNICInfo();
		switch (rv)
		{
			case ERROR_MIB_DLL:
				MessageBox("Couldn't load INETMIB1.DLL!", "MIB Error", MB_ICONSTOP);
				return;
			case ERROR_MIB_WINSOCK:
				MessageBox("Couldn't load Winsock DLL!", "MIB Error", MB_ICONSTOP);
				return;
			case ERROR_MIB_INIT:
				MessageBox("Couldn't initialize MIB!", "MIB Error", MB_ICONSTOP);
				return;
			default:
				if (rv > 0)
					DisplayNICInfo(m_NICIndex, &m_pNICInfo[m_NICIndex]);
				break;
		}
	}
}

void CViewNICDlg::OnDestroy() 
{
	CDialog::OnDestroy();
	
	// TODO: Add your message handler code here
	if (m_pNICInfo)
		delete m_pNICInfo;
}
