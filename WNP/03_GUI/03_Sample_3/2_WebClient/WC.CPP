// 네트워크 기본 코드 복사해 오세요..

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>	
#include <ws2tcpip.h>
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
	char url[256] = { 0 };
	printf("접속할 URL 을 넣어주세요 >> ");
	scanf("%s", url );

	// URL -> IP 로 변경하기.
	HOSTENT* p = gethostbyname( url );
	
	int index = 0;

	while ( p->h_addr_list[index] )
	{
		in_addr addr = *((in_addr*)(p->h_addr_list[index]));

		printf("IP Address : %s\n", inet_ntoa( addr ) );

		index++;
	}
	//------------------------------------------------
	in_addr serverip = *((in_addr*)(p->h_addr_list[0]));

	SOCKET s = socket( AF_INET, SOCK_STREAM, 0);

	// 서버 단의 소켓은 종료 후에도 TIME_WAIT 만큼 대기 하므로 
	// 종료후 즉시 다시 실행했을때 bind()가 실패 한다.(리눅스/unix 에서)

	// 서버 종료후 바로 재시동 할수 있게 하려면 아래 Option 을 지정해야 한다.
	BOOL b = TRUE;

	setsockopt( s,  SOL_SOCKET,  // option level
					SO_REUSEADDR, // option name
					(char*)&b,    // option 설정 값 
					sizeof(b) );  // option 값의 크기.


	// 수신 버퍼를 키우려면 역시 option을 지정한다.
	int sz = 8192;
	setsockopt( s, SOL_SOCKET,	SO_RCVBUF, (char*)&sz, sizeof(sz));

	// UDP client 가 BROADCAST 가 가능하게 하는 option(x.x.x.255)
	BOOL bBroad = TRUE;
	setsockopt( s, SOL_SOCKET, SO_BROADCAST, (char*)&bBroad, sizeof( bBroad));


	SOCKADDR_IN addr = { 0 };
	addr.sin_family		= AF_INET;
	addr.sin_port		= htons(80);	// web server port 번호.
	addr.sin_addr		= serverip;


	if ( connect( s, (SOCKADDR*)&addr, sizeof( addr ) ) == -1 )
	{
		printf("서버에 접속할수 없습니다.\n");
		closesocket( s );
		return ;
	}
	printf("서버에 접속되었습니다.\n");

	// 서버에 HTTP 요청 패킷을 보낸다.
	char buf[256] = "GET / HTTP/1.0\r\n\r\n";  // \r\n\r\n 이 빠졌서 그랬습니다.
												// 이렇게 보내 보세요.

	send( s, buf, strlen(buf)+1, 0);

	// 서버에서 오는 data 를 수신한다.
	char data[8192] = { 0 };
	recv( s, data, 8192, 0);

	printf("%s\n", data );

	closesocket( s );
	//--------------------------------------------
	WSACleanup();
}












