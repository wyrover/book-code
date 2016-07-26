////////////////////////////////////////////
// Sniffer.cpp文件

#include "../common/initsock.h"
#include "../common/protoinfo.h" 

#include <stdio.h>
#include <mstcpip.h>

#pragma comment(lib, "Advapi32.lib")

CInitSock theSock;

void DecodeTCPPacket(char *pData)
{
	TCPHeader *pTCPHdr = (TCPHeader *)pData;

	printf(" Port: %d -> %d \n", ntohs(pTCPHdr->sourcePort), ntohs(pTCPHdr->destinationPort));
	
	// 下面还可以根据目的端口号进一步解析应用层协议
	switch(::ntohs(pTCPHdr->destinationPort))
	{
	case 21:
		break;
	case 80:
	case 8080:
		break;
	}
}

void DecodeIPPacket(char *pData)
{
	IPHeader *pIPHdr = (IPHeader*)pData;	
	in_addr source, dest;
	char szSourceIp[32], szDestIp[32]; 

	printf("\n\n-------------------------------\n");

	// 从IP头中取出源IP地址和目的IP地址
	source.S_un.S_addr = pIPHdr->ipSource;
	dest.S_un.S_addr = pIPHdr->ipDestination;
	strcpy(szSourceIp, ::inet_ntoa(source));
	strcpy(szDestIp, ::inet_ntoa(dest));

	printf("	%s -> %s \n", szSourceIp, szDestIp);
	// IP头长度
	int nHeaderLen = (pIPHdr->iphVerLen & 0xf) * sizeof(ULONG);

	switch(pIPHdr->ipProtocol)
	{
	case IPPROTO_TCP: // TCP协议
		DecodeTCPPacket(pData + nHeaderLen);
		break;
	case IPPROTO_UDP:
		break;
	case IPPROTO_ICMP:
		break; 
	}
}


void main()
{
	// 创建原始套节字
	SOCKET sRaw = socket(AF_INET, SOCK_RAW, IPPROTO_IP);

	// 获取本地IP地址
	char szHostName[56];
	SOCKADDR_IN addr_in;
	struct  hostent *pHost;
	gethostname(szHostName, 56);
	if((pHost = gethostbyname((char*)szHostName)) == NULL)	
		return ;

	// 在调用ioctl之前，套节字必须绑定
	addr_in.sin_family  = AF_INET;
	addr_in.sin_port    = htons(0);
	memcpy(&addr_in.sin_addr.S_un.S_addr, pHost->h_addr_list[0], pHost->h_length);

	printf(" Binding to interface : %s \n", ::inet_ntoa(addr_in.sin_addr));
	if(bind(sRaw, (PSOCKADDR)&addr_in, sizeof(addr_in)) == SOCKET_ERROR)
		return;

	// 设置SIO_RCVALL控制代码，以便接收所有的IP包	
	DWORD dwValue = 1;
	if(ioctlsocket(sRaw, SIO_RCVALL, &dwValue) != 0)	
		return ;
	
	// 开始接收封包
	char buff[1024];
	int nRet;
	while(TRUE)
	{
		nRet = recv(sRaw, buff, 1024, 0);
		if(nRet > 0)
		{
			DecodeIPPacket(buff);
		}
	}
	closesocket(sRaw);
}

