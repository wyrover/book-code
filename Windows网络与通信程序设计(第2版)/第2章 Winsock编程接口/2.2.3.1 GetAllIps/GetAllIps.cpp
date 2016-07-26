//////////////////////////////////////////////////
// GetAllIps.cpp文件


#include "../common/InitSock.h"


#include <stdio.h>
CInitSock initSock;		// 初始化Winsock库

void main()
{
	char szHost[256];	
	::gethostname(szHost, 256);// 取得本地主机名称	
	hostent *pHost = ::gethostbyname(szHost);// 通过主机名得到地址信息
	// 打印出所有IP地址
	in_addr addr;	//存储IP地址
	for(int i = 0; ; i++)
	{
		char *p = pHost->h_addr_list[i];
		if(p == NULL)
			break;

		memcpy(&addr.S_un.S_addr, p, pHost->h_length);
		char *szIp = ::inet_ntoa(addr);	//将网络字节的32位IP地址转化为字符串
		printf(" 本机IP地址：%s  \n ", szIp);
	}
}