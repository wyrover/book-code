// KerRuleDlg.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "KerRuleDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CKerRuleDlg dialog

PassthruFilter CKerRuleDlg::m_RuleItem;


CKerRuleDlg::CKerRuleDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CKerRuleDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CKerRuleDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CKerRuleDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CKerRuleDlg)
	DDX_Control(pDX, IDC_SOURCE_PORT, m_SourcePort);
	DDX_Control(pDX, IDC_SOURCE_MASK, m_SourceMask);
	DDX_Control(pDX, IDC_SOURCE_IP, m_SourceIP);
	DDX_Control(pDX, IDC_RULE_PROTOCOL, m_RuleProtocol);
	DDX_Control(pDX, IDC_RULE_ACTION, m_RuleAction);
	DDX_Control(pDX, IDC_DEST_PORT, m_DestPort);
	DDX_Control(pDX, IDC_DEST_MASK, m_DestMask);
	DDX_Control(pDX, IDC_DEST_IP, m_DestIP);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CKerRuleDlg, CDialog)
	//{{AFX_MSG_MAP(CKerRuleDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CKerRuleDlg message handlers


BOOL CKerRuleDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();

	// 初始化实施动作组合框资源
	m_RuleAction.SetItemData(m_RuleAction.AddString(L"放行"), 0);
	m_RuleAction.SetItemData(m_RuleAction.AddString(L"拒绝"), 1);
	m_RuleAction.SetCurSel(0);

	// 初始化协议组合框资源
	m_RuleProtocol.SetItemData(m_RuleProtocol.AddString(L"全部"), 0);
	m_RuleProtocol.SetItemData(m_RuleProtocol.AddString(L"TCP"), IPPROTO_TCP);
	m_RuleProtocol.SetItemData(m_RuleProtocol.AddString(L"UDP"), IPPROTO_UDP);
	m_RuleProtocol.SetItemData(m_RuleProtocol.AddString(L"ICMP"), IPPROTO_ICMP);
	m_RuleProtocol.SetCurSel(0);

	// 设置实施动作组合框
	m_RuleAction.SetCurSel(m_RuleItem.bDrop);

	// 设置协议组合框
	for(int i=0; i<m_RuleProtocol.GetCount(); i++)
	{
		if(m_RuleProtocol.GetItemData(i) == m_RuleItem.protocol)
		{
			m_RuleProtocol.SetCurSel(i);
			break;
		}
	}

	// 设置源IP地址，和IP掩位码
	m_SourceIP.SetAddress(m_RuleItem.sourceIP);
	m_SourceMask.SetAddress(m_RuleItem.sourceMask);

	// 设置目的IP地址，和IP掩位码
	m_DestIP.SetAddress(m_RuleItem.destinationIP);
	m_DestMask.SetAddress(m_RuleItem.destinationMask);

	// 设置源端口号
	CString tmpStr;
	tmpStr.Format(L"%u", m_RuleItem.sourcePort);
	m_SourcePort.SetWindowText(tmpStr);

	// 设置目的端口号
	tmpStr.Format(L"%u", m_RuleItem.destinationPort);
	m_DestPort.SetWindowText(tmpStr);

	m_SourcePort.SetLimitText(5);
	m_DestPort.SetLimitText(5);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CKerRuleDlg::OnOK() 
{
	PassthruFilter tmpRule = { 0 };
	CString strSourcePort, strDestPort;

	
	// 获取源端口号
	m_SourcePort.GetWindowText(strSourcePort);
	int nPort = _ttoi(strSourcePort);
	if(nPort > 65535 || nPort < 0)
	{
		AfxMessageBox(L"源端口号无效，有效范围为 0 - 65535，请重新输入。");
		m_SourcePort.SetFocus();
		return ;
	}
	tmpRule.sourcePort = nPort;

	// 获取目的端口号
	m_DestPort.GetWindowText(strDestPort);
	nPort = _ttoi(strDestPort);
	if(nPort > 65535 || nPort < 0)
	{
		AfxMessageBox(L"目的端口号无效，有效范围为 0 - 65535，请重新输入。");
		m_DestPort.SetFocus();
		return ;
	}
	tmpRule.destinationPort = nPort;

	// 采取的动作
	tmpRule.bDrop = (BOOLEAN)m_RuleAction.GetItemData(m_RuleAction.GetCurSel());
	// 协议
	tmpRule.protocol = (USHORT)m_RuleProtocol.GetItemData(m_RuleProtocol.GetCurSel());
	// 源IP和掩位码。注意，CIPAddressCtrl类是以主机字节顺序返回IP地址的
	m_SourceIP.GetAddress(tmpRule.sourceIP);
	m_SourceMask.GetAddress(tmpRule.sourceMask);

	// 目的IP和掩位码
	m_DestIP.GetAddress(tmpRule.destinationIP);
	m_DestMask.GetAddress(tmpRule.destinationMask);

	m_RuleItem = tmpRule;
	CDialog::OnOK();
}
