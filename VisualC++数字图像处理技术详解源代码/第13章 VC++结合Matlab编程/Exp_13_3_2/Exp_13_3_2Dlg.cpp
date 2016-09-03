// Exp_13_3_2Dlg.cpp : implementation file
//

#include "stdafx.h"
#include "Exp_13_3_2.h"
#include "Exp_13_3_2Dlg.h"
#include "myfind.hpp"

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
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExp_13_3_2Dlg dialog

CExp_13_3_2Dlg::CExp_13_3_2Dlg(CWnd* pParent /*=NULL*/)
	: CDialog(CExp_13_3_2Dlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CExp_13_3_2Dlg)
	m_data = _T("");
	m_data2 = _T("");
	m_num = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require m_pA subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExp_13_3_2Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CExp_13_3_2Dlg)
	DDX_Text(pDX, IDC_EDIT_DATA, m_data);
	DDX_Text(pDX, IDC_EDIT_DATA2, m_data2);
	DDX_Text(pDX, IDC_EDIT_NUM, m_num);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CExp_13_3_2Dlg, CDialog)
	//{{AFX_MSG_MAP(CExp_13_3_2Dlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_EXECUTE, OnBtnExecute)
	ON_WM_CLOSE()
	ON_WM_CANCELMODE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CExp_13_3_2Dlg message handlers

BOOL CExp_13_3_2Dlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

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
	//  when the application's main window is not m_pA dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
	m_pA = mxCreateDoubleMatrix(5,4,mxREAL);
	double data[5][4] = {{3.21,11.8,7.6,8.96},
                        {9.38,14.62,2.34,27.35},
                        {1.8,15.3,21.45,5.63},
                        {33.6,9.55,11.52,66.3},
                        {70.1,27.53,7.33,87.63}};
	memcpy((void*)mxGetPr(m_pA),(void*)data,sizeof(data));
	for(int i = 0; i < 5; i++)
	{			
		for(int j = 0; j < 4; j++)
		{
			CString strTemp = "";
			strTemp.Format("%8.2f",data[i][j]);
			m_data.Insert(m_data.GetLength(), strTemp);		
		}		
		m_data.Insert(m_data.GetLength(),"\r\n");
	}	

	// 更新界面显示
	UpdateData(FALSE);
	
	return TRUE;  // return TRUE  unless you set the focus to m_pA control
}

void CExp_13_3_2Dlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// If you add m_pA minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CExp_13_3_2Dlg::OnPaint() 
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
HCURSOR CExp_13_3_2Dlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CExp_13_3_2Dlg::OnBtnExecute() 
{
	// TODO: Add your control notification handler code here
	// 更新数据
	UpdateData(TRUE);
	
	// 清空原有数据
	m_data2 = "";

	mxArray *pB;
    pB = mxCreateDoubleMatrix(5,4,mxREAL);
    memcpy((void*)mxGetPr(pB),(void*)mxGetPr(m_pA),5*4*sizeof(double));

	mwArray resData = myfind(pB, mwArray(m_num));
    int nDim = resData.Size((int)1);
	for (int i = 0; i < nDim; i++)
	{
		CString strTemp = "";
		strTemp.Format("%8.2f",resData.x(i));
		m_data2.Insert(m_data2.GetLength(), strTemp);
 		if((i != 0) && ((i+1)%4 == 0))
 		{
 			m_data2.Insert(m_data2.GetLength(),"\r\n");
 		}
	}

	// 更新界面显示
	UpdateData(FALSE);
}

void CExp_13_3_2Dlg::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	// 释放空间
	mxDestroyArray(m_pA);
	CDialog::OnClose();
}
