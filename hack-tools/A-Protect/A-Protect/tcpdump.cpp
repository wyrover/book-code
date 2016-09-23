#include "stdafx.h"
#include "A-ProtectView.h"
#include "A-Protect.h"
#include "tcpdump.h"

void SaveStructInfo(PVOID lpszBuffer,CHAR *lpFilePath)
{
	HANDLE	hFile = CreateFileA(lpFilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != 0)
	{
		DWORD dwBytesWrite = 0;
		SetFilePointer(hFile, 0, 0, FILE_END);
		WriteFile(hFile,lpszBuffer,sizeof(TCP_SNIFFER_INFO)*1025, &dwBytesWrite, NULL);
		CloseHandle(hFile);
	}
}
void ReadStructInfo(PVOID lpszBuffer,CHAR *lpFilePath,CMyList *m_list)
{
	DWORD dwWritten; 
	PTCP_SNIFFER_INFO PTcpSniffer;

	HANDLE hFile = CreateFileA(lpFilePath, GENERIC_READ, FILE_SHARE_READ,NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != NULL)
	{
		ReadFile(hFile,lpszBuffer,sizeof(TCP_SNIFFER_INFO)*1025, &dwWritten, NULL);
		CloseHandle(hFile);
	}
}
void DoXOR(DWORD key, char *data, int len)
{
	while(len--)
		*(data++) ^= key;
}
//读取配置到界面
void ReadSnifferInfo(CMyList *m_list,char *lpData,char *lpDataDescription)
{
	PTCP_SNIFFER_INFO PTcpSniffer;
	int i=0;
	int x=0;

	//读取数据包监控的关键字到界面
	PTcpSniffer = (PTCP_SNIFFER_INFO)VirtualAlloc(0, sizeof(TCP_SNIFFER_INFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (PTcpSniffer)
	{
		char ModuleFile[260] = {0};
		char SaveFile[260] = {0};

		GetModuleFileNameA(NULL,ModuleFile,sizeof(ModuleFile));
		wsprintfA(SaveFile,"%s\\A-Protect.apt",ExtractFilePath(ModuleFile));
		ReadStructInfo(PTcpSniffer,SaveFile,m_list);

		if (lpData &&
			lpDataDescription){
				i = PTcpSniffer->ulCount;
				i++;
				strcat(PTcpSniffer->SnifferInfo[i].Data,lpData);
				//DoXOR(0x1999, PTcpSniffer->SnifferInfo[i].Data, 99);

				strcat(PTcpSniffer->SnifferInfo[i].DataDescription,lpDataDescription);
				//DoXOR(0x1999, PTcpSniffer->SnifferInfo[i].DataDescription, 99);

				//char lpBuff[260] = {0};
				//wsprintfA(lpBuff,"%s\n%s\n%d %d",PTcpSniffer->SnifferInfo[i].Data,PTcpSniffer->SnifferInfo[i].DataDescription,i,PTcpSniffer->ulCount);
				//MessageBoxA(0,lpBuff,0,0);

				PTcpSniffer->ulCount = i;  //重新赋值

				DeleteFileA(SaveFile);
				SaveStructInfo(PTcpSniffer,SaveFile);
		}
		if (m_list)
		{
			for (x=0;x<PTcpSniffer->ulCount+1;x++)
			{
				WCHAR Number[50] = {0};
				WCHAR Data[260] = {0};
				WCHAR DataDescription[260] = {0};

				memset(Number,0,sizeof(Number));
				memset(Data,0,sizeof(Data));
				memset(DataDescription,0,sizeof(DataDescription));

				wsprintfW(Number,L"%d",x);
				//DoXOR(0x1999, PTcpSniffer->SnifferInfo[x].Data, 99);
				//DoXOR(0x1999, PTcpSniffer->SnifferInfo[x].DataDescription, 99);

				//char lpBuff[260] = {0};
				//wsprintfA(lpBuff,"%s\n\n%d",PTcpSniffer->SnifferInfo[x].Data,x);
				//MessageBoxA(0,lpBuff,0,0);

				MultiByteToWideChar(
					CP_ACP,
					0, 
					PTcpSniffer->SnifferInfo[x].Data,
					-1, 
					Data, 
					strlen(PTcpSniffer->SnifferInfo[x].Data)
					);
				MultiByteToWideChar(
					CP_ACP,
					0, 
					PTcpSniffer->SnifferInfo[x].DataDescription,
					-1, 
					DataDescription, 
					strlen(PTcpSniffer->SnifferInfo[x].DataDescription)
					);
				m_list->InsertItem(i,Number,RGB(77,77,77));
				m_list->SetItemText(i,1,Data);
				m_list->SetItemText(i,2,DataDescription);

			}

		}
		VirtualFree(PTcpSniffer,sizeof(TCP_SNIFFER_INFO)*1025,MEM_RESERVE | MEM_COMMIT);
	}
}
//初始化的时候，如果记录文件不存在，则写入
void WriteSnifferInfo()
{
	PTCP_SNIFFER_INFO PTcpSniffer;
	CHAR ModuleFile[260] = {0};
	CHAR SaveFile[260] = {0};

	PTcpSniffer = (PTCP_SNIFFER_INFO)VirtualAlloc(0, sizeof(TCP_SNIFFER_INFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (PTcpSniffer)
	{
		strcat(PTcpSniffer->SnifferInfo[0].Data,"Microsoft Corp");   //cmd的特征
		//DoXOR(0x1999, PTcpSniffer->SnifferInfo[0].Data, 99);

		strcat(PTcpSniffer->SnifferInfo[0].DataDescription,"可能通过远程木马对您的计算机执行命令");
		//DoXOR(0x1999, PTcpSniffer->SnifferInfo[0].DataDescription, 99);

		strcat(PTcpSniffer->SnifferInfo[1].Data,"命令");          //cmd命令的特征
		//DoXOR(0x1999, PTcpSniffer->SnifferInfo[1].Data, 99);

		strcat(PTcpSniffer->SnifferInfo[1].DataDescription,"可能通过远程木马对您的计算机执行命令");
		//DoXOR(0x1999, PTcpSniffer->SnifferInfo[1].DataDescription, 99);

		strcat(PTcpSniffer->SnifferInfo[2].Data,"Gh0st");
		strcat(PTcpSniffer->SnifferInfo[2].DataDescription,"可能是Gh0st木马数据包");

		PTcpSniffer->ulCount = 3;

		GetModuleFileNameA(NULL,ModuleFile,sizeof(ModuleFile));
		wsprintfA(SaveFile,"%s\\A-Protect.apt",ExtractFilePath(ModuleFile));
		if (GetFileAttributesA(SaveFile) == -1)
		{
			SaveStructInfo(PTcpSniffer,SaveFile);
		}
	}
	return;
}
BOOL WINAPI CtrlHandlerRoutine(DWORD dwCtrlType)
{
	bMainLoop = FALSE;
	
	return TRUE;
}

void packet_init(packet *p, PBYTE buf, int len)
{
	memset(p, 0, sizeof(packet));
	p->plen = len;

	p->eh = (struct ethhdr *)buf;
	if (p->eh->h_proto == __constant_ntohs(ETH_P_IP))
	{
		p->ih = (struct iphdr *)(p->eh + 1);
		switch (p->ih->protocol)
		{
		case IPPROTO_TCP:
			p->th = (struct tcphdr *)((PBYTE)p->ih + p->ih->ihl * 4);
			p->data = (PBYTE)p->th + p->th->doff * 4;
			p->dlen = len - (p->data - buf);
			break;
		case IPPROTO_UDP:
			p->uh = (struct udphdr *)((PBYTE)p->ih + p->ih->ihl * 4);
			p->data = (PBYTE)(p->uh + 1);
			p->dlen = len - (p->data - buf);
			break;
		case IPPROTO_ICMP:
			p->icmph = (struct icmphdr *)((PBYTE)p->ih + p->ih->ihl * 4);
			p->data = (PBYTE)(p->icmph + 1);
			p->dlen = len - (p->data - buf);
			break;

		default:
			p->data = (PBYTE)((PBYTE)p->ih + p->ih->ihl * 4);
			p->dlen = len - (p->data - buf);
			break;
		}
	}
	else if (p->eh->h_proto == __constant_ntohs(ETH_P_ARP))
	{
		p->ah = (struct arphdr *)(p->eh + 1);
		p->data = (PBYTE)(p->eh + 1);
		p->dlen = len - (p->data - buf);
	}
	else
	{
		p->data = (PBYTE)(p->eh + 1);
		p->dlen = len - (p->data - buf);
	}
}
int packet_filter(packet *p)
{
	if (h_proto && p->eh->h_proto != h_proto)
		return 0;

	if (p->eh->h_proto == __constant_ntohs(ETH_P_IP))
	{
		if (protocol && p->ih->protocol != protocol)
			return 0;
		if (saddr && daddr && (p->ih->saddr == saddr || p->ih->daddr == daddr))
			return 1;
		if (saddr && p->ih->saddr != saddr)
			return 0;
		if (daddr && p->ih->daddr != daddr)
			return 0;

		if (p->ih->protocol == IPPROTO_TCP)
		{
			if (sport && dport && (p->th->source == sport || p->th->dest == dport))
				return 1;
			if (sport && p->th->source != sport)
				return 0;
			if (dport && p->th->dest != dport)
				return 0;
		}
		if (p->ih->protocol == IPPROTO_UDP)
		{
			if (sport && dport && (p->uh->source == sport || p->uh->dest == dport))
				return 1;
			if (sport && p->uh->source != sport)
				return 0;
			if (dport && p->uh->dest != dport)
				return 0;
		}
	}
	else if (p->eh->h_proto == __constant_ntohs(ETH_P_ARP))
	{
		if (protocol || sport || dport)
			return 0;
		if (saddr && *(u_long*)p->ah->ar_sha != saddr)
			return 0;
		if (daddr && *(u_long*)p->ah->ar_tip != daddr)
			return 0;
	}

	return 1;
}
void SaveSnifferData(CHAR *lpszBuffer)
{
	CHAR FilePath[260] = {0};

	//默认路径 c:\windows\A-Protect-TcpSniffer.txt

	GetWindowsDirectoryA(FilePath,sizeof(FilePath));
	strcat(FilePath,"\\A-Protect-TcpSniffer.txt");

	HANDLE	hFile = CreateFileA(FilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	DWORD dwBytesWrite = 0;
	SetFilePointer(hFile, 0, 0, FILE_END);
	WriteFile(hFile,lpszBuffer,strlen(lpszBuffer), &dwBytesWrite, NULL);
	CloseHandle(hFile);
}
void packet_print(packet *p,CMyList *m_list)
{
	CHAR addr[260] = {0};
	CHAR DataBuff[1024] = {0};
	CHAR CmdBuffer[260] = {0};
	PTCP_SNIFFER_INFO PTcpSniffer;

	if (print_eh || (p->eh->h_proto != __constant_ntohs(ETH_P_IP) && p->eh->h_proto != __constant_ntohs(ETH_P_ARP)))
	{
		printf("%02X:%02X:%02X:%02X:%02X:%02X -> %02X:%02X:%02X:%02X:%02X:%02X %04X Len=%d\n",
			p->eh->h_source[0], p->eh->h_source[1], p->eh->h_source[2], 
			p->eh->h_source[3], p->eh->h_source[4], p->eh->h_source[5], 
			p->eh->h_dest[0], p->eh->h_dest[1], p->eh->h_dest[2], 
			p->eh->h_dest[3], p->eh->h_dest[4], p->eh->h_dest[5],
			__constant_ntohs(p->eh->h_proto), p->plen);
	}
	if (p->eh->h_proto == __constant_ntohs(ETH_P_IP))
	{
		switch (p->ih->protocol)
		{
		case IPPROTO_TCP:
			wsprintfA(addr,"%u.%u.%u.%u:%d > %u.%u.%u.%u:%d ",
				NIPQUAD(p->ih->saddr), __constant_ntohs(p->th->source),
				NIPQUAD(p->ih->daddr), __constant_ntohs(p->th->dest));

			wsprintfA(DataBuff,"\r\n[TCP]%s \r\n%s\r\n",addr,p->data);
			SaveSnifferData(DataBuff);

			PTcpSniffer = (PTCP_SNIFFER_INFO)VirtualAlloc(0, sizeof(TCP_SNIFFER_INFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (PTcpSniffer)
			{
				char ModuleFile[260] = {0};
				char SaveFile[260] = {0};

				GetModuleFileNameA(NULL,ModuleFile,sizeof(ModuleFile));
				wsprintfA(SaveFile,"%s\\A-Protect.apt",ExtractFilePath(ModuleFile));
				ReadStructInfo(PTcpSniffer,SaveFile,NULL);

				for (int i=0;i<PTcpSniffer->ulCount+1;i++)
				{
					//DoXOR(0x1999, PTcpSniffer->SnifferInfo[i].Data, 99);
					//DoXOR(0x1999, PTcpSniffer->SnifferInfo[i].DataDescription, 99);
					if (!strlen(PTcpSniffer->SnifferInfo[i].Data))
						continue;

					if (strstr((char *)p->data,PTcpSniffer->SnifferInfo[i].Data) != NULL)
					{
						wsprintfA(CmdBuffer,"发现可疑数据:\r\nIP:%u.%u.%u.%u:%d\r\n描述：%s\r\n\r\n截获到的数据包如下:\r\n%s\r\n",
							NIPQUAD(p->ih->daddr), __constant_ntohs(p->th->dest),PTcpSniffer->SnifferInfo[i].DataDescription,DataBuff);

						MessageBoxA(0,CmdBuffer,"A盾电脑防护-数据监控警告",MB_ICONSTOP);
					}
				}
				VirtualFree(PTcpSniffer,sizeof(TCP_SNIFFER_INFO)*1025,MEM_RESERVE | MEM_COMMIT);
			}
			//一次
			if (Gh0st == 0){
				memcpy(Gh0stData1,p->data,5);
			}
			//二次
			if (Gh0st == 1){
				memcpy(Gh0stData2,p->data,5);

				//ghost发送命令的数据包特征
				//原理：三次发包，头5字节的数据一样，就说明是ghost
				if (strcmpi(Gh0stData1,Gh0stData2) == 0 &&
					strlen(Gh0stData2) == 5)
				{
					wsprintfA(CmdBuffer,"发现可疑数据:\r\nIP:%u.%u.%u.%u:%d\r\n描述：%s\r\n\r\n截获到的数据包如下:\r\n%s\r\n",
						NIPQUAD(p->ih->daddr), __constant_ntohs(p->th->dest),"您的计算机可能存在Gh0st木马",p->data);

					MessageBoxA(0,CmdBuffer,"A盾电脑防护-数据监控警告",MB_ICONSTOP);
				}
				Gh0st = 0;
			}
			Gh0st++;

			if (p->th->fin)
				printf("F");
			if (p->th->syn)
				printf("S");
			if (p->th->rst)
				printf("R");
			if (p->th->psh)
				printf("P");
			if (p->th->ack)
				printf("A");
			if (p->th->urg)
				printf("U");

			printf(" ");
			printf("s%u a%u w%u ", __constant_ntohl(p->th->seq), __constant_ntohl(p->th->ack_seq),
				__constant_ntohs(p->th->window));
			printf("\n");
			break;

		case IPPROTO_UDP:
			wsprintfA(addr,"%u.%u.%u.%u:%d > %u.%u.%u.%u:%d L=%d,%d",
				NIPQUAD(p->ih->saddr), __constant_ntohs(p->uh->source),
				NIPQUAD(p->ih->daddr), __constant_ntohs(p->uh->dest),
				__constant_ntohs(p->ih->tot_len), __constant_ntohs(p->uh->len));

			wsprintfA(DataBuff,"\r\n[UDP]%s \r\n%s\r\n",addr,p->data);
			SaveSnifferData(DataBuff);
			break;

		case IPPROTO_ICMP:
			wsprintfA(addr,"%u.%u.%u.%u > %u.%u.%u.%u T=%d C=%d",
				NIPQUAD(p->ih->saddr), NIPQUAD(p->ih->daddr),
				p->icmph->type, p->icmph->code);
			
			wsprintfA(DataBuff,"\r\n[ICMP]%s \r\n%s\r\n",addr,p->data);
			SaveSnifferData(DataBuff);

			break;

		default:
			printf("%u.%u.%u.%u -> %u.%u.%u.%u P=%d L=%d",
				NIPQUAD(p->ih->saddr), NIPQUAD(p->ih->daddr),
				p->ih->protocol, __constant_ntohs(p->ih->tot_len));
			printf("\n");
			break;
		}
	}
	else if (p->eh->h_proto == __constant_ntohs(ETH_P_ARP))
	{
		printf("ARP %02X:%02X:%02X:%02X:%02X:%02X %u.%u.%u.%u > %u.%u.%u.%u %02X:%02X:%02X:%02X:%02X:%02X OP=%d",
			p->ah->ar_sha[0], p->ah->ar_sha[1], p->ah->ar_sha[2], 
			p->ah->ar_sha[3], p->ah->ar_sha[4], p->ah->ar_sha[5], 
			p->ah->ar_sip[0], p->ah->ar_sip[1], p->ah->ar_sip[2], p->ah->ar_sip[3],
			p->ah->ar_tip[0], p->ah->ar_tip[1], p->ah->ar_tip[2], p->ah->ar_tip[3],
			p->ah->ar_tha[0], p->ah->ar_tha[1], p->ah->ar_tha[2], 
			p->ah->ar_tha[3], p->ah->ar_tha[4], p->ah->ar_tha[5], 
			__constant_ntohs(p->ah->ar_op));
		printf("\n");
	}
	int i=0;
	int j=0;

	if (print_data && p->data)
	{
		char buf[16];
			
		for (i=0; i<p->dlen/16; i++)
		{
			printf(".... ");

			for (j=0; j<16; j++)
				printf("%02X ", p->data[i*16+j]);

			memcpy(buf, p->data + i*16, 16);

			for (i=0; i<16; i++)
				if (buf[i] < ' ')
					buf[i] = '.';
			printf("%.*s\n", 16, buf);
		}

		if (p->dlen % 16)
		{
			printf(".... ");

			for (j=0; j<p->dlen % 16; j++)
				printf("%02X ", p->data[i*16+j]);

			for (; j<16; j++)
				printf("   ");

			memcpy(buf, p->data + i*16, p->dlen%16);

			for (i=0; i<p->dlen%16; i++)
				if (buf[i] < ' ')
					buf[i] = '.';

			printf("%.*s\n", p->dlen%16, buf);
		}
	}
}
/*
void print_help()
{
	printf("*************************************************\n");
	printf("             TCPDUMP for winXP/2K3(32bit)        \n");
	printf("                                                 \n");
	printf("TCPDUMP v0.1 Copyright (c) 1994-2004 Chunhua Liu.\n");
	printf("TCPDUMP v0.2 Copyright (c) 2012-2022 ^(oo)^\n");
	printf("*************************************************\n\n");
	printf("TCPDUMP v0.2 parameters:\n");
	printf("    -i      network card index, e.g. -i 1\n");
	printf("    -saddr  source ip address, e.g. -saddr 192.168.0.1\n");
	printf("    -daddr  dest ip address, e.g. -daddr 202.112.20.132\n");
	printf("    -host   source/dest ip address, e.g. -host 202.112.20.132\n");
	printf("    -sport  source port, e.g. -sport 1024\n");
	printf("    -dport  dest port, e.g. -dport 23\n");
	printf("    -tcp    capture only tcp packet\n");
	printf("    -udp    capture only udp packet\n");
	printf("    -icmp   capture only icmp packet\n");
	printf("    -prot   capture ip protocol packet, e.g. -prot 2 means IGMP packet\n");
	printf("    -ep	    capture ethernet protocol packet, e.g. -ep 8035 means RARP\n");
	printf("    -e      print mac address\n");
	printf("    -d      print data\n");
	printf("    -f      flood mode\n");
	printf("    -replyarp send a arp reply\n"
		   "            e.g. 192.168.0.1 11-22-33-44-55-66 <dest ip> <dest mac>\n");
}
*/
void DoCapture(CMyList *m_list)
{
	BYTE *buf = new BYTE[65536];

	OVERLAPPED ov;
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ov.Offset = 0;
	ov.OffsetHigh = 0;

	packet p;

	DWORD received_packets = 0, handled_packets = 0;
	DWORD read_request = 0, pended_request = 0;
	DWORD max_group = 0;

	while (bMainLoop)
	{
		DWORD dwLength = 65536;

		read_request++;
		if (!PacketRead(hDevice, buf, &dwLength, &ov))
		{
			if (GetLastError() != ERROR_IO_PENDING)
				break;

			pended_request++;

			while (bMainLoop)
			{
				if (WaitForSingleObject(ov.hEvent, 1000) == WAIT_OBJECT_0)
				{
					GetOverlappedResult(hDevice, &ov, &dwLength, TRUE);
					goto __ok;
				}					
			}
			break;
		}
__ok:

		DWORD i = 0;
		PPACKET_GROUP pGroup;

		for (pGroup = (PPACKET_GROUP)buf;
				(DWORD)pGroup - (DWORD)buf < dwLength;
				pGroup = (PPACKET_GROUP)(pGroup->Data + pGroup->Length)
			)
		{
			received_packets++;
			i++;
		
			packet_init(&p, pGroup->Data, pGroup->Length);
			if (packet_filter(&p))
			{
				handled_packets++;

				packet_print(&p,m_list);

			}
		}
		if ((DWORD)pGroup - (DWORD)buf != dwLength)
		{
			printf("Bad packet buffer!\n");
			break;
		}
		if (i > max_group)
			max_group = i;
	}

	STATISTICS_DATA sd;
	PacketQueryStatistics(hDevice, &sd);
	
	printf("Packet read invoked: %d, pended %d.\n",
		read_request, pended_request);
	printf("Received %u packets, handled %u packets, max group %d.\n",
		received_packets, handled_packets, max_group);
	printf("Statistics Rx: %I64d, Drop: %I64d rate=%.3f%%\n",
		sd.ReceivedPackets, sd.DroppedPackets, 100.0f*sd.DroppedPackets/sd.ReceivedPackets);

	CloseHandle(ov.hEvent);
	delete buf;
}

BOOL ReplyARP(struct arphdr *ah)
{
	BOOL bResult = FALSE;
	OVERLAPPED ov;
	ov.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	ov.Offset = 0;
	ov.OffsetHigh = 0;
	
	const int arplen = sizeof(struct ethhdr) + sizeof(struct arphdr);
	char buf[arplen];

	struct ethhdr *eh = (struct ethhdr *)buf;
	memcpy(eh->h_dest, ah->ar_tha, ETH_ALEN);
	memcpy(eh->h_source, ah->ar_sha, ETH_ALEN);
	eh->h_proto = __constant_htons(ETH_P_ARP);
	memcpy(eh + 1, ah, sizeof(struct arphdr));

	DWORD dwLength = arplen;
	if (!PacketWrite(hDevice, buf, &dwLength, &ov))
	{
		if (GetLastError() == ERROR_IO_PENDING)
			if (GetOverlappedResult(hDevice, &ov, &dwLength, TRUE))
				if (dwLength == arplen)
					bResult = TRUE;
	}
	else
		bResult = TRUE;

	CloseHandle(ov.hEvent);

	return bResult;
}
//TRUE start
//FALSE stop
BOOL TcpSnifferData(BOOL Type)
{
	CAdapter adapter;
	int i=0;

	if (Type){
		adapter.Create();

		if (adapter.GetCount() == 0)
		{
			printf("No adapter installed.\n");
			return 0;
		}

		for (i=0; i<adapter.GetCount(); i++)
		{
			printf("%d.\n", i);
			printf("    AdapterName=%s\n", adapter[i]->AdapterName);
			printf("    Description=%s\n", adapter[i]->Description);
			printf("    Hardware Address=%02X:%02X:%02X:%02X:%02X:%02X\n",
				adapter[i]->Address[0], adapter[i]->Address[1], adapter[i]->Address[2],
				adapter[i]->Address[3], adapter[i]->Address[4], adapter[i]->Address[5]);
			printf("    IP Address=%s\n", adapter[i]->IpAddressList.IpAddress.String);
			printf("    Gateway=%s\n", adapter[i]->GatewayList.IpAddress.String);
		}
		if (eth >= adapter.GetCount())
		{
			printf("You don't have this card.\n");
			return 0;
		}
		//*************************************************
		//释放驱动文件并且加载
		//*************************************************
		char ExeModuleFile[260] = {0};
		char DriverFile[260] = {0};

		GetWindowsDirectoryA(ExeModuleFile,sizeof(ExeModuleFile));
		sprintf(DriverFile,"%s\\A-ProtectTcpSniffer.sys",ExeModuleFile);

		DeleteFileA(DriverFile);
		BFS_WriteFile(
			DriverFile,
			lpszTcpsniffer,
			sizeof(lpszTcpsniffer)
			);
		if (GetFileAttributesA(DriverFile) == INVALID_FILE_ATTRIBUTES)
		{
			printf("write driver file failed.\n");
			return 0;
		}
		LoadNTDriver(TcpSniffer,DriverFile);
		//*************************************************
		hDevice = PacketOpenAdapter(adapter[eth]->AdapterName, TRUE);
		if (hDevice == INVALID_HANDLE_VALUE)
		{
			printf("Open adapter %s failed(%d)\n", adapter[0]->AdapterName, GetLastError());
			return 0;
		}
		PacketSetPacketFilter(hDevice, NDIS_PACKET_TYPE_PROMISCUOUS);

	}else{

		PacketSetPacketFilter(hDevice, NDIS_PACKET_TYPE_PROMISCUOUS);
		PacketCloseAdapter(hDevice);
		//UnloadNTDriver(TcpSniffer);
	}
}
int StartTcpSniffer(char *argv,char *Buffer)
{
	if (strcmp(argv, "-i") == 0)
		eth = atoi(Buffer);
	else if (strcmp(argv, "-saddr") == 0)
		saddr = inet_addr(Buffer);
	else if (strcmp(argv, "-daddr") == 0)
		daddr = inet_addr(Buffer);
	else if (strcmp(argv, "-host") == 0)
		saddr = daddr = inet_addr(Buffer);
	else if (strcmp(argv, "-sport") == 0 )
	{
		h_proto = __constant_htons(ETH_P_IP);
		sport = atoi(Buffer);
		sport = __constant_htons(sport);
	}
	else if (strcmp(argv, "-dport") == 0)
	{
		h_proto = __constant_htons(ETH_P_IP);
		dport = atoi(Buffer);
		dport = __constant_htons(dport);
	}
	else if (strcmp(argv, "-tcp") == 0)
	{
		h_proto = __constant_htons(ETH_P_IP);
		protocol = IPPROTO_TCP;
	}
	else if (strcmp(argv, "-udp") == 0)
	{
		h_proto = __constant_htons(ETH_P_IP);
		protocol = IPPROTO_UDP;
	}
	else if (strcmp(argv, "-icmp") == 0)
	{
		h_proto = __constant_htons(ETH_P_IP);
		protocol = IPPROTO_ICMP;
	}
	else if (strcmp(argv, "-prot") == 0)
	{
		h_proto = __constant_htons(ETH_P_IP);
		protocol = atoi(Buffer);
	}
	else if (strcmp(argv, "-ep") == 0)
	{
		sscanf(Buffer, "%x", &h_proto);
		h_proto = __constant_htons(h_proto);
	}
	else if (strcmp(argv, "-arp") == 0)
		h_proto = __constant_htons(ETH_P_ARP);

	else if (strcmp(argv, "-f") == 0)
		flood = 1;
	else if (strcmp(argv, "-e") == 0)
		print_eh = 1;
	else if (strcmp(argv, "-d") == 0)
		print_data = 1;
	else if (strcmp(argv, "-?") == 0)
	{
		//print_help();
		return 0;
	}
	return 0;
}

