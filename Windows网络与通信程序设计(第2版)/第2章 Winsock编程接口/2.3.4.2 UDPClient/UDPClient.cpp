//////////////////////////////////////////////////////////
// UDPClient文件


#include "../common/InitSock.h"
#include <stdio.h>
CInitSock initSock;		// 初始化Winsock库

int main()
{
	// 创建套节字
	SOCKET s = ::socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(s == INVALID_SOCKET)
	{
		printf("Failed socket() %d \n", ::WSAGetLastError());
		return 0;
	}
	
	// 也可以在这里调用bind函数绑定一个本地地址
	// 否则系统将会自动安排
		
	// 填写远程地址信息
	sockaddr_in addr; 
	addr.sin_family = AF_INET;
	addr.sin_port = htons(4567);
	// 注意，这里要填写服务器程序所在机器的IP地址
	// 如果你的计算机没有联网，直接使用127.0.0.1即可
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");

	// 发送数据
	char szText[] = " TCP Server Demo! \r\n";
	::sendto(s, szText, strlen(szText), 0, (sockaddr*)&addr, sizeof(addr));

	::closesocket(s);
	return 0;
}