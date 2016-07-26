//////////////////////////////////////////////////////////
// UDPServer.cpp文件


#include "../common/InitSock.h"
#include <stdio.h>
CInitSock initSock;		// 初始化Winsock库

int main()
{
	// 创建套节字
	SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == INVALID_SOCKET)
	{
		printf("Failed socket() \n");
		return 0;
	}
	
	// 填充sockaddr_in结构
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(4567);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	
	// 绑定这个套节字到一个本地地址
	if(::bind(s, (LPSOCKADDR)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf("Failed bind() \n");
		return 0;
	}

	// 接收数据
	char buff[1024];
	sockaddr_in addr;
	int nLen = sizeof(addr);
	while(TRUE)
	{
		int nRecv = ::recvfrom(s, buff, 1024, 0, (sockaddr*)&addr, &nLen); 
		if(nRecv > 0)
		{
			buff[nRecv] = '\0';
			printf(" 接收到数据（%s）：%s", ::inet_ntoa(addr.sin_addr), buff);
		}
	}
	::closesocket(s);
}