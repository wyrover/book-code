///////////////////////////////////////////////////
// Acl.cpp文件


#define UNICODE
#define _UNICODE

#include <winsock2.h>
#include <windows.h>
#include <stdio.h>
#include <tchar.h>

#include "../common/debug.h"

#include "Acl.h"


///////////////////////////////////////
// 共享内存

// 所有使用Winsock访问网络的应用程序都共享initdata和uinitdata段的变量
#pragma data_seg(".initdata")
HWND			g_hPhoenixWnd = NULL;			// 主窗口句柄
UCHAR			g_ucWorkMode = PF_PASS_ALL;		// 工作模式
#pragma data_seg()

#pragma bss_seg(".uinitdata") 
RULE_ITEM		g_Rule[MAX_RULE_COUNT];				  // 应用层规则
ULONG			g_RuleCount;
QUERY_SESSION	g_QuerySession[MAX_QUERY_SESSION];	  // 向主程序发送会话询问时使用
SESSION			g_SessionBuffer[MAX_SESSION_BUFFER];  // 向主程序发送会话信息时使用
TCHAR			g_szPhoenixFW[MAX_PATH];			  // 记录主程序路径
#pragma bss_seg()


extern TCHAR	g_szCurrentApp[MAX_PATH];

CRITICAL_SECTION	g_csGetAccess;



CAcl::CAcl()
{
	m_nSessionCount = 0;
	// 为会话结构预申请内存空间
	m_nSessionMaxCount = INIT_SESSION_BUFFER;	
	m_pSession = new SESSION[m_nSessionMaxCount];

	::InitializeCriticalSection(&g_csGetAccess);
}

CAcl::~CAcl()
{
	ODS(L" CAcl::~CAcl send CODE_APP_EXIT ... ");
	// 通知主模块，当前应用程序正在退出
	int nIndex = CreateSession(0, 0);
	NotifySession(&m_pSession[nIndex], CODE_APP_EXIT);

	delete[] m_pSession;
	::DeleteCriticalSection(&g_csGetAccess);
}

/////////////////////////////////////////////////////////
// 检查函数

void CAcl::CheckSocket(SOCKET s, int af, int type, int protocol)
{	
	if (af != AF_INET) // 仅支持IPv4
		return;

	// 先判断基本协议类型

	int nProtocol = RULE_SERVICE_TYPE_ALL;

	if(protocol == 0)
	{
		if(type ==  SOCK_STREAM)
			nProtocol = RULE_SERVICE_TYPE_TCP;
		else if(type == SOCK_DGRAM)
			nProtocol = RULE_SERVICE_TYPE_UDP;
	}
	else if(protocol == IPPROTO_TCP)
		nProtocol = RULE_SERVICE_TYPE_TCP;
	else if(protocol == IPPROTO_UDP)
		nProtocol = RULE_SERVICE_TYPE_UDP;

	// 为新套节字创建会话，指明协议类型
	CreateSession(s, nProtocol);
}

void CAcl::CheckCloseSocket(SOCKET s)
{
	// 删除会话
	DeleteSession(s);
}

void CAcl::CheckBind(SOCKET s, const struct sockaddr *addr)
{
	int nIndex;
	if((nIndex = FindSession(s)) >= m_nSessionCount)
		return;

	// 设置会话
	sockaddr_in *pLocal = (sockaddr_in *)addr;
	m_pSession[nIndex].usLocalPort = ntohs(pLocal->sin_port);

	if(pLocal->sin_addr.S_un.S_addr != ADDR_ANY)
		m_pSession[nIndex].ulLocalIP = *((DWORD*)&pLocal->sin_addr);
}


int CAcl::CheckAccept(SOCKET s, SOCKET sNew, sockaddr FAR *addr)
{
	int nIndex;
	if((nIndex = FindSession(s)) >= m_nSessionCount)
		return PF_PASS;

	nIndex = CreateSession(sNew, RULE_SERVICE_TYPE_TCP);

	// 设置会话
	if(addr != NULL)
	{
		sockaddr_in *pRemote = (sockaddr_in *)addr;
		USHORT usPort = ntohs(pRemote->sin_port);
		DWORD dwIP = *((DWORD*)&pRemote->sin_addr);
		SetSession(&m_pSession[nIndex], usPort, dwIP, RULE_DIRECTION_IN_OUT);
	}

	return GetAccessInfo(&m_pSession[nIndex]);
}

int CAcl::CheckConnect(SOCKET s, const struct sockaddr FAR *addr)
{
	int nIndex;
	if((nIndex = FindSession(s)) >= m_nSessionCount)
		return PF_PASS;

	// 设置会话远程地址
	sockaddr_in *pRemote = (sockaddr_in *)addr;
	USHORT usPort = ntohs(pRemote->sin_port);
	DWORD dwIP = *((DWORD*)&pRemote->sin_addr);
	SetSession(&m_pSession[nIndex], usPort, dwIP, RULE_DIRECTION_IN_OUT);

	return GetAccessInfo(&m_pSession[nIndex]);
}

int CAcl::CheckSendTo(SOCKET s, const SOCKADDR *pTo)
{
	int nIndex;
	if((nIndex = FindSession(s)) >= m_nSessionCount)
		return PF_PASS;

	if(pTo != NULL)
	{
		// 设置会话远程地址
		sockaddr_in *pRemote = (sockaddr_in *)pTo;
		USHORT usPort = ntohs(pRemote->sin_port);
		DWORD dwIP = *((DWORD*)&pRemote->sin_addr);
		SetSession(&m_pSession[nIndex], usPort, dwIP, RULE_DIRECTION_OUT);
	}

	return GetAccessInfo(&m_pSession[nIndex]);
}

int CAcl::CheckRecvFrom(SOCKET s, SOCKADDR *pFrom)
{
	int nIndex;
	if((nIndex = FindSession(s)) >= m_nSessionCount)
		return PF_PASS;

	if(pFrom != NULL)
	{
		// 设置会话远程地址
		sockaddr_in *pRemote = (sockaddr_in *)pFrom;
		USHORT usPort = ntohs(pRemote->sin_port);
		DWORD dwIP = *((DWORD*)&pRemote->sin_addr);
		SetSession(&m_pSession[nIndex], usPort, dwIP, RULE_DIRECTION_IN);
	}
	return GetAccessInfo(&m_pSession[nIndex]);
}


/////////////////////////////////////////////////////////////
// 查看访问权限


int CAcl::GetAccessInfo(SESSION *pSession)
{
	// 如果是主模块访问网络，放行
	if(wcsicmp(g_szCurrentApp, g_szPhoenixFW) == 0)
	{
		return PF_PASS;
	}
	
	// 先查看工作模式
	int nRet;
	if((nRet = GetAccessFromWorkMode(pSession)) != PF_FILTER)
	{
		ODS(L" GetAccessInfo return from WorkMode \n");
		return nRet;
	}

	// 工作模式为过滤，则按照文件中记录的规则过滤之
	::EnterCriticalSection(&g_csGetAccess);

	RULE_ITEM *pItem = NULL;
	int nIndex = 0;
	nRet = PF_PASS;
	while(TRUE)
	{
		// 如果不是第一次查询，则加1，避免查找相同的规则
		if(nIndex > 0) 
			nIndex++;

		nIndex = FindRule(g_szCurrentApp, nIndex);
		if(nIndex >= (int)g_RuleCount)
		{
			if(pItem == NULL)	// 一个记录项也没有，则查询
			{
				// 询问主模块怎么办
				if(!QueryAccess())
				{
					nRet = PF_DENY;
				}
				break;
			}
			else				// 按照上一个记录项处理
			{
				if(pItem->ucAction != RULE_ACTION_PASS)
				{	
					nRet = PF_DENY;
				}
				break;
			}
		}
		
		ODS(L" Find a rule in GetAccessInfo ");
		// 查看规则和会话的属性是否一致
		pItem = &g_Rule[nIndex];
		// 方向
		if(pItem->ucDirection != RULE_DIRECTION_IN_OUT &&
			pItem->ucDirection != pSession->ucDirection)
			continue;
		// 服务类型
		if(pItem->ucServiceType != RULE_SERVICE_TYPE_ALL &&
			pItem->ucServiceType != pSession->nProtocol)
			continue;
		// 服务端口
		if(pItem->usServicePort != RULE_SERVICE_PORT_ALL &&
			 pItem->usServicePort != pSession->usRemotePort)
			 continue;
		// 程序运行到这里，说明找到了一个和会话属性完全相同的规则
		if(pItem->ucAction != RULE_ACTION_PASS)
		{	
			nRet = PF_DENY;
		}	
		break;
	}
	::LeaveCriticalSection(&g_csGetAccess);

	if(nRet == PF_PASS)
		pSession->ucAction = RULE_ACTION_PASS;
	else
		pSession->ucAction =  RULE_ACTION_DENY;
	return nRet;
}

int CAcl::GetAccessFromWorkMode(SESSION *pSession)
{
	if(g_ucWorkMode == PF_PASS_ALL)
		return PF_PASS;

	if(g_ucWorkMode == PF_DENY_ALL)
		return PF_DENY;

	if(g_ucWorkMode == PF_QUERY_ALL)
		return PF_FILTER;

	return PF_UNKNOWN;
}

int CAcl::FindRule(TCHAR *szAppName, int nStart)
{
	// 从指定位置开始查找，返回规则的索引
	for(int nIndex = nStart; nIndex < (int)g_RuleCount; nIndex++)
	{
		if(wcsicmp(szAppName, g_Rule[nIndex].szApplication) == 0)
			break;
	}
	return nIndex;
}

BOOL CAcl::QueryAccess()
{
	ODS(L" QueryAccess ... ");

	// 发送消息
	for(int i=0; i<MAX_QUERY_SESSION; i++)
	{
		if(!g_QuerySession[i].bUsed) // 找到一个没有使用的QuerySession，发出询问
		{
			g_QuerySession[i].bUsed = TRUE;
			wcscpy(g_QuerySession[i].szPathName, g_szCurrentApp);

			if(!::PostMessage(g_hPhoenixWnd, PM_QUERY_ACL_NOTIFY, i, 0))
			{
				g_QuerySession[i].bUsed = FALSE;
				return TRUE;
			}
			// 询问发送成功，等待
			ODS(L"询问发送成功，等待... ");

			int n=0;
			while(g_QuerySession[i].bUsed)
			{
				if(n++ > 3000)		// 等5分钟，如果用户还不决定，就禁止
					return FALSE;
				::Sleep(100);
			}

			if(g_QuerySession[i].nReturnValue == 0)
				return FALSE;
			return TRUE;
		}
	}
	// 用完了
	return FALSE;
}


/////////////////////////////////////////////////////////////////////////////////////
// 会话操作

int CAcl::CreateSession(SOCKET s, int nProtocol)
{
	for(int i=0; i<m_nSessionCount; i++)
	{
		if(m_pSession[i].s == s)
			return i;
	}

	// 确保有足够的内存空间
	if(m_nSessionCount >= m_nSessionMaxCount)	// 已经达到最大数量
	{
		SESSION *pTmp = new SESSION[m_nSessionMaxCount];
		memcpy(pTmp, m_pSession, m_nSessionMaxCount);

		delete[] m_pSession;
		m_pSession = new SESSION[m_nSessionMaxCount*2];

		memcpy(m_pSession, pTmp,  m_nSessionMaxCount);
		delete[] pTmp;
		m_nSessionMaxCount = m_nSessionMaxCount*2;
	}
	// 初始化新的会话
	InitializeSession(&m_pSession[m_nSessionCount]);
	// 设置会话属性
	m_pSession[m_nSessionCount].s = s;
	m_pSession[m_nSessionCount].nProtocol = nProtocol;
	wcscpy(m_pSession[m_nSessionCount].szPathName, g_szCurrentApp);
	m_nSessionCount++;
	
	ODS1(L" CreateSession m_nSessionCount = %d \n", m_nSessionCount);
	// 返回会话索引
	return m_nSessionCount - 1;
}

void CAcl::InitializeSession(SESSION *pSession)
{
	memset(pSession, 0, sizeof(SESSION));
	pSession->ucDirection = RULE_DIRECTION_NOT_SET;
	pSession->ucAction = RULE_ACTION_NOT_SET;
}

void CAcl::DeleteSession(SOCKET s)
{
	for(int i=0; i<m_nSessionCount; i++)
	{
		if(m_pSession[i].s == s)
		{
			// 通知应用程序，有一个会话销毁了
			NotifySession(&m_pSession[i], CODE_DELETE_SESSION);
			memcpy(&m_pSession[i], &m_pSession[i+1], m_nSessionCount - i - 1);
			m_nSessionCount --;
			break;
		}
	}
}

void CAcl::SetSession(SESSION *pSession, USHORT usRemotePort, ULONG ulRemoteIP, UCHAR ucDirection)
{
	pSession->ucDirection = ucDirection;
	if((pSession->usRemotePort != usRemotePort) || (pSession->ulRemoteIP != ulRemoteIP))
	{
		// 根据远程端口号设置远程服务类型
		if(pSession->nProtocol == RULE_SERVICE_TYPE_TCP)
		{
			if(usRemotePort == RULE_SERVICE_PORT_FTP)
				pSession->nProtocol = RULE_SERVICE_TYPE_FTP;
			else if(usRemotePort == RULE_SERVICE_PORT_TELNET)
				pSession->nProtocol = RULE_SERVICE_TYPE_TELNET;
			else if(usRemotePort == RULE_SERVICE_PORT_POP3)
				pSession->nProtocol = RULE_SERVICE_TYPE_POP3;
			else if(usRemotePort == RULE_SERVICE_PORT_SMTP)
				pSession->nProtocol = RULE_SERVICE_TYPE_SMTP;
			else if(usRemotePort  == RULE_SERVICE_PORT_NNTP)
				pSession->nProtocol = RULE_SERVICE_TYPE_NNTP;
			else if(usRemotePort  == RULE_SERVICE_PORT_HTTP)
				pSession->nProtocol = RULE_SERVICE_TYPE_HTTP;
		}

		// 设置其它
		pSession->usRemotePort = usRemotePort;
		pSession->ulRemoteIP = ulRemoteIP;

		// 通知主程序
		NotifySession(pSession, CODE_CHANGE_SESSION);
	}
}

int CAcl::FindSession(SOCKET s)
{
	for(int i=0; i<m_nSessionCount; i++)
	{
		if(m_pSession[i].s == s)
		{
			break;
		}
	}
	return i;
}

void CAcl::NotifySession(SESSION *pSession, int nCode)
{	
	ODS(L" NotifySession... ");
	if(g_hPhoenixWnd != NULL)
	{
		// 在g_SessionBuffer数组中查找一个未使用的成员
		for(int i=0; i<MAX_SESSION_BUFFER; i++)
		{
			if(g_SessionBuffer[i].s == 0)
			{
				g_SessionBuffer[i] = *pSession;
				break;
			}
		}
		// 将会话发送给主模块
		if(i<MAX_SESSION_BUFFER &&
				!::PostMessage(g_hPhoenixWnd, PM_SESSION_NOTIFY, i, nCode))
		{
			// 如果发送失败，恢复成员标识
			g_SessionBuffer[i].s = 0;
		}
	}
}





////////////////////////////////////////////////////////////



int __stdcall PLSPIoControl(LSP_IO_CONTROL *pIoControl, int nType)
{
	switch(nType)
	{
	case IO_CONTROL_SET_RULE_FILE:			// 设置应用层规则
		{
			if(pIoControl->pRuleFile->header.ulLspRuleCount <= MAX_RULE_COUNT)
			{
				g_RuleCount = pIoControl->pRuleFile->header.ulLspRuleCount;
				memcpy(g_Rule, pIoControl->pRuleFile->LspRules, g_RuleCount * sizeof(RULE_ITEM));
			}
		}
		break; 
	case IO_CONTROL_SET_WORK_MODE:			// 设置工作模式
		{
			g_ucWorkMode = pIoControl->ucWorkMode;
		}
		break;
	case IO_CONTROL_GET_WORK_MODE:			// 获取工作模式
		{
			return g_ucWorkMode;
		}
		break;
	case IO_CONTROL_SET_PHOENIX_INSTANCE:	// 设置主模块信息
		{
			g_hPhoenixWnd = pIoControl->hPhoenixWnd;
			wcscpy(g_szPhoenixFW, pIoControl->szPath);
		}
		break;
	case IO_CONTROL_GET_SESSION:			// 获取一个会话
		{
			*pIoControl->pSession = g_SessionBuffer[pIoControl->nSessionIndex];
			// 标识已经不再使用这个成员了
			g_SessionBuffer[pIoControl->nSessionIndex].s = 0;
		}
		break; 
	case IO_CONTROL_SET_QUERY_SESSION:		// 返回DLL询问的结果
		{
			g_QuerySession[pIoControl->nSessionIndex].nReturnValue = pIoControl->ucWorkMode;
			// 标识已经不再使用这个成员了
			g_QuerySession[pIoControl->nSessionIndex].bUsed = FALSE;
		}
		break;
	case IO_CONTROL_GET_QUERY_SESSION:		// 获取发出询问的会话
		{	
			wcscpy(pIoControl->szPath, g_QuerySession[pIoControl->nSessionIndex].szPathName);
		}
		break;
	}
	return 0;
}












/*




int CAcl::CheckSend(SOCKET s, const char *buf, DWORD dwTransed)
{
	int nIndex;
	if(nIndex = FindSession(s) >= m_nSessionCount)
		return PF_PASS;


	return GetAccessInfo(&m_pSession[nIndex]);
}
	int	CheckSend(SOCKET s,  const char *buf, DWORD dwTransed);
	int	CheckRecv(SOCKET s,  const char *buf, DWORD dwTransed);
int CAcl::CheckRecv(SOCKET s, const char *buf, DWORD dwTransed)
{
	int nIndex;
	if(nIndex = FindSession(s) >= m_nSessionCount)
		return PF_PASS;

	return GetAccessInfo(&m_pSession[nIndex]);
}


BOOL CAcl::IsLocalIP(DWORD dwIP)
{
	if(dwIP == 0 || ((BYTE*)&dwIP)[3] == 127)
		return TRUE;
	return FALSE;
}

	// 保持线程同步，首先检查有没有其它线程正在发送这个询问
	for(int i=0; i<MAX_QUERY_SESSION; i++)
	{
		if(g_QuerySession[i].bUsed && 
				(wcscmp(g_QuerySession[i].szPathName, g_szCurrentApp) == 0)) // 已经有会话发出询问，等待
		{
			// 等待
			ODS(L"已经有会话发出询问，等待");
			int n=0;
			while(g_QuerySession[i].bUsed)
			{
				if(n++ > 3000)		// 等5分钟，如果用户还不决定，就禁止
					return FALSE;
				::Sleep(100);
			}

			if(g_QuerySession[i].nReturnValue == 0)
				return FALSE;
			return TRUE;
		}
	}
  */
