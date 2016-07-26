////////////////////////////////////////
// GroupTalk.cpp文件

#include "GroupTalk.h"
#include <windows.h>
#include <Ws2tcpip.h>	// IP_MULTICAST_TTL、IP_MULTICAST_IF都定义在Ws2tcpip.h文件



BOOL CGroupTalk::JoinGroup()
{
	// 加入会议组
	ip_mreq	mcast;
	mcast.imr_interface.S_un.S_addr = INADDR_ANY;
	mcast.imr_multiaddr.S_un.S_addr = m_dwMultiAddr;
	int nRet = ::setsockopt(m_sRead, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char*)&mcast, sizeof(mcast));
	
	if(nRet != SOCKET_ERROR)
	{
		// 向组中所有成员发送MT_JION消息，告诉它们自己的用户信息
		char buf[sizeof(GT_HDR)] = { 0 };
		GT_HDR *pHeader = (GT_HDR *)buf;
		pHeader->gt_type = MT_JION;
		strncpy(pHeader->szUser, m_szUser, 15);
		Send(buf, sizeof(GT_HDR), m_dwMultiAddr);
		return TRUE;
	}
	return FALSE;
}

BOOL CGroupTalk::LeaveGroup()
{
	// 离开会议组
	ip_mreq	mcast;
	mcast.imr_interface.S_un.S_addr = INADDR_ANY;
	mcast.imr_multiaddr.S_un.S_addr = m_dwMultiAddr;
	int nRet = ::setsockopt(m_sRead, IPPROTO_IP, IP_DROP_MEMBERSHIP, (char*)&mcast, sizeof(mcast));

	if(nRet != SOCKET_ERROR)
	{
		// 向组中所有成员发送MT_LEAVE消息，告诉它们自己离开了
		char buf[sizeof(GT_HDR)] = { 0 };
		GT_HDR *pHeader = (GT_HDR *)buf;
		pHeader->gt_type = MT_LEAVE;
		strncpy(pHeader->szUser, m_szUser, 15);
		Send(buf, sizeof(GT_HDR), m_dwMultiAddr);
		return TRUE;
	}
	return FALSE;
}

int CGroupTalk::Send(char *szText, int nLen, DWORD dwRemoteAddr)
{
	// 发送UDP封包
	sockaddr_in dest;
	dest.sin_family = AF_INET;
	dest.sin_addr.S_un.S_addr = dwRemoteAddr;
	dest.sin_port = ::ntohs(GROUP_PORT);
	return ::sendto(m_sSend, szText, nLen, 0, (sockaddr*)&dest, sizeof(dest));
}


CGroupTalk::CGroupTalk(HWND hNotifyWnd, DWORD dwMultiAddr, DWORD dwLocalAddr, int nTTL)
{
	m_hNotifyWnd = hNotifyWnd;
	m_dwMultiAddr = dwMultiAddr;
	m_dwLocalAddr = dwLocalAddr;
	m_nTTL = nTTL;
	m_bQuit = FALSE;

	// 取得本机的用户名作为当前客户用户名
	DWORD dw = 256;
	::gethostname(m_szUser, dw);
	//创建事件对象和工作线程
	m_hEvent = ::WSACreateEvent();
	m_hThread = ::CreateThread(NULL, 0, _GroupTalkEntry, this, 0, NULL);
}

CGroupTalk::~CGroupTalk()
{
	// 通知工作线程退出，等它退出后，释放资源
	m_bQuit = TRUE;
	::SetEvent(m_hEvent);
	::WaitForSingleObject(m_hThread, INFINITE);
	::CloseHandle(m_hThread);
	::CloseHandle(m_hEvent);
}


DWORD WINAPI _GroupTalkEntry(LPVOID lpParam)
{
	CGroupTalk *pTalk = (CGroupTalk *)lpParam;

	// 创建发送套节字和接收套节字         
	pTalk->m_sSend = ::socket(AF_INET, SOCK_DGRAM, 0);
	pTalk->m_sRead = ::WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, WSA_FLAG_OVERLAPPED);

	// 设置允许其它套节字也接收此接收套节字所监听端口的地址
	BOOL bReuse = TRUE;
	::setsockopt(pTalk->m_sRead, SOL_SOCKET, SO_REUSEADDR, (char*)&bReuse, sizeof(BOOL));

	// 设置多播封包的TTL值
	::setsockopt(pTalk->m_sSend, 
				IPPROTO_IP, IP_MULTICAST_TTL, (char*)&pTalk->m_nTTL, sizeof(pTalk->m_nTTL));
	
	// 设置要使用的发送接口
	setsockopt(pTalk->m_sSend, 
			IPPROTO_IP, IP_MULTICAST_IF, (char*)&pTalk->m_dwLocalAddr, sizeof(pTalk->m_dwLocalAddr));

	// 绑定接收套节字到本地端口
	sockaddr_in si;
	si.sin_family = AF_INET;
	si.sin_port = ::ntohs(GROUP_PORT);
	si.sin_addr.S_un.S_addr = pTalk->m_dwLocalAddr;
	int nRet = ::bind(pTalk->m_sRead, (sockaddr*)&si, sizeof(si));
	if(nRet == SOCKET_ERROR)
	{		
		::closesocket(pTalk->m_sSend);
		::closesocket(pTalk->m_sRead);
		::SendMessage(pTalk->m_hNotifyWnd, WM_GROUPTALK, -1, (long)"bind failed! \n");
		return -1;
	}

	// 加入多播组
	if(!pTalk->JoinGroup())
	{
		::closesocket(pTalk->m_sSend);
		::closesocket(pTalk->m_sRead);
		::SendMessage(pTalk->m_hNotifyWnd, WM_GROUPTALK, -1, (long)"JoinGroup failed! \n");
		return -1;
	}

	// 循环接收到来的封包
	WSAOVERLAPPED ol = { 0 };
	ol.hEvent = pTalk->m_hEvent;
	WSABUF buf;
	buf.buf = new char[BUFFER_SIZE];
	buf.len = BUFFER_SIZE;	
	while(TRUE)
	{
		// 投递接收I/O
		DWORD dwRecv;
		DWORD dwFlags = 0;
		sockaddr_in saFrom;
		int nFromLen = sizeof(saFrom);
		int ret = ::WSARecvFrom(pTalk->m_sRead, 
						&buf, 1, &dwRecv, &dwFlags, (sockaddr*)&saFrom, &nFromLen, &ol, NULL);
		if(ret == SOCKET_ERROR)
		{
			if(::WSAGetLastError() != WSA_IO_PENDING)
			{
				::SendMessage(pTalk->m_hNotifyWnd, WM_GROUPTALK, -1, (long)"PostRecv failed! \n");
				pTalk->LeaveGroup();	
				::closesocket(pTalk->m_sSend);
				::closesocket(pTalk->m_sRead);
				break;
			}
		}

		// 等待I/O完成，处理封包
		::WSAWaitForMultipleEvents(1, &pTalk->m_hEvent, TRUE, WSA_INFINITE, FALSE);
		if(pTalk->m_bQuit)		// 是否退出？
		{
			pTalk->LeaveGroup();	
			::closesocket(pTalk->m_sSend);
			::closesocket(pTalk->m_sRead);
			break;
		}
		BOOL b = ::WSAGetOverlappedResult(pTalk->m_sRead, &ol, &dwRecv, FALSE, &dwFlags);
		if(b && dwRecv >= sizeof(GT_HDR))
		{	
			GT_HDR *pHeader = (GT_HDR*)buf.buf;	
			// 填写源地址信息
			pHeader->dwAddr = saFrom.sin_addr.S_un.S_addr;
			pTalk->DispatchMsg(pHeader, dwRecv);
		}
	}

	delete buf.buf;
	return 0;
}

// 处理到来的消息，将它们分发到主窗口
void CGroupTalk::DispatchMsg(GT_HDR *pHeader, int nLen)
{
	if(pHeader->gt_type == MT_JION)	// 新用户加入
	{	
		// 向新加入用户发送自己的用户信息
		char buff[sizeof(GT_HDR)] = { 0 };
		GT_HDR *pSend = (GT_HDR*)buff;
		strncpy(pSend->szUser, m_szUser, 15);	
		pSend->gt_type = MT_MINE;
		pSend->nDataLength = 0;
		Send(buff, sizeof(GT_HDR), pHeader->dwAddr);
	}
	else if(pHeader->gt_type == MT_MINE)
	{	
		// 是否来自自己，如果是，则不处理
		if(strcmp(pHeader->szUser, m_szUser) == 0)
			return;
		// 为简单起见，把在线用户当成新加入用户处理
		pHeader->gt_type = MT_JION;
	}

	// 通知主窗口处理
	::SendMessage(m_hNotifyWnd, WM_GROUPTALK, 0, (LPARAM)pHeader);
}



int CGroupTalk::SendText(char *szText, int nLen, DWORD dwRemoteAddr)
{
	// 构建消息封包
	char buf[sizeof(GT_HDR) + 1024] = { 0 };
	GT_HDR *pHeader = (GT_HDR *)buf;
	pHeader->gt_type = MT_MESG;
	pHeader->nDataLength = nLen < 1024 ? nLen : 1024;
	strncpy(pHeader->szUser, m_szUser, 15);
	strncpy(pHeader->data(), szText, pHeader->nDataLength);

	// 发送此封包
	int nSends = Send(buf, pHeader->nDataLength + sizeof(GT_HDR), dwRemoteAddr == 0 ? m_dwMultiAddr : dwRemoteAddr);
	return nSends - sizeof(GT_HDR);
}