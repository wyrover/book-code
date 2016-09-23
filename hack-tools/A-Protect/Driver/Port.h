#ifndef _PORT_H_
#define _PORT_H_

#include "ntifs.h"
#include <tdiinfo.h>
#include <tdistat.h>
#include "ntos.h"
#include "Strsafe.h"
#include <wsk.h>

extern POBJECT_TYPE *PsProcessType;

#define TCP_MIB_ADDRTABLE_ENTRY_EX_ID 0x102

/* TCP/UDP/RawIP IOCTL code definitions */

#define FSCTL_TCP_BASE                  FILE_DEVICE_NETWORK

#define _TCP_CTL_CODE(function, method, access) \
	CTL_CODE(FSCTL_TCP_BASE, function, method, access)

#define IOCTL_TCP_QUERY_INFORMATION_EX  \
	_TCP_CTL_CODE(0, METHOD_NEITHER, FILE_ANY_ACCESS)

#define ntohs(s) \
	(((s >> 8) & 0x00FF) | \
	((s << 8) & 0xFF00))

enum {TCPPORT, UDPPORT};

typedef ULONG DWORD;

typedef struct _MIB_TCPROW_OWNER_PID
{
	DWORD dwState;
	DWORD dwLocalAddr;
	DWORD dwLocalPort;
	DWORD dwRemoteAddr;
	DWORD dwRemotePort;
	DWORD dwOwningPid;

} MIB_TCPROW_OWNER_PID, *PMIB_TCPROW_OWNER_PID;

typedef struct _MIB_UDPROW_OWNER_PID {
	DWORD dwLocalAddr;
	DWORD dwLocalPort;
	DWORD dwOwningPid;
} MIB_UDPROW_OWNER_PID, *PMIB_UDPROW_OWNER_PID;

///////////////////////////////////////////////////////////////////////////////////////////////
#define MIB_TCP_STATE_CLOSED            1
#define MIB_TCP_STATE_LISTEN            2
#define MIB_TCP_STATE_SYN_SENT          3
#define MIB_TCP_STATE_SYN_RCVD          4
#define MIB_TCP_STATE_ESTAB             5
#define MIB_TCP_STATE_FIN_WAIT1         6
#define MIB_TCP_STATE_FIN_WAIT2         7
#define MIB_TCP_STATE_CLOSE_WAIT        8
#define MIB_TCP_STATE_CLOSING           9
#define MIB_TCP_STATE_LAST_ACK         10
#define MIB_TCP_STATE_TIME_WAIT        11
#define MIB_TCP_STATE_DELETE_TCB       12

#pragma pack (1)

typedef struct _UF_TCPROW_PID{  
	CHAR  State[128];  
	CHAR  LocalAddr[128];  
	CHAR  RemoteAddr[128];  
	WCHAR ProcessPath[260]; 
	WCHAR ProcessName[128]; 
    WCHAR ProductName[260];
	WCHAR CompanyName[260];
	WCHAR FileDescription[260];
	ULONG dwPid;
}UF_TCPROW_PID, *PUF_TCPROW_PID;

typedef struct _UF_UDPROW_PID{  
	CHAR   LocalAddr[128];  
	CHAR   RemoteAddr[128];  
	WCHAR ProcessPath[260]; 
	WCHAR ProcessName[128]; 
    WCHAR ProductName[260];
	WCHAR CompanyName[260];
	WCHAR FileDescription[260];  
	ULONG  dwPid;
}UF_UDPROW_PID, *PUF_UDPROW_PID;

#pragma pack ()
 
typedef struct _INTERNAL_TCP_TABLE_SUBENTRY
{
	char bytesfill0[2];
	USHORT Port;
	ULONG dwIP;
	char bytesfill[20];

}INTERNAL_TCP_TABLE_SUBENTRY,*PINTERNAL_TCP_TABLE_SUBENTRY;

typedef struct _INTERNAL_TCP_TABLE_ENTRY
{
	INTERNAL_TCP_TABLE_SUBENTRY localEntry;
	INTERNAL_TCP_TABLE_SUBENTRY remoteEntry;
	
}INTERNAL_TCP_TABLE_ENTRY,*PINTERNAL_TCP_TABLE_ENTRY;

typedef struct _INTERNAL_UDP_TABLE_ENTRY
{
	char bytesfill0[2];
	USHORT Port;
	ULONG dwIP;
	char bytesfill[20];

}INTERNAL_UDP_TABLE_ENTRY,*PINTERNAL_UDP_TABLE_ENTRY;

typedef struct _NSI_STATUS_ENTRY
{   
	ULONG  dwState;
	char bytesfill[8]; 

}NSI_STATUS_ENTRY,*PNSI_STATUS_ENTRY;

typedef struct _NSI_PROCESSID_INFO
{
	
	ULONG dwUdpProId;
	ULONG UnknownParam2;
	ULONG UnknownParam3;
	ULONG dwProcessId;
	ULONG UnknownParam5;
	ULONG UnknownParam6;
	ULONG UnknownParam7;
	ULONG UnknownParam8;
	
}NSI_PROCESSID_INFO,*PNSI_PROCESSID_INFO;
 
typedef struct _NSI_PARAM
{
	 
	ULONG_PTR UnknownParam1;
	SIZE_T UnknownParam2;
	PVOID UnknownParam3;
	SIZE_T UnknownParam4;
	ULONG UnknownParam5;
	ULONG UnknownParam6;
	PVOID UnknownParam7;
	SIZE_T UnknownParam8;
	PVOID UnknownParam9;
	SIZE_T UnknownParam10;
	PVOID UnknownParam11;
	SIZE_T UnknownParam12;
	PVOID UnknownParam13;
	SIZE_T UnknownParam14;
	SIZE_T ConnCount;      

}NSI_PARAM,*PNSI_PARAM;

#define IOCTL_NSI_GETALLPARAM 0x12001B
//////////////////////////////////////////////////////////////////////////////////////

typedef struct _TCPUDPINFO_INFORMATION {          //TCPUDPINFO_INFORMATION
	ULONG ulType;   //Ð­Òé
	ULONG ulLocalAddress;
	ULONG ulLocalPort;

	ULONG ulRemoteAddress;
	ULONG ulRemotePort;
	ULONG ulConnectType;
	ULONG ulPid;
	WCHAR lpwzFullPath[256];
	char lpszFileMd5[100];
} TCPUDPINFO_INFORMATION, *PTCPUDPINFO_INFORMATION;

typedef struct _TCPUDPINFO {          //TCPUDP
	ULONG ulCount;
	TCPUDPINFO_INFORMATION TCPUDP[1];
} TCPUDPINFO, *PTCPUDPINFO;


extern BOOL DebugOn;

NTSTATUS ZwQueryInformationProcess(
  __in       HANDLE ProcessHandle,
  __in       ULONG ProcessInformationClass,
  __out      PVOID ProcessInformation,
  __in       ULONG ProcessInformationLength,
  __out_opt  PULONG ReturnLength
);

NTSTATUS SafeCopyMemory(
	PVOID SrcAddr, 
	PVOID DstAddr, 
	ULONG Size
	);
NTSTATUS GetObjectByName(
	OUT PHANDLE FileHandle,
	OUT PFILE_OBJECT  *FileObject,
	IN WCHAR  *DeviceName
	);
NTSTATUS LookupProcessByPid(
	IN HANDLE hProcessPid,
	OUT PEPROCESS *pEprocess
	);

BOOL GetProcessFullImagePath(    
	IN  PEPROCESS Eprocess,     
	OUT WCHAR *FullProcessImagePath    
	);
#endif