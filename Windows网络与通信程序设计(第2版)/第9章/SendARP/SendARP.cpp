//////////////////////////////////////////////////////
// SendARP.cpp文件

#include <windows.h>
#include <stdio.h>

#include <Iphlpapi.h>
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")


void main()
{
	char szDestIP[] = "192.168.0.23";

	// 发送ARP请求
	u_char arDestMac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	ULONG ulLen = 6;
	if(::SendARP(::inet_addr(szDestIP), 0, (ULONG*)arDestMac, &ulLen) == NO_ERROR)
	{
		// 打印出结果
		u_char *p = arDestMac;
		printf("   pEtherh->shost: %02X-%02X-%02X-%02X-%02X-%02X \n", p[0], p[1], p[2], p[3], p[4], p[5]);
	}
}











