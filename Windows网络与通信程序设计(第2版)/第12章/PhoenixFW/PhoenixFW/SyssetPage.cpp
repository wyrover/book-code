// SyssetPage.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "SyssetPage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CSyssetPage property page

IMPLEMENT_DYNCREATE(CSyssetPage, CPropertyPage)

CSyssetPage::CSyssetPage() : CPropertyPage(CSyssetPage::IDD)
{
	//{{AFX_DATA_INIT(CSyssetPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CSyssetPage::~CSyssetPage()
{
}

void CSyssetPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSyssetPage)
	DDX_Control(pDX, IDC_AUTOSTART, m_AutoStart);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSyssetPage, CPropertyPage)
	//{{AFX_MSG_MAP(CSyssetPage)
	ON_BN_CLICKED(IDC_INSTALL, OnInstall)
	ON_BN_CLICKED(IDC_REMOVE, OnRemove)
	ON_BN_CLICKED(IDC_AUTOSTART, OnAutostart)
	ON_BN_CLICKED(IDC_PASS_ALL, OnPassAll)
	ON_BN_CLICKED(IDC_QUERY_ALL, OnQueryAll)
	ON_BN_CLICKED(IDC_DENY_ALL, OnDenyAll)
	ON_BN_CLICKED(IDC_KERPASS_ALL, OnKerpassAll)
	ON_BN_CLICKED(IDC_KERSTART_FILTER, OnKerstartFilter)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSyssetPage message handlers

BOOL CSyssetPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// 设置开机自动启动复选框
	if(g_RuleFile.m_header.bAutoStart)
		((CButton*)GetDlgItem(IDC_AUTOSTART))->SetCheck(1);

	// 用户层工作模式设置单选框
	switch(g_RuleFile.m_header.ucLspWorkMode)
	{
	case PF_PASS_ALL:
		((CButton*)GetDlgItem(IDC_PASS_ALL))->SetCheck(1);
		break;
	case PF_QUERY_ALL:
		((CButton*)GetDlgItem(IDC_QUERY_ALL))->SetCheck(1);
		break;
	case PF_DENY_ALL:
		((CButton*)GetDlgItem(IDC_DENY_ALL))->SetCheck(1);
		break;
	}

	// 核心层工作模式设置单选框
	switch(g_RuleFile.m_header.ucKerWorkMode)
	{
	case IM_PASS_ALL:
		((CButton*)GetDlgItem(IDC_KERPASS_ALL))->SetCheck(1);
		break;
	case IM_START_FILTER:
		((CButton*)GetDlgItem(IDC_KERSTART_FILTER))->SetCheck(1);
		break;
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSyssetPage::OnInstall()				// 用户点击"安装"按钮
{
	TCHAR szPathName[256];
	TCHAR* p;

	// 注意，安装LSP需要使用完整DLL路径。这样的话，CPIOControl类在加载DLL时也应使用
	// 完整路径，否则CPIOControl类加载的DLL不能和作为LSP的DLL共享内存
	if(::GetFullPathName(PHOENIX_SERVICE_DLL_NAME, 256, szPathName, &p) != 0)
	{
		if(InstallProvider(szPathName))
		{
			MessageBox(L" 应用层过滤安装成功！"); 
			return;
		}
	}
	MessageBox(L" 应用层过滤安装失败！"); 
}

void CSyssetPage::OnRemove()				// 用户点击"卸载"按钮
{	
	if(RemoveProvider())
		MessageBox(L" 应用层过滤卸载成功！");
	else
		MessageBox(L" 应用层过滤卸载失败！");
}


void CSyssetPage::OnAutostart()				// 	用户点击"开机自动运行"复选框 
{
	BOOL bCheck = m_AutoStart.GetCheck();
	g_RuleFile.m_header.bAutoStart = bCheck;

	// 有效主对话框的应用按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}

void CSyssetPage::OnPassAll()				// 用户点击用户层下的“放行所有”单选框
{
	SetLspWorkMode(PF_PASS_ALL);
}

void CSyssetPage::OnQueryAll()				// 用户点击用户层下的“询问” 单选框
{
	SetLspWorkMode(PF_QUERY_ALL);
}

void CSyssetPage::OnDenyAll()				// 用户点击用户层下的“拒绝所有” 单选框
{
	SetLspWorkMode(PF_DENY_ALL);
}


void CSyssetPage::OnKerpassAll()			// 用户点击核心层下的“放行所有” 单选框
{
	SetKerWorkMode(IM_PASS_ALL);	
}

void CSyssetPage::OnKerstartFilter()		// 用户点击核心层下的“开启过滤” 单选框
{	
	SetKerWorkMode(IM_START_FILTER);	
}


void CSyssetPage::SetKerWorkMode(int nWorkMode)
{
	g_RuleFile.m_header.ucKerWorkMode = nWorkMode;
	// 有效主对话框的应用按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}

void CSyssetPage::SetLspWorkMode(int nWorkMode)
{
	g_RuleFile.m_header.ucLspWorkMode = nWorkMode;
	// 有效主对话框的应用按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}




