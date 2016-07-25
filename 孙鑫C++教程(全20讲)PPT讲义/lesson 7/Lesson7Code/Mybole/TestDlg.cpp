// TestDlg.cpp : implementation file
//

#include "stdafx.h"
#include "Mybole.h"
#include "TestDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CTestDlg dialog


CTestDlg::CTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CTestDlg)
	m_num1 = 0;
	m_num2 = 0;
	m_num3 = 0;
	//}}AFX_DATA_INIT
	m_bIsCreate=FALSE;
}


void CTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CTestDlg)
	DDX_Control(pDX, IDC_EDIT3, m_edit3);
	DDX_Control(pDX, IDC_EDIT2, m_edit2);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Text(pDX, IDC_EDIT1, m_num1);
	DDV_MinMaxInt(pDX, m_num1, 0, 100);
	DDX_Text(pDX, IDC_EDIT2, m_num2);
	DDV_MinMaxInt(pDX, m_num2, 0, 100);
	DDX_Text(pDX, IDC_EDIT3, m_num3);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CTestDlg, CDialog)
	//{{AFX_MSG_MAP(CTestDlg)
	ON_BN_CLICKED(IDC_BTN_ADD, OnBtnAdd)
	ON_BN_CLICKED(IDC_NUMBER1, OnNumber1)
	ON_BN_CLICKED(IDC_BUTTON2, OnButton2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTestDlg message handlers


void CTestDlg::OnBtnAdd() 
{
	// TODO: Add your control notification handler code here
/*	static BOOL bIsCreate=FALSE;
//	if(m_bIsCreate==FALSE)
	if(bIsCreate==FALSE)
	{
		m_btn.Create("维新",BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD,
			CRect(0,0,100,100),this,123);
		//m_bIsCreate=TRUE;
		bIsCreate=TRUE;
	}
	else
	{
		m_btn.DestroyWindow();
		//m_bIsCreate=FALSE;
		bIsCreate=FALSE;
	}*/
/*	if(!m_btn.m_hWnd)
	{
		m_btn.Create("维新",BS_DEFPUSHBUTTON | WS_VISIBLE | WS_CHILD,
			CRect(0,0,100,100),this,123);
	}
	else
	{
		m_btn.DestroyWindow();
	}*/
/*	int num1,num2,num3;
	char ch1[10],ch2[10],ch3[10];

	GetDlgItem(IDC_EDIT1)->GetWindowText(ch1,10);
	GetDlgItem(IDC_EDIT2)->GetWindowText(ch2,10);

	num1=atoi(ch1);
	num2=atoi(ch2);
	num3=num1+num2;
	itoa(num3,ch3,10);

	GetDlgItem(IDC_EDIT3)->SetWindowText(ch3);*/

/*	int num1,num2,num3;
	char ch1[10],ch2[10],ch3[10];

	GetDlgItemText(IDC_EDIT1,ch1,10);
	GetDlgItemText(IDC_EDIT2,ch2,10);

	num1=atoi(ch1);
	num2=atoi(ch2);
	num3=num1+num2;
	itoa(num3,ch3,10);

	SetDlgItemText(IDC_EDIT3,ch3);*/

/*	int num1,num2,num3;
	num1=GetDlgItemInt(IDC_EDIT1);
	num2=GetDlgItemInt(IDC_EDIT2);

	num3=num1+num2;
	SetDlgItemInt(IDC_EDIT3,num3);*/

	/*UpdateData();
	m_num3=m_num1+m_num2;
	UpdateData(FALSE);*/

/*	int num1,num2,num3;
	char ch1[10],ch2[10],ch3[10];

	m_edit1.GetWindowText(ch1,10);
	m_edit2.GetWindowText(ch2,10);

	num1=atoi(ch1);
	num2=atoi(ch2);
	num3=num1+num2;
	itoa(num3,ch3,10);

	m_edit3.SetWindowText(ch3);*/

/*	int num1,num2,num3;
	char ch1[10],ch2[10],ch3[10];

	//::SendMessage(GetDlgItem(IDC_EDIT1)->m_hWnd,WM_GETTEXT,10,(LPARAM)ch1);
	//::SendMessage(m_edit1.m_hWnd,WM_GETTEXT,10,(LPARAM)ch1);
	//GetDlgItem(IDC_EDIT1)->SendMessage(WM_GETTEXT,10,(LPARAM)ch1);
	m_edit1.SendMessage(WM_GETTEXT,10,(LPARAM)ch1);
	m_edit2.SendMessage(WM_GETTEXT,10,(LPARAM)ch2);

	num1=atoi(ch1);
	num2=atoi(ch2);
	num3=num1+num2;
	itoa(num3,ch3,10);

	m_edit3.SendMessage(WM_SETTEXT,0,(LPARAM)ch3);*/

	int num1,num2,num3;
	char ch1[10],ch2[10],ch3[10];

	SendDlgItemMessage(IDC_EDIT1,WM_GETTEXT,10,(LPARAM)ch1);
	SendDlgItemMessage(IDC_EDIT2,WM_GETTEXT,10,(LPARAM)ch2);

	num1=atoi(ch1);
	num2=atoi(ch2);
	num3=num1+num2;
	itoa(num3,ch3,10);

	SendDlgItemMessage(IDC_EDIT3,WM_SETTEXT,0,(LPARAM)ch3);
	SendDlgItemMessage(IDC_EDIT3,EM_SETSEL,0,-1);
	m_edit3.SetFocus();
}

void CTestDlg::OnNumber1() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if(GetDlgItem(IDC_NUMBER1)->GetWindowText(str),str=="Number1:")
	{
		GetDlgItem(IDC_NUMBER1)->SetWindowText("数值1:");
	}
	else
	{
		GetDlgItem(IDC_NUMBER1)->SetWindowText("Number1:");
	}
}

void CTestDlg::OnButton2() 
{
	// TODO: Add your control notification handler code here
	CString str;
	if(GetDlgItemText(IDC_BUTTON2,str),str=="收缩<<")
	{
		SetDlgItemText(IDC_BUTTON2,"扩展>>");
	}
	else
	{
		SetDlgItemText(IDC_BUTTON2,"收缩<<");
	}

	static CRect rectLarge;
	static CRect rectSmall;
	
	if(rectLarge.IsRectNull())
	{
		CRect rectSeparator;
		GetWindowRect(&rectLarge);
		GetDlgItem(IDC_SEPARATOR)->GetWindowRect(&rectSeparator);

		rectSmall.left=rectLarge.left;
		rectSmall.top=rectLarge.top;
		rectSmall.right=rectLarge.right;
		rectSmall.bottom=rectSeparator.bottom;
	}
	if(str=="收缩<<")
	{
		SetWindowPos(NULL,0,0,rectSmall.Width(),rectSmall.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
	else
	{
		SetWindowPos(NULL,0,0,rectLarge.Width(),rectLarge.Height(),
			SWP_NOMOVE | SWP_NOZORDER);
	}
}

void CTestDlg::OnOK() 
{
	// TODO: Add extra validation here
	//GetDlgItem(IDC_EDIT1)->GetNextWindow()->SetFocus();
	//GetFocus()->GetNextWindow()->SetFocus();
	//GetFocus()->GetWindow(GW_HWNDNEXT)->SetFocus();
	GetNextDlgTabItem(GetFocus())->SetFocus();
//	CDialog::OnOK();
}
WNDPROC prevProc;
LRESULT CALLBACK WinSunProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	if(uMsg==WM_CHAR && wParam==0x0d)
	{
		//::SetFocus(::GetNextWindow(hwnd,GW_HWNDNEXT));
		//SetFocus(::GetWindow(hwnd,GW_HWNDNEXT));
		SetFocus(::GetNextDlgTabItem(::GetParent(hwnd),hwnd,FALSE));
		return 1;
	}
	else
	{
		return prevProc(hwnd,uMsg,wParam,lParam);
	}
}


BOOL CTestDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	prevProc=(WNDPROC)SetWindowLong(GetDlgItem(IDC_EDIT1)->m_hWnd,GWL_WNDPROC,
		(LONG)WinSunProc);
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
