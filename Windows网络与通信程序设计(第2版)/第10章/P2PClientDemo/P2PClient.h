////////////////////////////////////////////////////////
// p2pclient.h文件

#ifndef __P2PCLIENT_H__
#define __P2PCLIENT_H__

#include <windows.h>
#include "../comm.h"

class CP2PClient
{
public:
	CP2PClient();
	~CP2PClient();
	// 初始化对象的成员
	BOOL Init(USHORT usLocalPort = 0);

	// 登陆服务器，登出服务器
	BOOL Login(char *pszUserName, char *pszServerIp);
	void Logout();

	// 向服务器请求用户列表，更新用户列表记录
	BOOL GetUserList();

	// 向一个用户发送消息
	BOOL SendText(char *pszUserName, char *pszText, int nTextLen);

	// 接收到来消息的虚函数
	virtual void OnRecv(char *pszUserName, char *pszData, int nDataLen) { }

	// 用户列表
	CPeerList m_PeerList;

protected:
	void HandleIO(char *pBuf, int nBufLen, sockaddr *addr, int nAddrLen);
	static DWORD WINAPI RecvThreadProc(LPVOID lpParam);

	CRITICAL_SECTION m_PeerListLock;	// 同步对用户列表的访问

	SOCKET m_s;				// 用于P2P通信的套节字句柄
	HANDLE m_hThread;		// 线程句柄	
	WSAOVERLAPPED m_ol;		// 用于等待网络事件的重叠结构

	PEER_INFO m_LocalPeer;	// 本用户信息

	DWORD m_dwServerIp;		// 服务器IP地址

	BOOL m_bThreadExit;		// 用于指示接收线程退出

	BOOL m_bLogin;			// 是否登陆
	BOOL m_bUserlistCmp;	// 用户列表是否传输结束
	BOOL m_bMessageACK;		// 是否接收到消息应答
};


#endif // __P2PCLIENT_H__