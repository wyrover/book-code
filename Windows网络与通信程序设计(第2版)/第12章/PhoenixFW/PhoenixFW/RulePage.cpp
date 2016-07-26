// RulePage.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "RulePage.h"
#include "Ruledlg.h"
#include "PhoenixFWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRulePage property page
extern CPhoenixApp theApp;


IMPLEMENT_DYNCREATE(CRulePage, CPropertyPage)

CRulePage::CRulePage() : CPropertyPage(CRulePage::IDD)
{
	//{{AFX_DATA_INIT(CRulePage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CRulePage::~CRulePage()
{
}

void CRulePage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CRulePage)
	DDX_Control(pDX, IDC_RULES, m_rules);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CRulePage, CPropertyPage)
	//{{AFX_MSG_MAP(CRulePage)
	ON_BN_CLICKED(IDC_ADD, OnAdd)
	ON_BN_CLICKED(IDC_DEL, OnDel)
	ON_NOTIFY(NM_CLICK, IDC_RULES, OnClickRules)
	ON_BN_CLICKED(IDC_EDIT, OnEdit)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_RULES, OnItemchangedRules)
	ON_NOTIFY(NM_DBLCLK, IDC_RULES, OnDblclkRules)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRulePage message handlers


BOOL CRulePage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// 初始化列表视图控件
	m_rules.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);
	m_rules.InsertColumn(0, L"应用程序", LVCFMT_LEFT, sizeof(L"应用程序")*8, 0);
	m_rules.InsertColumn(1, L"动作", LVCFMT_LEFT, sizeof( L"动作")*8, 1);
	m_rules.InsertColumn(2, L"类型/端口", LVCFMT_LEFT, sizeof(L"类型/端口")*8, 2);
	m_rules.InsertColumn(3, L"应用程序路径", LVCFMT_LEFT, sizeof(L"应用程序路径")*12, 3);
	m_rules.InsertColumn(4, L"说明", LVCFMT_LEFT, sizeof(L"说明")*12, 4);

	// 更新列表，即向列表中添加规则
	UpdateList();

	// 无效删除和编辑按钮
	GetDlgItem(IDC_DEL)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CRulePage::UpdateList()
{
	// 清空列表
	m_rules.DeleteAllItems();

	// 向列表中添加规则
	for(int i=0; i<(int)g_RuleFile.m_header.ulLspRuleCount; i++)
		EditARule(&g_RuleFile.m_pLspRules[i]);
}

void CRulePage::EditARule(RULE_ITEM *pItem, int nEditIndex)
{
	// 如果nEditIndex大于等于0就编辑索引为nEditIndex的项，否则插入一个新项
	int nIndex = m_rules.GetItemCount();
	if(nEditIndex >= 0)
		nIndex = nEditIndex;
	else
		m_rules.InsertItem(nIndex, L"", 0);

	// 构建新项的文本
	CString sAction, sServType;

	sAction = (pItem->ucAction == 0) ? L"放行" : L"拒绝";

	switch(pItem->ucServiceType)
	{	
	case RULE_SERVICE_TYPE_ALL:
		sServType.Format(L"所有/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_TCP:
		sServType.Format(L"TCP/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_UDP:
		sServType.Format(L"UDP/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_FTP:
		sServType.Format(L"FTP/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_TELNET:
		sServType.Format(L"TELNET/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_HTTP:
		sServType.Format(L"HTTP/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_NNTP:
		sServType.Format(L"NNTP/%d", pItem->usServicePort);
		break;
	case RULE_SERVICE_TYPE_POP3:
		sServType.Format(L"POP3/%d", pItem->usServicePort);
		break;
	}

	// 设置新项的文本
	m_rules.SetItemText(nIndex, 0, GetFileName(pItem->szApplication));
	m_rules.SetItemText(nIndex, 1, sAction);
	m_rules.SetItemText(nIndex, 2, sServType);
	m_rules.SetItemText(nIndex, 3, GetFilePath(pItem->szApplication));
	m_rules.SetItemText(nIndex, 4, pItem->sDemo);
}




int CRulePage::InitAddRule(LPCTSTR szQueryApp)
{
	if(g_RuleFile.m_header.ulLspRuleCount > MAX_RULE_COUNT)
	{
		AfxMessageBox(L" 超过规则的最大数目，不能够再添加");
		return -1;
	}

	// 设置一个默认的规则
	RULE_ITEM tmpRule;

	_tcscpy(tmpRule.sDemo, L"");
	_tcscpy(tmpRule.szApplication, L"");
	tmpRule.ucAction = RULE_ACTION_PASS;
	tmpRule.ucDirection = RULE_DIRECTION_IN_OUT;
	tmpRule.ucServiceType = RULE_SERVICE_TYPE_ALL;
	tmpRule.usServicePort = RULE_SERVICE_PORT_ALL;

	// 设置传递的参数
	CRuleDlg::m_sPathName = szQueryApp;
	CRuleDlg::m_RuleItem = tmpRule;
	CRuleDlg::m_bAppQuery = (szQueryApp == NULL) ? 0 : 1;

	// 设置本页面为活动页面
	if(CRuleDlg::m_bAppQuery)
		((CMainDlg*)theApp.m_pMainWnd)->m_sheet.SetActivePage(this);

	// 弹出添加规则对话框
	CRuleDlg dlg;
	if(dlg.DoModal() == IDCANCEL)
	{
		return -1;
	}

	// 将规则添加到文件
	if(!g_RuleFile.AddLspRules(&CRuleDlg::m_RuleItem, 1))
	{
		AfxMessageBox(L"添加 ACL 规则错误。");
		return -1;
	}

	// 将规则添加到列表试图
	EditARule(&CRuleDlg::m_RuleItem);

	return CRuleDlg::m_RuleItem.ucAction;
}

BOOL CRulePage::AddQueryRule(LPCTSTR pszQueryApp) // 静态函数
{
	int nRet = ((CMainDlg*)theApp.m_pMainWnd)->m_RulePage.InitAddRule(pszQueryApp);
	if( nRet == -1 )
		return FALSE;

	// 将规则保存到文件
	g_RuleFile.SaveRules();

	// 将规则应用到DLL模块
	theApp.ApplyFileData();
	// 无效主对话框的应用按钮
	((CMainDlg*)theApp.m_pMainWnd)->GetDlgItem(IDC_APPLY)->EnableWindow(FALSE);

	return nRet == RULE_ACTION_PASS;
}

void CRulePage::OnAdd()		// 用户点击“添加”按钮
{
	if(InitAddRule() != 0)
		return;
	// 有效主对话框的应用按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
}



void CRulePage::OnEdit()									// 用户点击“编辑” 按钮
{
	if(m_nListIndex < 0)
		return;

	CRuleDlg::m_RuleItem = g_RuleFile.m_pLspRules[m_nListIndex];
	CRuleDlg::m_bAppQuery = FALSE;

	CRuleDlg dlg;
	if(dlg.DoModal() == IDOK)
	{
		g_RuleFile.m_pLspRules[m_nListIndex] = CRuleDlg::m_RuleItem;
		EditARule(&CRuleDlg::m_RuleItem, m_nListIndex);
		GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);
	}
}

void CRulePage::OnDblclkRules(NMHDR* pNMHDR, LRESULT* pResult) 	// 用户双击列表
{
	NM_LISTVIEW* pNMList = (NM_LISTVIEW*)pNMHDR;
	if((m_nListIndex = pNMList->iItem) != -1)
	{
		OnEdit();
	}
	*pResult = 0;
}

void CRulePage::OnDel()					// 用户点击“删除” 按钮
{	
	if(m_nListIndex < 0)
		return;

	// 从文件中将规则删除
	g_RuleFile.DelLspRule(m_nListIndex);
	// 从列表试图中将文件删除
	m_rules.DeleteItem(m_nListIndex);
	// 有效主窗口的“应用”按钮
	GetOwner()->GetOwner()->GetDlgItem(IDC_APPLY)->EnableWindow(TRUE);

	// 如果没有规则了，则无效“编辑”和“删除”按钮
	if(m_rules.GetItemCount() <= 0)
	{
		GetDlgItem(IDC_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);	
		return;
	}
	// 否则，选中下一个规则
	if(m_nListIndex == m_rules.GetItemCount()) // 如果删除的是最后一个
		m_nListIndex--;
	m_rules.SetItemState(m_nListIndex, LVIS_SELECTED, LVIS_SELECTED);
}

void CRulePage::OnItemchangedRules(NMHDR* pNMHDR, LRESULT* pResult) // 用户改变所选项
{
	NM_LISTVIEW* pNMList = (NM_LISTVIEW*)pNMHDR;
	// 获取当前选择项的索引，如果没有选择任何项目，则无效“编辑”和“删除”按钮
	if((m_nListIndex = pNMList->iItem) != -1)
	{
		GetDlgItem(IDC_DEL)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT)->EnableWindow(TRUE);
	}

	*pResult = 0;
}

void CRulePage::OnClickRules(NMHDR* pNMHDR, LRESULT* pResult)		// 用户单击列表
{
	NM_LISTVIEW* pNMList = (NM_LISTVIEW*)pNMHDR;
	// 获取当前选择项的索引，如果没有选择任何项目，则无效“编辑”和“删除”按钮
	if((m_nListIndex = pNMList->iItem) == -1)
	{
		GetDlgItem(IDC_DEL)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT)->EnableWindow(FALSE);
	}
	*pResult = 0;
}

