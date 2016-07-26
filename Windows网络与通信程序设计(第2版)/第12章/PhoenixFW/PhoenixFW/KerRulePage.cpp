// KerRulePage.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "KerRulePage.h"

#include "KerRuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKerRulePage property page

IMPLEMENT_DYNCREATE(CKerRulePage, CPropertyPage)

CKerRulePage::CKerRulePage() : CPropertyPage(CKerRulePage::IDD)
{
	//{{AFX_DATA_INIT(CKerRulePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CKerRulePage::~CKerRulePage()
{
}

void CKerRulePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKerRulePage)
	DDX_Control(pDX, IDC_KERRULES, m_kerrules);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKerRulePage, CPropertyPage)
	//{{AFX_MSG_MAP(CKerRulePage)
	ON_BN_CLICKED(IDC_KERADD, OnKeradd)
	ON_BN_CLICKED(IDC_KERDEL, OnKerdel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKerRulePage message handlers

BOOL CKerRulePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	// 初始化列表视图控件
	m_kerrules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_kerrules.InsertColumn(0, L"源IP地址", LVCFMT_LEFT, sizeof(L"源IP地址")*8, 0);
	m_kerrules.InsertColumn(1, L"源屏蔽位", LVCFMT_LEFT, sizeof( L"源屏蔽位")*8, 1);
	m_kerrules.InsertColumn(2, L"源端口", LVCFMT_LEFT, sizeof(L"源端口")*8, 2);
	m_kerrules.InsertColumn(3, L"目的IP地址", LVCFMT_LEFT, sizeof(L"目IP地址")*8, 3);
	m_kerrules.InsertColumn(4, L"目的屏蔽位", LVCFMT_LEFT, sizeof(L"目屏蔽位")*8, 4);
	m_kerrules.InsertColumn(5, L"目的端口", LVCFMT_LEFT, sizeof(L"目端口")*8, 5);
	m_kerrules.InsertColumn(6, L"协议", LVCFMT_LEFT, sizeof(L"协议")*8, 6);
	m_kerrules.InsertColumn(7, L"动作", LVCFMT_LEFT, sizeof(L"动作")*8, 7);

	UpdateList();
	
	return TRUE;  // return TRUE unless you set the focus to a control
				// EXCEPTION: OCX Property Pages should return FALSE
}

void CKerRulePage::UpdateList()
{
	// 清空列表
	m_kerrules.DeleteAllItems();
	// 向列表中添加规则
	for(int i=0; i<(int)g_RuleFile.m_header.ulKerRuleCount; i++)
		AddRuleToList(&g_RuleFile.m_pKerRules[i]);
}

void CKerRulePage::AddRuleToList(PPassthruFilter pItem, int nEditIndex)
{
	int nIndex = m_kerrules.GetItemCount();
	if(nEditIndex >= 0)
		nIndex = nEditIndex;
	else
		m_kerrules.InsertItem(nIndex, L"", 0);

	WCHAR wszTemp[6];

	BYTE *pByte ;
	WCHAR wszIP[32];

	// 源IP地址
	pByte = (BYTE*)&pItem->sourceIP;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	CString s = (pItem->sourceIP == 0) ? L"所有" : wszIP;
	m_kerrules.SetItemText(nIndex, 0, s);
	// 源IP位掩码
	pByte = (BYTE*)&pItem->sourceMask;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	m_kerrules.SetItemText(nIndex, 1, wszIP);
	// 源端口号
	s = (pItem->sourcePort == 0) ? L"所有" : ::_itot(pItem->sourcePort, wszTemp, 10);
	m_kerrules.SetItemText(nIndex, 2, s);
	// 目的IP地址
	pByte = (BYTE*)&pItem->destinationIP;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	s = (pItem->destinationIP == 0) ? L"所有" : wszIP;
	m_kerrules.SetItemText(nIndex, 3, s);
	// 目的IP位掩码
	pByte = (BYTE*)&pItem->destinationMask;
	wsprintf(wszIP, L"%d.%d.%d.%d", pByte[3], pByte[2], pByte[1], pByte[0]);
	m_kerrules.SetItemText(nIndex, 4, wszIP);
	// 目的端口号
	s = (pItem->destinationPort == 0) ? L"所有" : ::_itot(pItem->destinationPort, wszTemp, 10);
	m_kerrules.SetItemText(nIndex, 5, s);
	// 协议
	if(pItem->protocol == 1)
		m_kerrules.SetItemText(nIndex, 6, L"ICMP");
	else if(pItem->protocol == 6)
		m_kerrules.SetItemText(nIndex, 6, L"TCP");
	else if(pItem->protocol == 17)
		m_kerrules.SetItemText(nIndex, 6, L"UDP");
	else
		m_kerrules.SetItemText(nIndex, 6, L"所有");
	// 动作
	s = pItem->bDrop ? L"丢弃" : L"放行";
	m_kerrules.SetItemText(nIndex, 7, s);
}

int CKerRulePage::InitAddRule()
{
	PPassthruFilter pItem = &CKerRuleDlg::m_RuleItem;

	// 设置默认规则
	memset(pItem, 0, sizeof(*pItem));
	pItem->destinationMask = -1;
	pItem->sourceMask = -1;
	pItem->bDrop = FALSE;

	// 弹出规则对话框
	CKerRuleDlg dlg;
	if(dlg.DoModal() == IDCANCEL)
	{
		return -1;
	}

	// 将规则添加到文件对象
	if(!g_RuleFile.AddKerRules(&CKerRuleDlg::m_RuleItem, 1))
	{
		AfxMessageBox(L"添加 核心层 规则错误。");
		return -1;
	}

	// 将规则添加到列表
	AddRuleToList(&CKerRuleDlg::m_RuleItem);
	return 0;
}

void CKerRulePage::OnKeradd()	// 添加一个规则
{
	if(InitAddRule() != 0)
		return;

	// 有效主对话框的应用按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}

void CKerRulePage::OnKerdel()	// 删除一个规则
{	
	POSITION pos = m_kerrules.GetFirstSelectedItemPosition();
	if(pos == NULL)
	{
		MessageBox(L"请选择一个规则！");
		return;
	}

	// 获取所选规则的索引
	int nIndex = m_kerrules.GetNextSelectedItem(pos);

	// 从文件对象中将规则删除
	g_RuleFile.DelKerRule(nIndex);

	// 从列表中将规则删除
	m_kerrules.DeleteItem(nIndex);

	// 有效主对话框的应用按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}
