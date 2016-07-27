// 기본 코드 복사해 오세요.

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>	
#pragma comment(lib, "ws2_32.lib")

void main()
{
	WSADATA wsadata;

	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		printf("Can't Initialize Socket !\n");
		return ;
	}
	//--------------------------------------------
	SOCKET s = socket( AF_INET, SOCK_DGRAM, 0);

	char ip[256];
	char msg[256];

	while ( 1 )
	{
		memset( ip, 0, 256);
		memset( msg, 0, 256);


		strcpy(ip, "234.5.5.2");
//		printf("전송할 상대 IP : ");
//		scanf( "%s", ip );
		printf("전송할 문자열 : ");
		scanf( "%s", msg );

		SOCKADDR_IN addr;
		addr.sin_family			= AF_INET;
		addr.sin_port			= htons( 4000 );
		addr.sin_addr.s_addr    = inet_addr( ip );

		sendto( s, msg, strlen(msg)+1, 0, (SOCKADDR*)&addr, sizeof(addr));
	}
	closesocket( s );
	//--------------------------------------------
	WSACleanup();
}









