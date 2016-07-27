#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>	
#pragma comment(lib, "ws2_32.lib")

#include <vector> // STL 의vector
using namespace std;

vector<SOCKET> client_list;


DWORD WINAPI foo( void* p)
{
	SOCKET s = (SOCKET)p;

	char buf[1024] = {0};
	int  nRead = 0;

	while ( 1 )
	{
		memset( buf, 0,1024);

		int nRead = recv( s, buf, 1024, 0);
		if ( nRead <= 0) break;
		// 받은 data를 list 에 있는 모든 client에게 보낸다.
		for ( int i = 0; i < client_list.size(); ++i )
		{
			if( client_list[i] != s)
				send( client_list[i], buf, nRead, 0);
		}	
	}


















	// 접속이 끊어 진 경우 list 에서 제거한다.
	for( int i = 0; i < client_list.size(); ++i )
	{
		if ( client_list[i] == s ) 
		{
			vector<SOCKET>::iterator p = client_list.begin();
			p = p + i;
			client_list.erase( p );
			break;
		}
	}



	closesocket(s );
	return 0;
}


void main()
{
	WSADATA wsadata;

	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		printf("Can't Initialize Socket !\n");
		return ;
	}
	//--------------------------------------------
	// 1. socket 생성 : TCP : SOCK_STREAM     UDP : SOCK_DGRAM
	SOCKET s = socket( AF_INET, SOCK_STREAM, 0);

	// 2. 생성된 소켓에 주소를 지정
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET;		 // 주소 종류(AF_INET : IP 주소라는 의미)
	addr.sin_port   = htons( 5252 ); // Port #
	addr.sin_addr.s_addr = INADDR_ANY;	// 자신의 모든 IP 사용 

	if ( bind( s, (SOCKADDR*)&addr, sizeof(addr)) == -1 )
	{
		printf("Can't bind \n"); return;
	}




	// 3. 소켓을 대기 상태로 전환한다.
	if ( listen( s, 5) == -1 )
	{
		printf("Can't Listen\n"); return;
	}






	printf("클라이언트를 대기합니다. \n");

	// 4. 이제 Client 에서 부터 전달된 요청을 허용한다.
	while ( 1 )
	{
		SOCKADDR_IN c_addr;
		int  size = sizeof(c_addr);
		
		SOCKET c_s = accept( s, (SOCKADDR*)&c_addr, &size );



















		printf("클라이언트 가 접속했습니다. IP : %s\n", 
												inet_ntoa( c_addr.sin_addr));

		// linked list 에 socket을 넣는다.
		client_list.push_back( c_s );




















		// 새로운 스레드를 생성해서 클라이 언트의 요청을 처리한다.
		HANDLE hThread = CreateThread( 0, 0, foo, (void*)c_s, 0, 0);
		CloseHandle( hThread );
	}


















	//--------------------------------------------
	closesocket( s ); // 소켓 닫기.
	WSACleanup();
}









