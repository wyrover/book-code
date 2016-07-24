// BvSocket.h: interface for the CBvSocket class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BVSOCKET_H__7BD31721_08C8_11D4_8D3A_00002100A114__INCLUDED_)
#define AFX_BVSOCKET_H__7BD31721_08C8_11D4_8D3A_00002100A114__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "winsock.h"
const int WINSOCK_VERSION = 0x0101;	 //Socket version over 1.1

class CBvSocket  
{
public:
	CBvSocket();
	virtual ~CBvSocket();
public:
	BOOL SetAddrIn(long ip,unsigned short port);
	int Connect(const char FAR* ip, unsigned short port);
	SOCKET Accept(SOCKADDR* addr, int* addrlen);
	int GetSocketOpt(int optname,char FAR* optval,int FAR*  optlen,int level);
	int SetSocketOpt(int optname,const char FAR * optval,int optlen,int level);
	int Connect();
	int Send(const char FAR * buf,int len,int flags);
	int Recieve(char FAR* buf,int len,int flags);
	BOOL Close();
	BOOL EndListen();
	BOOL Listen(int queue_size,HWND hWnd,unsigned int wMsg);
	BOOL Listen(int queue_size);
	BOOL Listen();
	BOOL AsyncSelect(HWND hWnd,unsigned int wMsg,long lEvent);
	BOOL Bind(const char FAR* ip,unsigned short port);
	BOOL Bind(unsigned short port);
	BOOL Bind();
	void SetAddrIn(unsigned short port);
	void SetAddrIn(const char FAR* ip,unsigned short port);
	BOOL Create(int af, int type,int protocol);
	BOOL Create();
	void ReportWinsockErr(LPSTR lpszErrorMsg);
	BOOL GetHostName(char FAR* name,int namelen);
	static BOOL Clean();
	static BOOL Init(WSADATA* wsaData);
	SOCKET	m_hSocket;
	SOCKADDR_IN  m_addrSocket;
	SOCKET Socket(){return m_hSocket;}
	SOCKADDR_IN SockAddr(){return m_addrSocket;}
	SOCKADDR* GetSockAddr(){return (SOCKADDR*)&m_addrSocket;}
	const CBvSocket& operator=(const CBvSocket& s){return s;}
	CBvSocket(SOCKET s,SOCKADDR_IN addr){m_hSocket=s;m_addrSocket=addr;}
};

#endif // !defined(AFX_BVSOCKET_H__7BD31721_08C8_11D4_8D3A_00002100A114__INCLUDED_)
