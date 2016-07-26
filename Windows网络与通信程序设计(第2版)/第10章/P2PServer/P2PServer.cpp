///////////////////////////////////////
// P2PServer.cpp文件


#include "../common/InitSock.h"
#include "../comm.h"

#include <stdio.h>


DWORD WINAPI IOThreadProc(LPVOID lpParam);


CInitSock theSock;

CPeerList  g_PeerList;				// 客户列表
CRITICAL_SECTION g_PeerListLock;	// 同步对客户列表的访问
SOCKET g_s;							// UDP套节字

void main()
{
	// 创建套节字，绑定到本地端口
	g_s = ::WSASocket(AF_INET, 
			SOCK_DGRAM , IPPROTO_UDP, NULL, 0, WSA_FLAG_OVERLAPPED);
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(SERVER_PORT);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	if(::bind(g_s, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" bind() failed %d \n", ::WSAGetLastError());
		return;
	}

	///////////////////////////////////////////////////////
	// 下面这段代码用来显示服务器绑定的终端
	char szHost[256];
	::gethostname(szHost, 256);
	hostent *pHost = ::gethostbyname(szHost);
	in_addr addr;
	for(int i = 0; ; i++)
	{
		char *p = pHost->h_addr_list[i];
		if(p == NULL)
			break;
		
		memcpy(&addr.S_un.S_addr, p, pHost->h_length);
		printf(" bind to local address -> %s:%ld \n", ::inet_ntoa(addr), SERVER_PORT);
	}	
	
	// 开启服务	
	printf(" P2P Server starting... \n\n");


	::InitializeCriticalSection(&g_PeerListLock);
	HANDLE hThread = ::CreateThread(NULL, 0, IOThreadProc, NULL, 0, NULL);

	// 定时向客户方发送“询问”消息，删除不响应的用户
	while(TRUE)
	{
		int nRet = ::WaitForSingleObject(hThread, 15*1000);
		if(nRet == WAIT_TIMEOUT)
		{
			CP2PMessage queryMsg;
			queryMsg.nMessageType = USERACTIVEQUERY;
			DWORD dwTick = ::GetTickCount();
			for(int i=0; i<g_PeerList.m_nCurrentSize; i++)
			{
				if(dwTick - g_PeerList.m_pPeer[i].dwLastActiveTime >= 2*15*1000 + 600) 
				{
					printf(" delete a non-active user: %s \n", g_PeerList.m_pPeer[i].szUserName);
					::EnterCriticalSection(&g_PeerListLock);
					g_PeerList.DeleteAPeer(g_PeerList.m_pPeer[i].szUserName);
					::LeaveCriticalSection(&g_PeerListLock);
					// 因为删了当前遍历到的用户，所以i值就不应该加1了
					i--;
				}
				else
				{
					// 注意，地址列表中的最后一个地址是客户的公共地址，询问消息应该发向这个地址
					sockaddr_in peerAddr = { 0 };
					peerAddr.sin_family = AF_INET;
					peerAddr.sin_addr.S_un.S_addr = 
						g_PeerList.m_pPeer[i].addr[g_PeerList.m_pPeer[i].AddrNum - 1].dwIp;
					peerAddr.sin_port = 
						htons(g_PeerList.m_pPeer[i].addr[g_PeerList.m_pPeer[i].AddrNum - 1].nPort);
					
					::sendto(g_s, (char*)&queryMsg, sizeof(queryMsg), 0, (sockaddr*)&peerAddr, sizeof(peerAddr));
				}
			}	
		} 
		else
		{
			break;
		}
	}

	printf(" P2P Server shutdown. \n");
	::DeleteCriticalSection(&g_PeerListLock);
	::CloseHandle(hThread);
	::closesocket(g_s);
}


DWORD WINAPI IOThreadProc(LPVOID lpParam)
{
	char buff[MAX_PACKET_SIZE];
	CP2PMessage *pMsg = (CP2PMessage*)buff;
	sockaddr_in remoteAddr;
	int nRecv, nAddrLen = sizeof(remoteAddr);
	while(TRUE)
	{
		nRecv = ::recvfrom(g_s, buff, MAX_PACKET_SIZE, 0, (sockaddr*)&remoteAddr, &nAddrLen);
		if(nRecv == SOCKET_ERROR)
		{
			printf(" recvfrom() failed \n");
			continue;
		}
		if(nRecv < sizeof(CP2PMessage))
			continue;

		// 防止用户发送错误的用户名
		pMsg->peer.szUserName[MAX_USERNAME] = '\0'; 
		switch(pMsg->nMessageType)
		{
		case USERLOGIN:		// 有用户登陆			
			{
				// 设置用户的公共终端信息，记录用户的活动时间
				pMsg->peer.addr[pMsg->peer.AddrNum].dwIp = remoteAddr.sin_addr.S_un.S_addr;
				pMsg->peer.addr[pMsg->peer.AddrNum].nPort = ntohs(remoteAddr.sin_port);
				pMsg->peer.AddrNum ++;
				pMsg->peer.dwLastActiveTime = ::GetTickCount();
				

				// 将用户信息保存到用户列表中
				::EnterCriticalSection(&g_PeerListLock);
				BOOL bOK = g_PeerList.AddAPeer(&pMsg->peer);
				::LeaveCriticalSection(&g_PeerListLock);
				if(bOK)
				{
					// 发送确认消息，将用户的公共地址传递过去
					pMsg->nMessageType = USERLOGACK;
					::sendto(g_s, (char*)pMsg, sizeof(CP2PMessage), 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr));	
					
					printf(" has a user login : %s (%s:%ld) \n", 
						pMsg->peer.szUserName, ::inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));
				}
			}
			break;
		case USERLOGOUT:	// 有用户登出
			{
				::EnterCriticalSection(&g_PeerListLock);
				g_PeerList.DeleteAPeer(pMsg->peer.szUserName);
				::LeaveCriticalSection(&g_PeerListLock);
				
				printf(" has a user logout : %s (%s:%ld) \n", 
					pMsg->peer.szUserName, ::inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));
			}
			break;

		case GETUSERLIST:	// 有用户请求发送用户列表
			{
				printf(" sending user list information to %s (%s:%ld)... \n",
					pMsg->peer.szUserName, ::inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));
				CP2PMessage peerMsg;
				peerMsg.nMessageType = GETUSERLIST;
				for(int i=0; i<g_PeerList.m_nCurrentSize; i++)
				{
					memcpy(&peerMsg.peer, &g_PeerList.m_pPeer[i], sizeof(PEER_INFO));
					::sendto(g_s, (char*)&peerMsg, sizeof(CP2PMessage), 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr));
				}

				// 发送结束封包
				peerMsg.nMessageType = USERLISTCMP;
				::sendto(g_s, (char*)&peerMsg, sizeof(CP2PMessage), 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr));
			}
			break;
		case P2PCONNECT:	// 有用户请求让另一个用户向它发送打洞消息
			{				
				char *pszUser = (char*)(pMsg + 1);
				printf(" %s wants to connect to %s \n", pMsg->peer.szUserName, pszUser);
			
				::EnterCriticalSection(&g_PeerListLock);
				PEER_INFO *pInfo = g_PeerList.GetAPeer(pszUser);
				::LeaveCriticalSection(&g_PeerListLock);

				if(pInfo != NULL)
				{
					remoteAddr.sin_addr.S_un.S_addr = pInfo->addr[pInfo->AddrNum -1].dwIp;
					remoteAddr.sin_port = htons(pInfo->addr[pInfo->AddrNum -1].nPort);
					
					::sendto(g_s, (char*)pMsg, 
						sizeof(CP2PMessage), 0, (sockaddr*)&remoteAddr, sizeof(remoteAddr));	
				}
			}
			break;

		case USERACTIVEQUERYACK:	// 用户对“询问”消息的应答	
			{
				printf(" recv active ack message from %s (%s:%ld) \n",
					pMsg->peer.szUserName, ::inet_ntoa(remoteAddr.sin_addr), ntohs(remoteAddr.sin_port));

				::EnterCriticalSection(&g_PeerListLock);
				PEER_INFO *pInfo = g_PeerList.GetAPeer(pMsg->peer.szUserName);
				if(pInfo != NULL)
				{
					pInfo->dwLastActiveTime = ::GetTickCount();
				}
				::LeaveCriticalSection(&g_PeerListLock);
			}
			break;
		}	
	}
	return 0;
}