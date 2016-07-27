//-------------------------------------------------------------
//	[  BroadCastReceiver.cpp ]
//-------------------------------------------------------------


#include <winsock2.h>	
#include <stdlib.h>	
#include <stdio.h>

#define BUFSIZE 512


// 소켓 함수 오류 출력 후 종료
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
	printf("[%s] %s", msg, (LPCTSTR)lpMsgBuf);
	LocalFree(lpMsgBuf);
}


int main(int argc, char* argv[])
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)		
		return -1;

	// socket()
	SOCKET sock = socket(AF_INET, SOCK_DGRAM, 0);
	if(sock == INVALID_SOCKET) err_quit("socket()");


	// bind()
	SOCKADDR_IN			localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family		= AF_INET;
	localaddr.sin_port			= htons(9000);
	localaddr.sin_addr.s_addr   = htonl(INADDR_ANY);
	retval		= bind(sock, (SOCKADDR*)&localaddr, 
							sizeof(localaddr));


	// 데이터 통신에 사용할 변수
	SOCKADDR_IN		peeraddr;
	int				addrlen;
	char			buf[BUFSIZE+1];
	
	// 멀티캐스트 데이터 받기
	while(1)
	{
		// 데이터 받기
		addrlen		= sizeof(peeraddr);
		retval		= recvfrom(sock, buf, BUFSIZE, 0, 
					(SOCKADDR*)&peeraddr, &addrlen);

		if(retval == SOCKET_ERROR)
		{
			err_display("recvfrom");
			continue;
		}

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(peeraddr.sin_addr),
						ntohs(peeraddr.sin_port), buf);
	}

	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
