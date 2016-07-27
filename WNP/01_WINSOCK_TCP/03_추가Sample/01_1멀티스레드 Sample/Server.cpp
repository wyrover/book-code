//-----------------------------------------------------------------
// Server
//   - 1:1 통신 서버 
//   - 자유로운 송수신 가능 
//-----------------------------------------------------------------

#include <iostream>
#include <winsock2.h>
#include <cstring>
#include <process.h>
#include <list>
using namespace std;



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


void sendthread(void* p);


void main()
{

	//================================================================

	int			retval;
	WSADATA		wsa;
	WSAStartup(MAKEWORD(2,2), &wsa);

	SOCKET	server_sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN	addr_server;
	addr_server.sin_family			= AF_INET;
	addr_server.sin_port			= htons(4000);
	addr_server.sin_addr.s_addr		= 0;

	int saddrlen		= sizeof(addr_server);

	retval = bind(server_sock, (SOCKADDR*)&addr_server, saddrlen); 
	if( retval == SOCKET_ERROR)
		err_quit("bind");

	retval = listen(server_sock, 5);
	if( retval == SOCKET_ERROR)
		err_quit("listen");

	//================================================================

	// 접속된 클라이언트의 주소를 저장할 구조체 
	SOCKADDR_IN		 addr_client;
	int len			= sizeof(addr_client);
	SOCKET			client_sock;
	while(1)
	{
		cout << "Wating Client....." << endl;
		client_sock = accept(server_sock, (SOCKADDR*)&addr_client, &len);
		cout << "connected clented...." << endl;


		_beginthread(sendthread, 0, (void*)client_sock);
		char buf[256];
		while(1)
		{
			int n = recv(client_sock, buf, 256, 0);
			if (n >0)
			{
				buf[n] = '\0';
				cout << buf << endl;
			}	
		}
	}
	WSACleanup();

}


void sendthread(void* p)
{
	SOCKET sock = (SOCKET)p;

	char buf[256];
	while(1)
	{
		cin >> buf;
		send(sock, buf, strlen(buf), 0);
	}	
}