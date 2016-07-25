// DemoWindowsXpBootDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DemoWindowsXpBoot.h"
#include "DemoWindowsXpBootDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDemoWindowsXpBootDlg dialog

CDemoWindowsXpBootDlg::CDemoWindowsXpBootDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CDemoWindowsXpBootDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDemoWindowsXpBootDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDemoWindowsXpBootDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDemoWindowsXpBootDlg)
	DDX_Control(pDX, IDC_BOOTBAR, m_ctrlDynBar);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CDemoWindowsXpBootDlg, CDialog)
	//{{AFX_MSG_MAP(CDemoWindowsXpBootDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ID_BUT_BOOT, OnButBoot)
	ON_BN_CLICKED(ID_BUT_STOP, OnButStop)
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDemoWindowsXpBootDlg message handlers

BOOL CDemoWindowsXpBootDlg::OnInitDialog()
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

void CDemoWindowsXpBootDlg::OnPaint() 
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
HCURSOR CDemoWindowsXpBootDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CDemoWindowsXpBootDlg::OnButBoot() 
{
	// TODO: Add your control notification handler code here
		m_ctrlDynBar.Start();
	
}

void CDemoWindowsXpBootDlg::OnButStop() 
{
	// TODO: Add your control notification handler code here
	m_ctrlDynBar.Stop();
	
}

HBRUSH CDemoWindowsXpBootDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{

	
 	pDC->SetTextColor(RGB(160,180,250));
	pDC->SetBkColor(RGB(0,0,0));
	HBRUSH br=CreateSolidBrush(RGB(0,0,0));
	return br;


}

void CDemoWindowsXpBootDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct) 
{

  SetBkColor(lpDrawItemStruct->hDC,RGB(0,0,0));

   // Get the button's text.
   CString strText;
   if (nIDCtl == ID_BUT_BOOT) { 
	   GetDlgItem(ID_BUT_BOOT)->GetWindowText(strText);
   }
   else 
	   GetDlgItem(ID_BUT_STOP)->GetWindowText(strText);


   // Draw the button text using the text color white.
   COLORREF crOldColor = ::SetTextColor(lpDrawItemStruct->hDC, RGB(50,50,50));
   ::DrawText(lpDrawItemStruct->hDC, strText, strText.GetLength(), 
      &lpDrawItemStruct->rcItem, DT_SINGLELINE|DT_VCENTER|DT_CENTER);
   ::SetTextColor(lpDrawItemStruct->hDC, crOldColor);

	
	CDialog::OnDrawItem(nIDCtl, lpDrawItemStruct);
}
