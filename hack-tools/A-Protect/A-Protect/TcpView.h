#include "stdafx.h"
#include "Md5.h"
//系统版本
extern WCHAR WindowsXPSP3_CN[1900][50];
extern WCHAR WindowsXPSP2_CN[1753][50];

extern WCHAR Windows2003SP1_CN[1776][50];
extern WCHAR Windows2003SP2_CN[1856][50];
extern WCHAR Windows7Home_CN[2409][50];
extern WCHAR Windows7SP1_CN[2526][50];

extern unsigned int conv(unsigned int a);

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);
BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath);

typedef struct _TCPUDPINFO_INFORMATION {          //TCPUDPINFO_INFORMATION
	ULONG ulType;   //协议
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

PTCPUDPINFO TCPUDPInfo;

CImageList TcpViewImg;//进程图标