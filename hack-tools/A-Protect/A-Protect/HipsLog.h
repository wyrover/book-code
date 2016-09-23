#include "stdafx.h"
#include "Md5.h"
//------------------------------------------------------------
//防御日志
//------------------------------------------------------------
typedef struct _LOGDEFENSE_INFORMATION {          //LOGDEFENSE_INFORMATION
	int  Type;     //类型 1:自己是否被结束 2:创建修改服务
	ULONG ulPID;
	ULONG ulInheritedFromProcessId;      //父进程
	CHAR lpszProName[100];            //进程名
	ULONG EProcess;
	WCHAR lpwzMoreEvents[1024];
	WCHAR lpwzCreateProcess[260];

} LOGDEFENSE_INFORMATION, *PLOGDEFENSE_INFORMATION;
typedef struct _LOGDEFENSE {          //防御日志
	ULONG ulCount;
	LOGDEFENSE_INFORMATION LogDefense[1];
} LOGDEFENSE, *PLOGDEFENSE;

PLOGDEFENSE LogDefenseInfo;

BOOL InstallDepthServicesScan(CHAR * serviceName);
void ShutdownWindows( DWORD dwReason );

extern unsigned int conv(unsigned int a);

extern WCHAR WindowsXPSP3_CN[1900][50];
extern WCHAR WindowsXPSP2_CN[1753][50];

extern WCHAR Windows2003SP1_CN[1776][50];
extern WCHAR Windows2003SP2_CN[1856][50];

extern WCHAR Windows7Home_CN[2409][50];
extern WCHAR Windows7SP1_CN[2526][50];

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue);
void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);

CImageList HipsLogImg;//进程图标

extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);