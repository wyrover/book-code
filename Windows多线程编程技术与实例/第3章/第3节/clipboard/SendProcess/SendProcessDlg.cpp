// SendProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SendProcess.h"
#include "SendProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSendProcessDlg dialog

CSendProcessDlg::CSendProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSendProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSendProcessDlg)
	m_strText1 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSendProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSendProcessDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strText1);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSendProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CSendProcessDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_B_SEND, OnBSend)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSendProcessDlg message handlers

BOOL CSendProcessDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSendProcessDlg::OnPaint() 
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
HCURSOR CSendProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CSendProcessDlg::OnBSend() 
{
	// TODO: Add your control notification handler code here
	COleDataSource* pSource = saveText();
	if (pSource) {
		pSource->SetClipboard();
	}

}

COleDataSource* CSendProcessDlg::saveText()
{
	UpdateData(TRUE);
	if (!m_strText1.IsEmpty()) {
		COleDataSource* pSource = new COleDataSource();
		int nTextSize = m_strText1.GetLength()+1;
		HGLOBAL hText = ::GlobalAlloc(GMEM_SHARE,nTextSize);
		LPSTR pText = (LPSTR)::GlobalLock(hText);
		ASSERT(pText);
		strcpy(pText,m_strText1);
		::GlobalUnlock(hText);
		pSource->CacheGlobalData(CF_TEXT,hText);
		return pSource;
	}
	return NULL;

}
