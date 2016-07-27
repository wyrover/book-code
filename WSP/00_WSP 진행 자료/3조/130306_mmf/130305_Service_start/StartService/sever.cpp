//sever.cpp
#include "std.h"
SOCKET sock;
SOCKADDR_IN		peeraddr;
int				addrlen;
int sever_init()
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)		
		return -1;

	// socket()
	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");


	// bind()
	SOCKADDR_IN			localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family		= AF_INET;
	localaddr.sin_port			= htons(1122);
	localaddr.sin_addr.s_addr   = htonl(INADDR_ANY);
	retval		= bind(sock, (SOCKADDR*)&localaddr, 
							sizeof(localaddr));


	
	//MessageBox(NULL,"","",MB_OK);
	// 멀티캐스트 데이터 받기
	//logoff();
	return 1;
}
void err_quit(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	MessageBox(NULL, (LPCTSTR)lpMsgBuf, msg, MB_ICONERROR);
	LocalFree(lpMsgBuf);
	exit(-1);
}



// 소켓 함수 오류 출력
void err_display(char *msg)
{
	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER|
		FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&lpMsgBuf, 0, NULL);
	LocalFree(lpMsgBuf);
}

int sever_exit()
{
	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 1;
}

int server_run()
{
	while(1)
	{
		
		// 데이터 받기
		int *i= new int;
		addrlen		= sizeof(peeraddr);
		recvfrom(sock, (char*)i, sizeof(int), 0, 
			(SOCKADDR*)&peeraddr, &addrlen);
		if (*i == FLAG_LOGOFF)
		{ 
			mylogoff();
		}
		if(*i == FLAG_SHUTDOWN)
		{
			myshutdown();	
		}
		free(i);
	}
	return 1;
}
