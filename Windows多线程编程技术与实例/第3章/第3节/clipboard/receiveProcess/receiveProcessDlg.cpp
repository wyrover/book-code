// receiveProcessDlg.cpp : implementation file
//

#include "stdafx.h"
#include "receiveProcess.h"
#include "receiveProcessDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define SETFORMATETC(fe,cf,asp,td,med,li) \
	(fe.cfFormat = cf,\
    fe.dwAspect=asp,\
	fe.ptd=td,\
	fe.tymed=med,\
	fe.lindex=li)
/////////////////////////////////////////////////////////////////////////////
// CReceiveProcessDlg dialog

CReceiveProcessDlg::CReceiveProcessDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CReceiveProcessDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CReceiveProcessDlg)
	m_strText2 = _T("");
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CReceiveProcessDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CReceiveProcessDlg)
	DDX_Text(pDX, IDC_EDIT1, m_strText2);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CReceiveProcessDlg, CDialog)
	//{{AFX_MSG_MAP(CReceiveProcessDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_B_RECIEVE, OnBRecieve)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CReceiveProcessDlg message handlers

BOOL CReceiveProcessDlg::OnInitDialog()
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

void CReceiveProcessDlg::OnPaint() 
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
HCURSOR CReceiveProcessDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CReceiveProcessDlg::OnBRecieve() 
{
	// TODO: Add your control notification handler code here
	COleDataObject dataObject;
	VERIFY(dataObject.AttachClipboard());
	pasteText(&dataObject);
	
}

BOOL CReceiveProcessDlg::pasteText(COleDataObject *pDataObject)
{
	FORMATETC fmt;
	STGMEDIUM stg;
	SETFORMATETC(fmt,CF_TEXT,DVASPECT_CONTENT,NULL,TYMED_HGLOBAL,-1);
	VERIFY(pDataObject->GetData(CF_TEXT,&stg,&fmt));
	HGLOBAL hText = stg.hGlobal;
	m_strText2 = (LPSTR)::GlobalLock(hText);
	::GlobalUnlock(hText);
	::GlobalFree(hText);
	UpdateData(FALSE);
	return TRUE;

}
