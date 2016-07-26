////////////////////////////////////////////////
// EnumeHosts.cpp文件

#include "../common/initsock.h"

#include <windows.h>
#include <stdio.h>
#include <Iphlpapi.h>

#include "protoutils.h"
#include "ProtoPacket.h"


#pragma comment(lib, "Iphlpapi.lib")


DWORD WINAPI SendThread(LPVOID lpParam);
BOOL GetGlobalData();

/////////////////////////////////////////
// 全局数据
u_char	g_ucLocalMac[6];	// 本地MAC地址
DWORD	g_dwGatewayIP;		// 网关IP地址
DWORD	g_dwLocalIP;		// 本地IP地址
DWORD	g_dwMask;			// 子网掩码

CInitSock theSock;

int main()
{
	// 获取全局数据
	GetGlobalData();
	// 启动服务
	if(!ProtoStartService())
	{
		printf(" ProtoStartService() failed %d \n", ::GetLastError());
		return -1;
	}
	// 打开控制设备对象
	HANDLE hControlDevice = ProtoOpenControlDevice();
	if(hControlDevice == INVALID_HANDLE_VALUE)
	{
		printf(" ProtoOpenControlDevice() failed() %d \n", ::GetLastError());
		ProtoStopService();
		return -1;
	}
	// 枚举绑定的下层适配器
	CPROTOAdapters adapters;
	if(!adapters.EnumAdapters(hControlDevice))
	{
		printf(" Enume adapter failed \n");
		ProtoStopService();
		return -1;
	}

	CAdapter adapter;
	// 默认使用第一个适配器
	if(!adapter.OpenAdapter(adapters.m_pwszSymbolicLink[0], TRUE))
	{
		printf(" OpenAdapter failed \n");
		ProtoStopService();
		return -1;
	}

	CArpPacket arp(&adapter);

	////////////////////////////////////////////////////////////////////////
	// 下面开始进行LAN扫描

	// 提供当前线程（接收数据的线程）的优先级，避免丢失到来的数据贞
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);
	// 创建发送线程，开始发送ARP请求
	::CloseHandle(::CreateThread(NULL, 0, SendThread, &arp, 0, NULL));

	// 接收ARP应答
	u_char *p;
	PARPHeader pArph;
	printf(" 开始扫描LAN中的计算机... \n");
	while(TRUE)
	{
		pArph = arp.WaitReply();
		if(pArph != NULL)
		{

			printf(" \n ------------------------------------------- \n");

			p = pArph->smac;
			printf("	Mac地址:	%02X-%02X-%02X-%02X-%02X-%02X \n", p[0], p[1], p[2], p[3], p[4], p[5]);
			p = (u_char*)&pArph->saddr;
			printf("	 IP地址:	%d.%d.%d.%d  \n ", p[0], p[1], p[2], p[3]);
	//		HOSTENT *pHost = ::gethostbyaddr((char*)&pArph->saddr, 4, AF_INET);
	//		if(pHost != NULL)
	//			printf("	 主机名:	%s \n", pHost->h_name);
		}
		else
		{
			break;
		}
	}
	printf("\n\n 扫描完毕 \n");

	ProtoStopService();

	getchar();

	return 0;
}


DWORD WINAPI SendThread(LPVOID lpParam)
{
	CArpPacket *pArp = (CArpPacket *)lpParam;
	// 取得LAN中IP地址空间的大小
	DWORD dwMask = ::ntohl(g_dwMask);
	int nMaxHosts = ~dwMask;

	// 设置以太头中目标MAC地址。发送ARP请求时，应该将它设为广播地址
	u_char destmacEther[6];
	memset(destmacEther, 0xff, 6);
	// ARP头中的目标MAC地址
	u_char destmacArp[6] = { 0 }; 
	// 向地址空间中的每个IP地址发送ARP请求
	DWORD dwTemp = ::ntohl(g_dwGatewayIP & g_dwMask) + 1;
	for(int i=1; i<nMaxHosts; i++, dwTemp++)
	{
		DWORD dwIP = ::htonl(dwTemp);
		if(dwIP != g_dwLocalIP)
		{
			if(!pArp->SendPacket(destmacEther, g_ucLocalMac, ARPOP_REQUEST, 
										destmacArp, dwIP, g_ucLocalMac, g_dwLocalIP))
			{
				printf(" SendPacket() failed \n");
				break;
			}
		}
	}
	return 0;
}

BOOL GetGlobalData()
{
	PIP_ADAPTER_INFO pAdapterInfo = NULL;
	ULONG ulLen = 0;

	// 为适配器结构申请内存
	::GetAdaptersInfo(pAdapterInfo,&ulLen);
	pAdapterInfo = (PIP_ADAPTER_INFO)::GlobalAlloc(GPTR, ulLen);

	// 取得本地适配器结构信息
	if(::GetAdaptersInfo(pAdapterInfo,&ulLen) ==  ERROR_SUCCESS)
	{
		if(pAdapterInfo != NULL)
		{
			memcpy(g_ucLocalMac, pAdapterInfo->Address, 6);
			g_dwGatewayIP = ::inet_addr(pAdapterInfo->GatewayList.IpAddress.String);
			g_dwLocalIP = ::inet_addr(pAdapterInfo->IpAddressList.IpAddress.String);
			g_dwMask = ::inet_addr(pAdapterInfo->IpAddressList.IpMask.String);
		}
	}	
	::GlobalFree(pAdapterInfo);

	printf(" \n -------------------- 本地主机信息 -----------------------\n\n");
	in_addr in;
	in.S_un.S_addr = g_dwLocalIP;
	printf("      IP Address : %s \n", ::inet_ntoa(in));

	in.S_un.S_addr = g_dwMask;
	printf("     Subnet Mask : %s \n", ::inet_ntoa(in));

	in.S_un.S_addr = g_dwGatewayIP;
	printf(" Default Gateway : %s \n", ::inet_ntoa(in));

	u_char *p = g_ucLocalMac;
	printf("     MAC Address : %02X-%02X-%02X-%02X-%02X-%02X \n", p[0], p[1], p[2], p[3], p[4], p[5]);

	printf(" \n \n ");



	return TRUE;
}
