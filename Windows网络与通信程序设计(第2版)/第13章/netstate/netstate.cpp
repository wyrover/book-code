//////////////////////////////////////////////////////////////////////
// netstate.cpp文件


#include <stdio.h>
#include <windows.h>
#include <Iphlpapi.h>
#include <tlhelp32.h>

#pragma comment(lib, "Iphlpapi.lib")
#pragma comment(lib, "WS2_32.lib")




typedef struct 
{
  DWORD   dwState;        	// 连接状态
  DWORD   dwLocalAddr;    	// 本地地址
  DWORD   dwLocalPort;    	// 本地端口
  DWORD   dwRemoteAddr;   	// 远程地址
  DWORD   dwRemotePort;   	// 远程端口
  DWORD	  dwProcessId;		// 进程ID号
} MIB_TCPEXROW, *PMIB_TCPEXROW;

typedef struct 
{
	DWORD			dwNumEntries;
	MIB_TCPEXROW	table[ANY_SIZE];
} MIB_TCPEXTABLE, *PMIB_TCPEXTABLE;

typedef struct 
{
  DWORD   dwLocalAddr;    	// 本地地址
  DWORD   dwLocalPort;    	// 本地端口
  DWORD	  dwProcessId;		// 进程ID号
} MIB_UDPEXROW, *PMIB_UDPEXROW;

typedef struct 
{
	DWORD			dwNumEntries;
	MIB_UDPEXROW	table[ANY_SIZE];
} MIB_UDPEXTABLE, *PMIB_UDPEXTABLE;


// 扩展函数原型
typedef DWORD (WINAPI *PFNAllocateAndGetTcpExTableFromStack)(
  PMIB_TCPEXTABLE *pTcpTable, 
  BOOL bOrder,             
  HANDLE heap,
  DWORD zero,
  DWORD flags
);

typedef DWORD (WINAPI *PFNAllocateAndGetUdpExTableFromStack)(
  PMIB_UDPEXTABLE *pUdpTable,  
  BOOL bOrder,              
  HANDLE heap,
  DWORD zero,
  DWORD flags
);


PCHAR ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, PCHAR ProcessName);

int main()
{
	// 定义扩展函数指针
	PFNAllocateAndGetTcpExTableFromStack pAllocateAndGetTcpExTableFromStack;
	PFNAllocateAndGetUdpExTableFromStack pAllocateAndGetUdpExTableFromStack;

	// 获取扩展函数的入口地址	
	HMODULE hModule = ::LoadLibrary("iphlpapi.dll");
	pAllocateAndGetTcpExTableFromStack = 
			(PFNAllocateAndGetTcpExTableFromStack)::GetProcAddress(hModule, 
									"AllocateAndGetTcpExTableFromStack");
	
	pAllocateAndGetUdpExTableFromStack = 
			(PFNAllocateAndGetUdpExTableFromStack)::GetProcAddress(hModule, 
									"AllocateAndGetUdpExTableFromStack");

	if(pAllocateAndGetTcpExTableFromStack == NULL || pAllocateAndGetUdpExTableFromStack == NULL)
	{
		printf(" Ex APIs are not present \n ");
		// 说明你应该调用普通的IP帮助API去获取TCP连接表和UDP监听表
		return 0;
	}

	// 调用扩展函数，获取TCP扩展连接表和UDP扩展监听表

	PMIB_TCPEXTABLE pTcpExTable;
	PMIB_UDPEXTABLE pUdpExTable;

	// pTcpExTable和pUdpExTable所指的缓冲区自动由扩展函数在进程堆中申请
	if(pAllocateAndGetTcpExTableFromStack(&pTcpExTable, TRUE, GetProcessHeap(), 2, 2) != 0)
	{
			printf(" Failed to snapshot TCP endpoints.\n");
			return -1;
	}
	if(pAllocateAndGetUdpExTableFromStack(&pUdpExTable, TRUE, GetProcessHeap(), 2, 2) != 0)
	{
			printf(" Failed to snapshot UDP endpoints.\n");
			return -1;
	}

	// 给系统内的所有进程拍一个快照
	HANDLE hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if(hProcessSnap == INVALID_HANDLE_VALUE)
	{
		printf(" Failed to take process snapshot. Process names will not be shown.\n\n");
		return -1;
	}

	printf(" Active Connections \n\n");
	char	szLocalAddr[128];
	char	szRemoteAddr[128];
	char	szProcessName[128];
	in_addr inadLocal, inadRemote;
	char    strState[128];
	DWORD   dwRemotePort = 0;	

	// 打印TCP扩展连接表信息
	for(UINT i = 0; i < pTcpExTable->dwNumEntries; ++i)
	{
		// 状态
		switch (pTcpExTable->table[i].dwState)
		{
		case MIB_TCP_STATE_CLOSED:
			strcpy(strState, "CLOSED");
			break;
		case MIB_TCP_STATE_TIME_WAIT:
			strcpy(strState, "TIME_WAIT");
			break;
		case MIB_TCP_STATE_LAST_ACK:
			strcpy(strState, "LAST_ACK");
			break;
		case MIB_TCP_STATE_CLOSING:
			strcpy(strState, "CLOSING");
			break;
		case MIB_TCP_STATE_CLOSE_WAIT:
			strcpy(strState, "CLOSE_WAIT");
			break;
		case MIB_TCP_STATE_FIN_WAIT1:
			strcpy(strState, "FIN_WAIT1");
			break;
		case MIB_TCP_STATE_ESTAB:
			strcpy(strState, "ESTAB");
			break;
		case MIB_TCP_STATE_SYN_RCVD:
			strcpy(strState, "SYN_RCVD");
			break;
		case MIB_TCP_STATE_SYN_SENT:
			strcpy(strState, "SYN_SENT");
			break;
		case MIB_TCP_STATE_LISTEN:
			strcpy(strState, "LISTEN");
			break;
		case MIB_TCP_STATE_DELETE_TCB:
			strcpy(strState, "DELETE");
			break;
		default:
			printf("Error: unknown state!\n");
			break;
		}
		// 本地IP地址
		inadLocal.s_addr = pTcpExTable->table[i].dwLocalAddr;
		
		// 远程端口
		if(strcmp(strState, "LISTEN") != 0)
		{
			dwRemotePort = pTcpExTable->table[i].dwRemotePort;
		}
		else
			dwRemotePort = 0;

		// 远程IP地址
		inadRemote.s_addr = pTcpExTable->table[i].dwRemoteAddr;
		

		sprintf(szLocalAddr, "%s:%u", inet_ntoa(inadLocal), 
					ntohs((unsigned short)(0x0000FFFF & pTcpExTable->table[i].dwLocalPort)));
		sprintf(szRemoteAddr, "%s:%u", inet_ntoa(inadRemote), 
					ntohs((unsigned short)(0x0000FFFF & dwRemotePort)));

		// 打印出此入口的信息
		printf("%-5s %s:%d\n      State:   %s\n", "[TCP]", 
			ProcessPidToName(hProcessSnap, pTcpExTable->table[i].dwProcessId, szProcessName),
			pTcpExTable->table[i].dwProcessId,
			strState);

		printf("      Local:   %s\n      Remote:  %s\n",
			szLocalAddr, szRemoteAddr);
	}

	// 打印UDP监听表信息
	for(i = 0; i < pUdpExTable->dwNumEntries; ++i)
	{
		// 本地IP地址
		inadLocal.s_addr = pUdpExTable->table[i].dwLocalAddr;

		sprintf(szLocalAddr,  "%s:%u", inet_ntoa(inadLocal), 
				ntohs((unsigned short)(0x0000FFFF & pUdpExTable->table[i].dwLocalPort)));

		// 打印出此入口的信息
		printf("%-5s %s:%d\n", "[UDP]", 
			ProcessPidToName(hProcessSnap, pUdpExTable->table[i].dwProcessId, szProcessName),
			pUdpExTable->table[i].dwProcessId );
		printf("      Local:   %s\n      Remote:  %s\n",
			szLocalAddr, "*.*.*.*:*" );
	}
	
	
	::CloseHandle(hProcessSnap);
	::LocalFree(pTcpExTable);
	::LocalFree(pUdpExTable);
	::FreeLibrary(hModule);
	return 0;
}


// 将进程ID号（PID）转化为进程名称
PCHAR ProcessPidToName(HANDLE hProcessSnap, DWORD ProcessId, PCHAR ProcessName)
{
	PROCESSENTRY32 processEntry;
	processEntry.dwSize = sizeof(processEntry);
	// 找不到的话，默认进程名为“???”
	strcpy(ProcessName, "???");
	if(!::Process32First(hProcessSnap, &processEntry)) 
		return ProcessName;
	do 
	{
		if(processEntry.th32ProcessID == ProcessId) // 就是这个进程
		{
			strcpy(ProcessName, processEntry.szExeFile);
			break;
		}
	}
	while(::Process32Next(hProcessSnap, &processEntry));
	return ProcessName;
}