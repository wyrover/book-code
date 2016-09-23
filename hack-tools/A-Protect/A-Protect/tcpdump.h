#include "stdafx.h"
#include <stdlib.h>

#include "ndis5hlp.h"

#include "..\\share\\netstd.h"
#include "..\\share\\adapter.h"

#include "..\\TcpSnifferDriver\\objchk_win7_x86\\i386\\tcpsniffer.h"

#pragma comment(lib,"Iphlpapi.lib")

//**********************************************
//读取要监控的自定义数据
typedef struct _SNIFFER_INFO{

	CHAR Data[256];        //要监控的自定义数据
	CHAR DataDescription[260];        //数据描述

}SNIFFER_INFO,*PSNIFFER_INFO;

typedef struct _TCP_SNIFFER_INFO {          //SNIFFER_INFO
	ULONG ulCount;
	SNIFFER_INFO SnifferInfo[1];

} TCP_SNIFFER_INFO, *PTCP_SNIFFER_INFO;

//**********************************************

struct packet
{
	struct ethhdr *eh;
	union
	{
		struct arphdr *ah;
		struct iphdr *ih;
	};
	union
	{
		struct tcphdr *th;
		struct udphdr *uh;
		struct icmphdr *icmph;
	};
	PBYTE data;
	int dlen;
	int plen;
};

HANDLE hDevice = INVALID_HANDLE_VALUE;
volatile BOOL bMainLoop = TRUE;

int print_eh = 0;
int print_data = 0;
u_long saddr = 0;
u_long daddr = 0;
u_short sport = 0;
u_short dport = 0;
BYTE protocol = 0;
u_short h_proto = 0;

int eth = 0;
int todo = 0;
int flood = 0;
struct arphdr ah;

char *TcpSniffer = "A-ProtectTcpSniffer";
ULONG count;

char Gh0stData1[10] = {0};
char Gh0stData2[10] = {0};
char Gh0stData3[10] = {0};
int Gh0st;

extern BOOL BFS_WriteFile(char *targetPath,unsigned char *lpszCode,ULONG ulSize);
extern LPSTR ExtractFilePath(LPSTR lpcFullFileName);
extern BOOL LoadNTDriver(char* lpszDriverName,char* lpszDriverPath);
extern BOOL UnloadNTDriver( char * szSvrName );