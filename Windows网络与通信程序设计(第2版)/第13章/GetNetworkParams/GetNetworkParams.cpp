//////////////////////////////////////////////////////////////////////
// GetNetworkParams.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>

#pragma comment(lib, "Iphlpapi.lib")


int main()
{
	FIXED_INFO fi;
	ULONG ulOutBufLen = sizeof(fi);

	// 获取本地电脑的网络参数
	if(::GetNetworkParams(&fi, &ulOutBufLen) != ERROR_SUCCESS)
	{
		printf(" GetNetworkParams() failed \n");
		return -1;
	}

	// 主机名称
	printf(" Host Name: %s \n", fi.HostName);

	// 电脑注册的域名
	printf(" Domain Name: %s \n", fi.DomainName);

	// 打印出所有的DNS服务器
	printf(" DNS Servers: \n");
	printf(" \t%s \n", fi.DnsServerList.IpAddress.String);
	IP_ADDR_STRING *pIPAddr = fi.DnsServerList.Next;
	while(pIPAddr != NULL)
	{
		printf(" \t%s \n", pIPAddr->IpAddress.String);
		pIPAddr = pIPAddr->Next;
	}

	return 0;
}

