//-------------------------------------------------------------
//	[  UDPServer.cpp ]
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
	SOCKADDR_IN			serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family		= AF_INET;
	serveraddr.sin_port			= htons(9000);
	serveraddr.sin_addr.s_addr   = htonl(INADDR_ANY);
	retval		= bind(sock, (SOCKADDR*)&serveraddr, 
							sizeof(serveraddr));
	if( retval == SOCKET_ERROR)
		err_quit("bind");


	// 데이터 통신에 사용할 변수
	SOCKADDR_IN		clientaddr;
	int				addrlen;
	char			buf[BUFSIZE+1];
	

	// 클라이언트와 데이터 통신
	while(1)
	{
		// 데이터 받기
		addrlen		= sizeof(clientaddr);
		retval		= recvfrom(sock, buf, BUFSIZE, 0, 
					(SOCKADDR*)&clientaddr, &addrlen);

		if(retval == SOCKET_ERROR)
		{
			err_display("recvfrom");
			continue;
		}

		// 받은 데이터 출력
		buf[retval] = '\0';
		printf("[UDP/%s:%d] %s\n", inet_ntoa(clientaddr.sin_addr),
						ntohs(clientaddr.sin_port), buf);

		// 데이터 보내기
		retval = sendto(sock, buf, retval, 0, 
				(SOCKADDR*)&clientaddr, sizeof(clientaddr));
		if( retval == SOCKET_ERROR)
		{
			err_display("sendto");
			continue;
		}
	}


	// closesocket()
	closesocket(sock);

	// 윈속 종료
	WSACleanup();
	return 0;
}
