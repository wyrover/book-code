// blocksock.cpp (CMyBlockSocketException, CMyBlockSocket, CMyHttpBlockSocket)
#include <stdafx.h>
#include "blocksock.h"

// Class CMyBlockSocketException
IMPLEMENT_DYNAMIC(CMyBlockSocketException, CException)

CMyBlockSocketException::CMyBlockSocketException(char* pchMessage)
{
	m_strMessage = pchMessage;
	m_nError = WSAGetLastError();
}

BOOL CMyBlockSocketException::GetErrorMessage(LPTSTR lpstrError, UINT nMaxError,
		PUINT pnHelpContext /*= NULL*/)
{

	char text[200];
	if(m_nError == 0) {
		wsprintf(text, "%s 错误", (const char*) m_strMessage);
	}
	else {
		wsprintf(text, "%s   错误号：#%d", (const char*) m_strMessage, m_nError);
	}
	strncpy(lpstrError, text, nMaxError - 1);
	return TRUE;
}

// Class CMyBlockSocket
IMPLEMENT_DYNAMIC(CMyBlockSocket, CObject)

void CMyBlockSocket::Cleanup()
{
	if(m_hSocket == NULL) return;
	VERIFY(closesocket(m_hSocket) != SOCKET_ERROR);
	m_hSocket = NULL;
}

void CMyBlockSocket::Create(int nType /* = SOCK_STREAM */)
{
	ASSERT(m_hSocket == NULL);
	if((m_hSocket = socket(AF_INET, nType, 0)) == INVALID_SOCKET) {
		throw new CMyBlockSocketException("创建套接字(Create)");
	}
}

void CMyBlockSocket::Bind(LPCSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	if(bind(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("绑定地址(Bind)");
	}
}

void CMyBlockSocket::Listen()
{
	ASSERT(m_hSocket != NULL);
	if(listen(m_hSocket, 5) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("侦听连接(Listen)");
	}
}

BOOL CMyBlockSocket::Accept(CMyBlockSocket& sConnect, LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	ASSERT(sConnect.m_hSocket == NULL);
	int nLengthAddr = sizeof(SOCKADDR);
	sConnect.m_hSocket = accept(m_hSocket, psa, &nLengthAddr);
	if(sConnect == INVALID_SOCKET) {
		if(WSAGetLastError() != WSAEINTR) {
			throw new CMyBlockSocketException("接受连接(Accept)");
		}
		return FALSE;
	}
	return TRUE;
}

void CMyBlockSocket::Close()
{
	ASSERT(m_hSocket != NULL);
	if(closesocket(m_hSocket) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("关闭套接字(Close)");
	}
	m_hSocket = NULL;
}

void CMyBlockSocket::Connect(LPCSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	if(connect(m_hSocket, psa, sizeof(SOCKADDR)) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("发送连接(Connect)");
	}
}

int CMyBlockSocket::Write(const char* pch, const int nSize, const int nSecs)
{
	int nBytesSent = 0;
	int nBytesThisTime;
	const char* pch1 = pch;
	do {
		nBytesThisTime = Send(pch1, nSize - nBytesSent, nSecs);
		nBytesSent += nBytesThisTime;
		pch1 += nBytesThisTime;
	} while(nBytesSent < nSize);
	return nBytesSent;
}

int CMyBlockSocket::Send(const char* pch, const int nSize, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) {
		throw new CMyBlockSocketException("发送超时(Send timeout)");
	}
	int nBytesSent;
	if((nBytesSent = send(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("发送数据(Send)");
	}
	return nBytesSent;
}

int CMyBlockSocket::Receive(char* pch, const int nSize, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) {
		throw new CMyBlockSocketException("接收超时(Receive timeout)");
	}

	int nBytesReceived;
	if((nBytesReceived = recv(m_hSocket, pch, nSize, 0)) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("接收数据(Receive)");
	}
	return nBytesReceived;
}

int CMyBlockSocket::ReceiveDatagram(char* pch, const int nSize, LPSOCKADDR psa, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, &fd, NULL, NULL, &tv) == 0) {
		throw new CMyBlockSocketException("接收超时(Receive timeout)");
	}

	int nFromSize = sizeof(SOCKADDR);
	int nBytesReceived = recvfrom(m_hSocket, pch, nSize, 0, psa, &nFromSize);
	if(nBytesReceived == SOCKET_ERROR) {
		throw new CMyBlockSocketException("接收数据报(ReceiveDatagram)");
	}
	return nBytesReceived;
}

int CMyBlockSocket::SendDatagram(const char* pch, const int nSize, LPCSOCKADDR psa, const int nSecs)
{
	ASSERT(m_hSocket != NULL);
	FD_SET fd = {1, m_hSocket};
	TIMEVAL tv = {nSecs, 0};
	if(select(0, NULL, &fd, NULL, &tv) == 0) {
		throw new CMyBlockSocketException("发送超时(Send timeout)");
	}

	int nBytesSent = sendto(m_hSocket, pch, nSize, 0, psa, sizeof(SOCKADDR));
	if(nBytesSent == SOCKET_ERROR) {
		throw new CMyBlockSocketException("发送数据报(SendDatagram)");
	}
	return nBytesSent;
}

void CMyBlockSocket::GetPeerAddr(LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	int nLengthAddr = sizeof(SOCKADDR);
	if(getpeername(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("获得对方地址(GetPeerName)");
	}
}

void CMyBlockSocket::GetSockAddr(LPSOCKADDR psa)
{
	ASSERT(m_hSocket != NULL);
	int nLengthAddr = sizeof(SOCKADDR);
	if(getsockname(m_hSocket, psa, &nLengthAddr) == SOCKET_ERROR) {
		throw new CMyBlockSocketException("获得本方地址(GetSockName)");
	}
}

CSocketAddress CMyBlockSocket::GetHostByName(const char* pchName, const USHORT ushPort /* = 0 */)
{
	hostent* pHostEnt = gethostbyname(pchName);
	if(pHostEnt == NULL) {
		throw new CMyBlockSocketException("通过名字查找主机(GetHostByName)");
	}
	ULONG* pulAddr = (ULONG*) pHostEnt->h_addr_list[0];
	SOCKADDR_IN sockTemp;
	sockTemp.sin_family = AF_INET;
	sockTemp.sin_port = htons(ushPort);
	sockTemp.sin_addr.s_addr = *pulAddr;
	return sockTemp;
}

const char* CMyBlockSocket::GetHostByAddr(LPCSOCKADDR psa)
{
	hostent* pHostEnt = gethostbyaddr((char*) &((LPSOCKADDR_IN) psa)
				->sin_addr.s_addr, 4, PF_INET);
	if(pHostEnt == NULL) {
		throw new CMyBlockSocketException("通过地址查找主机(GetHostByAddr)");
	}
	return pHostEnt->h_name;
}

// Class CMyHttpBlockSocket
IMPLEMENT_DYNAMIC(CMyHttpBlockSocket, CMyBlockSocket)

CMyHttpBlockSocket::CMyHttpBlockSocket()
{
	m_pReadBuf = new char[nSizeRecv];
	m_nReadBuf = 0;
}

CMyHttpBlockSocket::~CMyHttpBlockSocket()
{
	delete [] m_pReadBuf;
}

//读取整个头信息，并在结尾加上字符串的结束标志
int CMyHttpBlockSocket::ReadHttpHeaderLine(char* pch, const int nSize, const int nSecs)
{
	int nBytesThisTime = m_nReadBuf;
	int nLineLength = 0;
	char* pch1 = m_pReadBuf;
	char* pch2;
	do {
		if((pch2 = (char*) memchr(pch1 , '\n', nBytesThisTime)) != NULL) {
			ASSERT((pch2) > m_pReadBuf);
			ASSERT(*(pch2 - 1) == '\r');
			nLineLength = (pch2 - m_pReadBuf) + 1;
			if(nLineLength >= nSize) nLineLength = nSize - 1;
			memcpy(pch, m_pReadBuf, nLineLength);
			m_nReadBuf -= nLineLength;
			memmove(m_pReadBuf, pch2 + 1, m_nReadBuf);
			break;
		}
		pch1 += nBytesThisTime;
		nBytesThisTime = Receive(m_pReadBuf + m_nReadBuf, nSizeRecv - m_nReadBuf, nSecs);
		if(nBytesThisTime <= 0) {
			throw new CMyBlockSocketException("获取头部信息(ReadHeaderLine)");
		}
		m_nReadBuf += nBytesThisTime;
	}
	while(TRUE);
	*(pch + nLineLength) = '\0';
	return nLineLength;
}

//读取传输的信息的其它部分(假定头信息已经读取了)
int CMyHttpBlockSocket::ReadHttpResponse(char* pch, const int nSize, const int nSecs)
{
	int nBytesToRead, nBytesThisTime, nBytesRead = 0;
	if(m_nReadBuf > 0) {
		memcpy(pch, m_pReadBuf, m_nReadBuf);
		pch += m_nReadBuf;
		nBytesRead = m_nReadBuf;
		m_nReadBuf = 0;
	}
	do {
		nBytesToRead = min(nSizeRecv, nSize - nBytesRead);
		nBytesThisTime = Receive(pch, nBytesToRead, nSecs);
		if(nBytesThisTime <= 0) break;
		pch += nBytesThisTime;
		nBytesRead += nBytesThisTime;
	}
	while(nBytesRead <= nSize);
	return nBytesRead;
}

