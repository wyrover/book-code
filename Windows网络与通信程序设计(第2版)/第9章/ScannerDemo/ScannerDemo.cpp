////////////////////////////////////////////////
// ScannerDemo.cpp文件

#include "../common/initsock.h"

#include <windows.h>
#include <stdio.h>
#include "ntddndis.h"
#include "protoutils.h"
#include "ProtoPacket.h"

#include "Iphlpapi.h"
#pragma comment(lib, "Iphlpapi.lib")

#include "../common/comm.h"


DWORD WINAPI SendThread(LPVOID lpParam);
BOOL GetGlobalData();

/////////////////////////////////////////
// 全局数据
u_char	g_ucLocalMac[6];	// 本地MAC地址
DWORD	g_dwGatewayIP;		// 网关IP地址
DWORD	g_dwLocalIP;		// 本地IP地址
DWORD	g_dwMask;			// 子网掩码

CInitSock theSock;




 /* 
计算TCP伪头校验和。TCP校验和基于如下几个域：
	源IP地址
	目的IP地址
	8位0域
	8位协议域
	16位TCP长度
	TCP头
	TCP数据
 */


void ComputeTcpPseudoHeaderChecksum(
    IPHeader    *pIphdr,
    TCPHeader *pTcphdr,
    char    *payload,
    int      payloadlen
    )
{
	char buff[1024];
	char *ptr = buff;
	int chksumlen = 0;
	ULONG zero = 0;
	
		// 伪头
	// 包含源IP地址和目的IP地址
	memcpy(ptr, &pIphdr->ipSource, sizeof(pIphdr->ipSource));
	ptr += sizeof(pIphdr->ipSource);
	chksumlen += sizeof(pIphdr->ipSource);

	memcpy(ptr, &pIphdr->ipDestination, sizeof(pIphdr->ipDestination));
	ptr += sizeof(pIphdr->ipDestination);
	chksumlen += sizeof(pIphdr->ipDestination);

	// 包含8位0域
	memcpy(ptr, &zero, 1);
	ptr += 1;
	chksumlen += 1;

	// 协议
	memcpy(ptr, &pIphdr->ipProtocol, sizeof(pIphdr->ipProtocol));
	ptr += sizeof(pIphdr->ipProtocol);
	chksumlen += sizeof(pIphdr->ipProtocol);

	// TCP长度
	USHORT tcp_len = htons(sizeof(TCPHeader) + payloadlen);
	memcpy(ptr, &tcp_len, sizeof(tcp_len));
	ptr += sizeof(tcp_len);
	chksumlen += sizeof(tcp_len);

		// TCP头
	memcpy(ptr, pTcphdr, sizeof(TCPHeader));
	ptr += sizeof(TCPHeader);
	chksumlen += sizeof(TCPHeader);

		// 净荷
	memcpy(ptr, payload, payloadlen);
	ptr += payloadlen;
	chksumlen += payloadlen;

	// 补齐到下一个16位边界
	for(int i=0; i<payloadlen%2; i++)
	{
		*ptr = 0;
		ptr++;
		chksumlen++;
	}

	// 计算这个校验和，将结果填充到TCP头
	pTcphdr->checksum = checksum((USHORT*)buff, chksumlen);
}

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
	if(!adapter.OpenAdapter(adapters.m_pwszSymbolicLink[0], FALSE))
	{
		printf(" OpenAdapter failed \n");
		ProtoStopService();
		return -1;
	}
	
	adapter.SetFilter(	//  NDIS_PACKET_TYPE_PROMISCUOUS|
		NDIS_PACKET_TYPE_DIRECTED | 
			NDIS_PACKET_TYPE_MULTICAST | NDIS_PACKET_TYPE_BROADCAST);


	// 目的IP地址和要探测的端口号
	char szDestIP[] = "219.238.168.74";  
	USHORT usDestPort = 80;

	DWORD dwLocalIP = g_dwLocalIP;  // 这里您可以使用假的IP地址和MAC地址
	u_char *pLocalMac = g_ucLocalMac;


	// 得到网关的MAC地址
	u_char arGatewayMac[6] = { 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
	ULONG ulLen = 6;
	if(!::SendARP(g_dwGatewayIP, 0, (ULONG*)arGatewayMac, &ulLen) == NO_ERROR)
	{
		printf(" 取得网关的MAC地址出错 \n");
		return -1;
	}

	DWORD dwDestIP = ::inet_addr(szDestIP);

		// 构建TCP数据贞
	char frame[500] = { 0 };
	// 以太头
	ETHeader etHeader;
	memcpy(etHeader.dhost, arGatewayMac, 6);
	memcpy(etHeader.shost, pLocalMac, 6);
	etHeader.type = ::htons(ETHERTYPE_IP);
	memcpy(frame, &etHeader, sizeof(etHeader));

	// IP头
	IPHeader ipHeader = { 0 };
	ipHeader.iphVerLen = (4<<4 | (sizeof(ipHeader)/sizeof(ULONG)));

	ipHeader.ipLength = ::htons(sizeof(IPHeader) + sizeof(TCPHeader));

	ipHeader.ipID = 1;
	ipHeader.ipFlags = 0;
	ipHeader.ipTTL = 128;
	ipHeader.ipProtocol = IPPROTO_TCP;
	ipHeader.ipSource = dwLocalIP;
	ipHeader.ipDestination = dwDestIP;	
	ipHeader.ipChecksum = 0;
	ipHeader.ipChecksum = checksum((USHORT*)&ipHeader, sizeof(ipHeader));

	memcpy(&frame[sizeof(etHeader)], &ipHeader, sizeof(ipHeader));

	// TCP头
	TCPHeader tcpHeader = { 0 };
	tcpHeader.sourcePort = htons(6000);
	tcpHeader.destinationPort = htons(0);
	tcpHeader.sequenceNumber = htonl(55551);
	tcpHeader.acknowledgeNumber = 0;
	tcpHeader.dataoffset =  (sizeof(tcpHeader)/4<<4|0); 

	tcpHeader.flags = TCP_SYN;   // #define   TCP_SYN   0x02
	tcpHeader.urgentPointer = 0;

	tcpHeader.windows = htons(512);
	tcpHeader.checksum = 0;


	//	下面是探测代码。注意，要实现扫描话，在这里循环探测端口号即可
	{
		// 构建封包
		tcpHeader.destinationPort = htons(usDestPort);
		ComputeTcpPseudoHeaderChecksum(&ipHeader, &tcpHeader, NULL, 0);
		memcpy(&frame[sizeof(etHeader) + sizeof(ipHeader)], &tcpHeader, sizeof(tcpHeader));

		printf(" 开始探测【%s:%d】... \n\n",  szDestIP, usDestPort);

		// 发送封包
		int nLen = sizeof(etHeader) + sizeof(ipHeader) + sizeof(tcpHeader);
		if(adapter.SendData(frame, nLen) != nLen)
		{
			printf(" SendData failed \n");
			return 0;
		}

		// 接收封包
		char buff[500] = { 0 };
		for(int i=0; i<5; i++)  // 注意，您应该使用异步方式接收数据。这里使用循环是为了方便
		{
			adapter.RecieveData(buff, nLen);
			ETHeader *pEtherhdr = (ETHeader *)buff;
			if(pEtherhdr->type == ::htons(ETHERTYPE_IP))
			{
				IPHeader *pIphdr = (IPHeader *)&buff[sizeof(ETHeader)];
				if(pIphdr->ipProtocol == IPPROTO_TCP && pIphdr->ipSource == dwDestIP)
				{
					TCPHeader *pTcphdr = (TCPHeader *)&buff[sizeof(ETHeader) + sizeof(IPHeader)];
					
					if((pTcphdr->flags & TCP_SYN) && (pTcphdr->flags & TCP_ACK)) // #define   TCP_ACK   0x10
					{
						printf(" 【%s:%d】Open \n", szDestIP, usDestPort);
					}
					else
					{
						printf(" 【%s:%d】Closed \n", szDestIP, usDestPort);
					}
					break;
				}
			}
		}
	}

	ProtoStopService();

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



