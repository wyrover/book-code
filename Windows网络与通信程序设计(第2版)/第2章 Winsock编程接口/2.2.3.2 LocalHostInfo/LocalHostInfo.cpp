////////////////////////////////////////////////
// LocalHostInfo.cpp文件

#include <windows.h>
#include <stdio.h>

#include "Iphlpapi.h"		//包含对IP帮助函数的定义(需单独下载或者更新SDK)
#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")

/////////////////////////////////////////
// 全局数据
u_char	g_ucLocalMac[6];	// 本地MAC地址
DWORD	g_dwGatewayIP;		// 网关IP地址
DWORD	g_dwLocalIP;		// 本地IP地址
DWORD	g_dwMask;			// 子网掩码

BOOL GetGlobalData()
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	ULONG ulLen = 0;

	// 为适配器结构申请内存
	::GetAdaptersInfo(pAdapterInfo,&ulLen);		//获取本地机器的适配器信息,调用成功返回ERROR_SUCCESS
	pAdapterInfo = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR, ulLen);

	// 取得本地适配器结构信息
	if(::GetAdaptersInfo(pAdapterInfo,&ulLen) ==  ERROR_SUCCESS)
	{
		if(pAdapterInfo != NULL)
		{
			memcpy(g_ucLocalMac, pAdapterInfo->Address, 6);
			g_dwGatewayIP = ::inet_addr(pAdapterInfo->GatewayList.IpAddress.String);	//
			g_dwLocalIP = ::inet_addr(pAdapterInfo->IpAddressList.IpAddress.String);
			g_dwMask = ::inet_addr(pAdapterInfo->IpAddressList.IpMask.String);
		}
	}

	printf(" \n -------------------- 本地主机信息 -----------------------\n\n");
	in_addr in;
	in.S_un.S_addr = g_dwLocalIP;
	printf("      IP Address : %s \n", ::inet_ntoa(in));	//inet_ntoa将32位二进制数转换为字符串

	in.S_un.S_addr = g_dwMask;
	printf("     Subnet Mask : %s \n", ::inet_ntoa(in));

	in.S_un.S_addr = g_dwGatewayIP;
	printf(" Default Gateway : %s \n", ::inet_ntoa(in));

	u_char *p = g_ucLocalMac;
	printf("     MAC Address : %02X-%02X-%02X-%02X-%02X-%02X \n", p[0], p[1], p[2], p[3], p[4], p[5]);

	printf(" \n \n ");

	return TRUE;
}

int main()
{
	// 获取全局数据
	GetGlobalData();

	getchar();		//键入回车后
	return 0;
}

// typedef struct _IP_ADAPTER_INFO {
//     struct _IP_ADAPTER_INFO* Next;
//     DWORD ComboIndex;
//     char AdapterName[MAX_ADAPTER_NAME_LENGTH + 4];
//     char Description[MAX_ADAPTER_DESCRIPTION_LENGTH + 4];
//     UINT AddressLength;
//     BYTE Address[MAX_ADAPTER_ADDRESS_LENGTH];
//     DWORD Index;
//     UINT Type;
//     UINT DhcpEnabled;
//     PIP_ADDR_STRING CurrentIpAddress;
//     IP_ADDR_STRING IpAddressList;
//     IP_ADDR_STRING GatewayList;
//     IP_ADDR_STRING DhcpServer;
//     BOOL HaveWins;
//     IP_ADDR_STRING PrimaryWinsServer;
//     IP_ADDR_STRING SecondaryWinsServer;
//     time_t LeaseObtained;
//     time_t LeaseExpires;
// } IP_ADAPTER_INFO, *PIP_ADAPTER_INFO;

// typedef struct _IP_ADDR_STRING {
//     struct _IP_ADDR_STRING* Next;
//     IP_ADDRESS_STRING IpAddress;
//     IP_MASK_STRING IpMask;
//     DWORD Context;
// } IP_ADDR_STRING, *PIP_ADDR_STRING;