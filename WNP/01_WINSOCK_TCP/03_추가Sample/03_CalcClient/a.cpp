
#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>	
#pragma comment(lib, "ws2_32.lib")
#include <stdlib.h>

#include "CalcPacket.h"   // 헤더 복사해 오세요..

void main( int argc, char** argv)
{
	if ( argc != 4 )
	{
		printf("usage : CalcClient  <first number>  <+,-,*,/> <second number>\n");
		return;
	}
	int first = atoi(argv[1]);
	int second = atoi(argv[3]);
	int cmd    = (int)(argv[2][0]); 
	//----------------------------------------------------------------
	WSADATA wsadata;

	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		printf("Can't Initialize Socket !\n");
		return ;
	}
	//--------------------------------------------
	SOCKET s = socket( AF_INET, SOCK_STREAM, 0);
	SOCKADDR_IN addr = {0};
	addr.sin_family = AF_INET;
	addr.sin_port   = htons( 4000 );
	addr.sin_addr.s_addr = inet_addr("61.81.99.138"); // ????

	if ( connect(s, (SOCKADDR*)&addr, sizeof(addr)) == -1 )
	{
		printf("서버에 접속할수 없습니다.\n");
		return ;
	}
	// 서버에 packet을 보낸다.
	CALCPACKET packet = { cmd, first, second };

	send( s, (char*)&packet, sizeof(packet),0);

	//--------------------------------------------------------
	// 돌아오는 packet을 수신한다.
	int total = sizeof(packet); // 수신해야 하는 크기
	int current = 0;            // 현재 수신한 총 크기
	int nRead = 0;

	while ( total > current ) // 아직 다 받지 못한 경우
	{
		nRead = recv( s, (char*)&packet + current, total - current, 0 );

		if ( nRead > 0 )
		{
			current += nRead;
			continue; // 계속 수신한다
		}
		else if ( nRead == 0 ) // 접속이 끊어 진 경우
		{
			break;
		}
		else if ( nRead < 0 ) // 수신에러 발생
		{
			break;
		}
	}
	//===================================================
	printf("결과 : %d\n", packet.first);
	closesocket( s );
	//--------------------------------------------
	WSACleanup();
}











