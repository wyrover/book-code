// BvSocket.cpp: implementation of the CBvSocket class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Phone.h"
#include "BvSocket.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#pragma comment(lib, "wsock32")
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBvSocket::CBvSocket()
{

}

CBvSocket::~CBvSocket()
{

}

void CBvSocket::ReportWinsockErr(LPSTR lpszErrorMsg)
{
	char chMsgBuffer[100];

	wsprintf(chMsgBuffer, "\nWinsock error %d: %s\n\n", WSAGetLastError(), lpszErrorMsg);
	MessageBox(NULL,chMsgBuffer,  AfxGetAppName(), MB_OK|MB_ICONSTOP);
	return;   
}

BOOL CBvSocket::Init(WSADATA* wsaData)
{
	if (WSAStartup(WINSOCK_VERSION,wsaData))
	{
		MessageBeep(MB_ICONSTOP);
		MessageBox(NULL,"Winsock could not be initialized!", AfxGetAppName(), MB_OK|MB_ICONSTOP);
	    WSACleanup();
	    return(FALSE);
	}                  
	return TRUE;
}

BOOL CBvSocket::Clean()
{
	int iErrorCode;
	char chMsgBuffer[100];

	if ((iErrorCode = WSACleanup()))
	{
		wsprintf(chMsgBuffer, "Winsock error %d.", iErrorCode);
		MessageBeep(MB_ICONSTOP);
		MessageBox(NULL, chMsgBuffer,  AfxGetAppName(), MB_OK|MB_ICONSTOP);   
		return FALSE;
    }
	return TRUE;
}

BOOL CBvSocket::GetHostName(char *name, int namelen)
//determine if the local machine is on_line!
{
	if (gethostname(name, namelen))
	{
		ReportWinsockErr("\nCould not resolve local host!\nAre you on-line?\n");
		return FALSE;
	}
	return TRUE;
}

BOOL CBvSocket::Create()
//Create the default socket for general use!
{
	m_hSocket=socket(PF_INET, SOCK_STREAM, 0);
	if (m_hSocket == INVALID_SOCKET)
	{
		ReportWinsockErr("Could not create server socket.");
		return FALSE;
	}
	return TRUE;
}

BOOL CBvSocket::Create(int af, int type, int protocol)
//you can create the socket using the socket,there is equal!
{
	m_hSocket=socket(af,type,protocol);
	if (m_hSocket == INVALID_SOCKET)
	{
		ReportWinsockErr("Could not create server socket.");
		return FALSE;
	}
	return TRUE;
}

void CBvSocket::SetAddrIn(const char FAR* ip, unsigned short port)
//you can set you socket address using the doted string format.
{
	m_addrSocket.sin_family=AF_INET;
	m_addrSocket.sin_addr.S_un.S_addr=inet_addr(ip);
	m_addrSocket.sin_port=htons(port);
}

void CBvSocket::SetAddrIn(unsigned short port)
//you can also let the windows to process the ip,if your machine have
//more than one ip.
{
	m_addrSocket.sin_family=AF_INET;
	m_addrSocket.sin_addr.S_un.S_addr=INADDR_ANY;
	m_addrSocket.sin_port=htons(port);
}

BOOL CBvSocket::Bind()
//if you have used SetAddrIn() create the socket address,you can call
//this default bind function to name the unnamed socket address.
{
	if (bind(m_hSocket,(LPSOCKADDR)&m_addrSocket,sizeof(m_addrSocket)) == SOCKET_ERROR)
	{
		ReportWinsockErr("Could not bind server socket.");
		return FALSE;
	 }
	return TRUE;
}

BOOL CBvSocket::Bind(unsigned short port)
//you can also combine setaddrin() and bind() in one function.
{
	SetAddrIn(port);
	return	Bind();
}

BOOL CBvSocket::Bind(const char *ip, unsigned short port)
//you can also combine setaddrin() and bind() in one function.
//see the description of the SetAddrIn(const char *ip, unsigned short port).
{
	SetAddrIn(ip,port);
	return Bind();
}

BOOL CBvSocket::AsyncSelect(HWND hWnd, unsigned int wMsg, long lEvent)
//This function is just to call the WSAAsyncSelect(...)function,
//wMsg is the user defined Message constant,
//for more information, you can look the MSDN,you also can read a example project.
{
	int iErrorCode = WSAAsyncSelect(m_hSocket, hWnd, wMsg,lEvent);
	if (iErrorCode == SOCKET_ERROR) 
	{
		ReportWinsockErr("WSAAsyncSelect failed on server socket.");
		return FALSE;
	}
	return TRUE;
}

BOOL CBvSocket::Listen()
//just the simple call listen
{
	if (listen(m_hSocket, 5) == SOCKET_ERROR)
	{
		ReportWinsockErr("Server socket failed to listen.");
		return FALSE;
	}
	return TRUE;
}

BOOL CBvSocket::Listen(int queue_size)
//you can set the queue size,using this function.
{
	if (listen(m_hSocket, queue_size) == SOCKET_ERROR)
	{
		ReportWinsockErr("Server socket failed to listen.");
		return FALSE;
	}
	return TRUE;
}

BOOL CBvSocket::Listen(int queue_size,HWND hWnd, unsigned int wMsg)
{
	if(!WSAAsyncSelect(m_hSocket,hWnd,wMsg,FD_ACCEPT))
		return FALSE;
	if(!Listen(queue_size))
		return FALSE;
	return TRUE;
}

BOOL CBvSocket::EndListen()
{
	Close();
	return TRUE;
}

BOOL CBvSocket::Close()
{
	return closesocket(m_hSocket);
}


int CBvSocket::Recieve(char *buf, int len, int flags=0)
{
	return recv(m_hSocket,buf,len,flags);
}

int CBvSocket::Send(const char *buf, int len, int flags=0)
{
	return send(m_hSocket,buf,len,flags);
}

int CBvSocket::Connect()
{
	return connect(m_hSocket,(LPSOCKADDR)&m_addrSocket,sizeof(m_addrSocket));
}

int CBvSocket::SetSocketOpt(int optname, const char *optval, 
							int optlen, int level=SOL_SOCKET)
//for more information you can see the msdn.
{
	return setsockopt(m_hSocket,level,optname,optval,optlen);
}


int CBvSocket::GetSocketOpt(int optname, char *optval,
							int *optlen, int level=SOL_SOCKET)
{
	return getsockopt(m_hSocket,level,optname,optval,optlen);
}

SOCKET CBvSocket::Accept(SOCKADDR *addr, int *addrlen)
{
	return accept(m_hSocket,addr,addrlen);
}


int CBvSocket::Connect(const char FAR* ip, unsigned short port)
{
	SetAddrIn(ip,port);
	return Connect();
}

BOOL CBvSocket::SetAddrIn(long ip, unsigned short port)
{
	return TRUE;
}
