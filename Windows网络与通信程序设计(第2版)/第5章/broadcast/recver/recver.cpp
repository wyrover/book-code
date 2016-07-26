/////////////////////////////////////////////////////
// recver.cpp文件

#include "initsock.h"
#include "stdio.h"
#include <windows.h>

CInitSock theSock;

void main()
{
	SOCKET s = ::socket(AF_INET, SOCK_DGRAM, 0);

	// 首先要绑定一个本地地址，指明广播端口号
	SOCKADDR_IN sin;
	sin.sin_family = AF_INET;
	sin.sin_addr.S_un.S_addr = INADDR_ANY;	
	sin.sin_port = ::ntohs(4567);
	if(::bind(s, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" bind() failed \n");
		return;
	}

	// 接收广播
	printf(" 开始在4567端口接收广播数据... \n\n");
	SOCKADDR_IN addrRemote;	
	int nLen = sizeof(addrRemote);
	char sz[256];
	while(TRUE)
	{
		int nRet = ::recvfrom(s, sz, 256, 0, (sockaddr*)&addrRemote, &nLen);
		if(nRet > 0)
		{
			sz[nRet] = '\0';
			printf(sz);
		}
	}
}



