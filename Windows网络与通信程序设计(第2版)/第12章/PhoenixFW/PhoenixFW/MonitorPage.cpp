// MonitorPage.cpp : implementation file
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "MonitorPage.h"

#include "RulePage.h"

#include "PhoenixFWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMonitorPage property page

extern CPhoenixApp theApp;

IMPLEMENT_DYNCREATE(CMonitorPage, CPropertyPage)

CMonitorPage::CMonitorPage() : CPropertyPage(CMonitorPage::IDD)
{
	//{{AFX_DATA_INIT(CMonitorPage)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMonitorPage::~CMonitorPage()
{
}

void CMonitorPage::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMonitorPage)
	DDX_Control(pDX, IDC_TREEMONITOR, m_MonitorTree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMonitorPage, CPropertyPage)
	//{{AFX_MSG_MAP(CMonitorPage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMonitorPage message handlers

BOOL CMonitorPage::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}



void CMonitorPage::HandleNotifySession(SESSION *pSession, int nCode)
{
	// 得到当前CMonitorPage类的指针（注意，HandleNotifySession是一个静态函数）
	CMonitorPage *pThis = &(((CMainDlg*)theApp.m_pMainWnd)->m_MonitorPage);

	// 一个会话的属性改变了
	if(nCode == CODE_CHANGE_SESSION)	
		pThis->AddASession(pSession);
	// 一个会话被删除
	else if(nCode == CODE_DELETE_SESSION)
		pThis->DeleteASession(pSession, FALSE);
	// 一个应用程序退出，我们要删除此应用程序的所有会话
	else if(nCode == CODE_APP_EXIT)
		pThis->DeleteASession(pSession, TRUE);
}

void CMonitorPage::AddASession(SESSION *pSession)
{
	TRACE(L" AddASession... ");

	// 首先查看所属的应用程序，如果没有，就插入一个新的应用程序项
	// 要在此应用程序项下添加会话
	HTREEITEM hAppItem = FindAppItem(pSession->szPathName);
	if(hAppItem == NULL) 
	{
		hAppItem = m_MonitorTree.InsertItem(pSession->szPathName);
	}

	// 通过SESSION结构，构建可显示的文本
	CString sText = BuildSessionText(pSession);

	// 在应用程序项下，根据套接字句柄查看此会话是否已经存在，
	// 如果存在，仅设置子项的文本，如果不存在，要创建一个新的子项
	HTREEITEM hSessionItem = FindSessionItem(hAppItem, pSession);
	if(hSessionItem != NULL)
	{
		m_MonitorTree.SetItemText(hSessionItem, sText);
	}
	else
	{	
		hSessionItem = m_MonitorTree.InsertItem(sText,hAppItem);
		m_MonitorTree.SetItemData(hSessionItem, pSession->s);
	}
}

HTREEITEM CMonitorPage::FindAppItem(TCHAR *pszPathName)
{
	// 遍历所有应用程序项，看看指定应用程序是否存在
	HTREEITEM hAppItem = m_MonitorTree.GetNextItem(TVI_ROOT, TVGN_CHILD);
	while(hAppItem != NULL)
	{
		if(m_MonitorTree.GetItemText(hAppItem).CompareNoCase(pszPathName) == 0)
			return hAppItem; // 存在，返回项句柄

		hAppItem = m_MonitorTree.GetNextItem(hAppItem, TVGN_NEXT);
	}
	return NULL;
}

HTREEITEM CMonitorPage::FindSessionItem(HTREEITEM hAppItem, SESSION *pSession)
{
	// 变量所有会话项，看看指定会话是否存在
	HTREEITEM hSessionItem = m_MonitorTree.GetNextItem(hAppItem, TVGN_CHILD);
	while(hSessionItem != NULL)
	{
		if(pSession->s == m_MonitorTree.GetItemData(hSessionItem))
			return hSessionItem; // 存在，返回项句柄

		hSessionItem = m_MonitorTree.GetNextItem(hSessionItem, TVGN_NEXT);
	}
	return NULL;
}

CString CMonitorPage::BuildSessionText(SESSION *pSession)
{
	CString sText;

	CString sServType, sLocal, sRemote, sDirection;

	// 本地IP地址
	BYTE *pByte = (BYTE *)&pSession->ulLocalIP; // 注意，这里的IP地址是网络字节顺序
	sLocal.Format(L"%d.%d.%d.%d：%d", pByte[0], pByte[1], pByte[2], pByte[3], pSession->usLocalPort);

	// 远程IP地址
	pByte = (BYTE *)&pSession->ulRemoteIP;
	sRemote.Format(L"%d.%d.%d.%d：%d", pByte[0], pByte[1], pByte[2], pByte[3], pSession->usRemotePort);

	// 服务类型
	sServType = L"其它";
	switch(pSession->nProtocol)
	{	
	case RULE_SERVICE_TYPE_ALL:
		sServType.Format(L"所有");
		break;
	case RULE_SERVICE_TYPE_TCP:
		sServType.Format(L"TCP");
		break;
	case RULE_SERVICE_TYPE_UDP:
		sServType.Format(L"UDP");
		break;
	case RULE_SERVICE_TYPE_FTP:
		sServType.Format(L"FTP");
		break;
	case RULE_SERVICE_TYPE_TELNET:
		sServType.Format(L"TELNET");
		break;
	case RULE_SERVICE_TYPE_HTTP:
		sServType.Format(L"HTTP");
		break;
	case RULE_SERVICE_TYPE_NNTP:
		sServType.Format(L"NNTP");
		break;
	case RULE_SERVICE_TYPE_POP3:
		sServType.Format(L"POP3");
		break;
	case RULE_SERVICE_TYPE_SMTP:
		sServType.Format(L"SMTP");
		break;
	}

	// 方向
	switch(pSession->ucDirection)
	{
	case RULE_DIRECTION_IN:
		sDirection = L"<——";
		break;
	case RULE_DIRECTION_OUT:
		sDirection = L"——>";
		break;
	case RULE_DIRECTION_IN_OUT:
		sDirection = L"<——>";
		break;
	default:
		sDirection = L"——";
	}

	sText.Format(L" %s 协议	    【%s】 %s 【%s】 ", sServType, sLocal, sDirection, sRemote);
	return sText;
}


void CMonitorPage::DeleteASession(SESSION *pSession, BOOL bAppExit)
{
	TRACE(L" DeleteASession... ");

	HTREEITEM hAppItem = FindAppItem(pSession->szPathName);
	if(hAppItem != NULL)
	{
		if(bAppExit) // 应用程序退出，删除整个应用程序项（包含下面的会话子项）
		{
			m_MonitorTree.DeleteItem(hAppItem);
		}
		else		// 仅会话删除，在应用程序项下面找到这个会话，将之删除
		{
			HTREEITEM hSessionItem = FindSessionItem(hAppItem, pSession);
			if(hSessionItem != NULL)
			{	
				m_MonitorTree.DeleteItem(hSessionItem);
			}
			// 没有Session存在了，将应用程序项也删除
			if(m_MonitorTree.GetNextItem(hAppItem, TVGN_CHILD) == NULL) 
				m_MonitorTree.DeleteItem(hAppItem);
		}
	}
}
