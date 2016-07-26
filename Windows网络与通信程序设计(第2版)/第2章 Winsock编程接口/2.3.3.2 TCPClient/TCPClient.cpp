//////////////////////////////////////////////////////////
// TCPClient.cpp文件


#include "../common/InitSock.h"
#include <stdio.h>
CInitSock initSock;		// 初始化Winsock库

int main()
{
	// 创建套节字
	SOCKET s = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(s == INVALID_SOCKET)
	{
		printf(" Failed socket() \n");
		return 0;
	}
	
	// 也可以在这里调用bind函数绑定一个本地地址
	// 否则系统将会自动安排
	
	// 填写远程地址信息
	sockaddr_in servAddr; 
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(4567);
	// 注意，这里要填写服务器程序（TCPServer程序）所在机器的IP地址
	// 如果你的计算机没有联网，直接使用127.0.0.1即可
	//127.0.0.1是回送地址，指本地机，一般用来测试使用。
	//回送地址（127.x.x.x）是本机回送地址（Loopback Address），
	//即主机IP堆栈内部的IP地址，主要用于网络软件测试以及本地机进程间通信，
	//无论什么程序，一旦使用回送地址发送数据，协议软件立即返回，不进行任何网络传输。

	servAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	//请求与服务器链接
	if(::connect(s, (sockaddr*)&servAddr, sizeof(servAddr)) == -1)
	{
		printf(" Failed connect() \n");
		return 0;
	}
	
	// 接收数据
	char buff[256];
	int nRecv = ::recv(s, buff, 256, 0);	//从对方接收数据
	if(nRecv > 0)
	{
		buff[nRecv] = '\0';
		printf(" 接收到数据：%s", buff);
	}
	
	// 关闭套节字
	::closesocket(s);
	return 0;
}
