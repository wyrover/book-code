//////////////////////////////////////////////////////
// select.cpp文件
//select模型:好处是程序能够在单个线程内同时处理多个套接字链接

#include "../common/initsock.h"
#include <stdio.h>

CInitSock theSock;		// 初始化Winsock库
int main()
{
	USHORT nPort = 4567;	// 此服务器监听的端口号

	// 创建监听套节字
	SOCKET sListen = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);	
	sockaddr_in sin;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(nPort);
	sin.sin_addr.S_un.S_addr = INADDR_ANY;
	// 绑定套节字到本地机器
	if(::bind(sListen, (sockaddr*)&sin, sizeof(sin)) == SOCKET_ERROR)
	{
		printf(" Failed bind() \n");
		return -1;
	}
	// 进入监听模式
	::listen(sListen, 5);

		// select模型处理过程
	// 1）初始化一个套节字集合fdSocket，添加监听套节字句柄到这个集合
	fd_set fdSocket;		// 所有可用套节字集合
	FD_ZERO(&fdSocket);		//初始化为空
	FD_SET(sListen, &fdSocket);		//添加套接字到集合
	while(TRUE)
	{
		// 2）将fdSocket集合的一个拷贝fdRead传递给select函数，
		// 当有事件发生时，select函数移除fdRead集合中没有未决I/O操作的套节字句柄，然后返回。
		fd_set fdRead = fdSocket;
		int nRet = ::select(0, &fdRead, NULL, NULL, NULL);	//
		if(nRet > 0)
		{
			// 3）通过将原来fdSocket集合与select处理过的fdRead集合比较，
			// 确定都有哪些套节字有未决I/O，并进一步处理这些I/O。
			for(int i=0; i<(int)fdSocket.fd_count; i++)
			{
				if(FD_ISSET(fdSocket.fd_array[i], &fdRead))	//检查fdSocket.fd_array[i]是不是fdRead成员
				{
					if(fdSocket.fd_array[i] == sListen)		// （1）监听套节字接收到新连接
					{
						if(fdSocket.fd_count < FD_SETSIZE)
						{
							sockaddr_in addrRemote;
							int nAddrLen = sizeof(addrRemote);
							SOCKET sNew = ::accept(sListen, (SOCKADDR*)&addrRemote, &nAddrLen);
							FD_SET(sNew, &fdSocket);
							printf("接收到连接（%s）\n", ::inet_ntoa(addrRemote.sin_addr));
						}
						else
						{
							printf(" Too much connections! \n");
							continue;
						}
					}
					else
					{
						char szText[256];
						int nRecv = ::recv(fdSocket.fd_array[i], szText, strlen(szText), 0);
						if(nRecv > 0)						// （2）可读
						{
							szText[nRecv] = '\0';
							printf("接收到数据：%s \n", szText);
						}
						else								// （3）连接关闭、重启或者中断
						{
							::closesocket(fdSocket.fd_array[i]);
							FD_CLR(fdSocket.fd_array[i], &fdSocket);
						}
					}
				}
			}
		}
		else
		{
			printf(" Failed select() \n");
			break;
		}
	}
	return 0;
}