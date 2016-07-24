// MultiCast.cpp: implementation of the CMultiCast class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MultiCastChat.h"
#include "MultiCast.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMultiCast::CMultiCast()
{
	m_hWnd = NULL;
	m_bConnected = FALSE;
}

CMultiCast::~CMultiCast()
{

}

BOOL CMultiCast::Create(CString lpstrAddr, unsigned short port, HWND hWnd)
{
	if(m_bConnected){
		TRACE("Has Successfully Create the socket!\n");
		return FALSE;
	}
	m_hWnd = hWnd;
	int ret;

    // 将字符串地址转换为套接字地址
	int iLen = 0;
	int cbRet = 0;
	int nIP_TTL = 16;//设置IP数据生存期
	BOOL bFlag;
	iLen = sizeof(addr);

	m_hSocket = WSASocket(AF_INET, SOCK_DGRAM, 0, NULL, 0, 
		WSA_FLAG_OVERLAPPED | WSA_FLAG_MULTIPOINT_C_LEAF | WSA_FLAG_MULTIPOINT_D_LEAF);
	if (m_hSocket == INVALID_SOCKET){
		TRACE("WSASocket:%d\n",WSAGetLastError());
		return FALSE;
    } 

	bFlag = TRUE; // 设置套接字为可重用端口地址
	ret = setsockopt(m_hSocket,	SOL_SOCKET,	SO_REUSEADDR, (char *)&bFlag,
				sizeof (bFlag)); 

	if(ret == SOCKET_ERROR){
		TRACE("setsockopt:%d",WSAGetLastError());
		return FALSE;
    } 
    // 将套接字绑扎到用户指定端口及默认的接口
    addr.sin_family = PF_INET;
    addr.sin_port = htons (port);
    addr.sin_addr.s_addr = INADDR_ANY;
	ret = bind (m_hSocket, (struct sockaddr FAR *)&addr, 
		sizeof(struct sockaddr));

    if (ret == SOCKET_ERROR){
		TRACE("bind: %d", WSAGetLastError());
		return FALSE;
    }
    // 设置多址广播数据报传播范围（TTL）
	ret = WSAIoctl(m_hSocket, SIO_MULTICAST_SCOPE, &nIP_TTL, 
		sizeof (nIP_TTL), NULL, 0, (unsigned long*)&cbRet, NULL, NULL);           		 
    if (ret == SOCKET_ERROR){
		TRACE("SAIoctl(SIO_MULTICAST_SCOPE):%d\n",WSAGetLastError());
		return FALSE;
	}
			
	char* chAddr = lpstrAddr.GetBuffer(0);
	ret = WSAStringToAddress(chAddr, AF_INET, 
		NULL, (LPSOCKADDR)&addr, &iLen);          		
	if(ret == SOCKET_ERROR){
		m_bConnected = FALSE;
		TRACE("WSAStringToAddress:%d\n",WSAGetLastError());
		return FALSE;
	}

	srcaddr = addr;
	srcaddr.sin_port = htons (port);
	WSABUF wsaCalleeData;
	m_hGroupSocket = WSAJoinLeaf(m_hSocket,	(PSOCKADDR)&addr,	
		sizeof(addr), NULL, &wsaCalleeData, NULL, NULL, JL_BOTH);				 

    if (m_hGroupSocket == INVALID_SOCKET){
		m_bConnected = FALSE;
		TRACE("WSAJoinLeaf():%d\n", WSAGetLastError());
		return FALSE;
    }
	ret = WSAAsyncSelect(m_hGroupSocket, m_hWnd, WM_MULTIPOINTEVENT, 
		FD_WRITE | FD_READ | FD_QOS | FD_GROUP_QOS | FD_CONNECT);

	if(ret == SOCKET_ERROR){
		TRACE("WSAAsyncSelect():%d\n", WSAGetLastError());
		m_bConnected = FALSE;
		return FALSE;
	}
	m_bConnected = TRUE;
	return TRUE;
}

int CMultiCast::Send(char *lpData, int size)
{
	if(m_hSocket == INVALID_SOCKET)
		return -1;
	unsigned long nDataSent = 0;
	int nReturnCode;
	WSABUF wsaBuf;
	wsaBuf.len = size;
	wsaBuf.buf = lpData;
	nReturnCode = WSASendTo(m_hSocket, &wsaBuf, 1, &nDataSent, 
			0, (LPSOCKADDR)&srcaddr, sizeof(SOCKADDR), NULL, NULL);
	if(nReturnCode == SOCKET_ERROR){
		TRACE0("Error in send packets to the network!\n");
		return -1;
	}
	
	return nDataSent;
}

int CMultiCast::Receive(char *lpData, int size)
{
	int nReturnCode ;
	WSABUF wsaRecvBuf;
	wsaRecvBuf.buf = lpData;
	wsaRecvBuf.len = size;
	unsigned long cbRet = 0;

	int	iLen = sizeof (srcaddr);
	int	dFlag = 0;
	
	nReturnCode = WSARecvFrom(m_hGroupSocket, &wsaRecvBuf, 1,
		&cbRet, (unsigned long*)&dFlag, (struct sockaddr *)&srcaddr,
		&iLen, NULL, NULL); 
	if(nReturnCode == SOCKET_ERROR){
		if(WSAEWOULDBLOCK == WSAGetLastError())
			TRACE("Socket will block!\n");
		else{
			if(WSAGetLastError() == WSA_IO_PENDING)
				TRACE("IO is pending!\n");
			else
				return -1;
		}
		TRACE("RecvFrom Error:%d\n", WSAGetLastError());
	}
	//本次操作没有接收到数据
	if(cbRet == 0)
		return 0;

	//通知父窗口读数据
	return cbRet;
}

void CMultiCast::Initilize()
{
	WSADATA wsaData;
	WORD version = MAKEWORD(2, 0);
	int ret = WSAStartup(version, &wsaData);
	if(ret != 0)
		TRACE("Initilize Error!\n");
}

void CMultiCast::Unintilize()
{
	if (WSACleanup() != 0){
		TRACE("UnInitilize Error:%d\n", WSAGetLastError());
	}
}

BOOL CMultiCast::IsConnected()
{
	return m_bConnected;
}

void CMultiCast::Close()
{
	closesocket(m_hSocket);
	closesocket(m_hGroupSocket);
	m_bConnected = FALSE;
}
