//sever.c
#define DLL_SOURCE
#include <winsock2.h>	
#pragma comment(lib, "ws2_32.lib")
#include <process.h>
//#include <stdlib.h>	
#include <stdio.h>
#include "sever.h"
SOCKET recv_sock;
SOCKADDR_IN		peeraddr;
int				addrlen;

RecvFun myrefun;

void sock_init(RecvFun rfun)
{
	myrefun = rfun;
}

int sever_init()
{
	int retval;
	// 윈속 초기화
	WSADATA wsa;
	if(WSAStartup(MAKEWORD(2,2), &wsa) != 0)		
		return -1;

	// socket()
	recv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	//if(recv_sock == INVALID_SOCKET) err_quit("socket()");


	// bind()
	SOCKADDR_IN			localaddr;
	ZeroMemory(&localaddr, sizeof(localaddr));
	localaddr.sin_family		= AF_INET;
	localaddr.sin_port			= htons(5252);
	localaddr.sin_addr.s_addr   = htonl(INADDR_ANY);
	retval		= bind(recv_sock, (SOCKADDR*)&localaddr, 
							sizeof(localaddr));


	
	//MessageBox(NULL,"","",MB_OK);
	// 멀티캐스트 데이터 받기
	//logoff();
	return 1;
}


int sever_exit()
{
	// closesocket()
	closesocket(recv_sock);

	// 윈속 종료
	WSACleanup();
	return 1;
}

int server_run()
{
	while(1)
	{
		
		// 데이터 받기
		char *i;
		i = (char*)malloc(94);
		memset( i, 0, 94);
		addrlen		= sizeof(peeraddr);
		recvfrom(recv_sock, (char*)i, 94, 0, 
			(SOCKADDR*)&peeraddr, &addrlen);
		
		myrefun(i,94,recv_sock);
		free(i);
	}
	return 1;
}
