#include "stdafx.h"

void UDBG(TCHAR *msg, ...);

void FreeAllInfFiles();
int FindAllInfFiles(char* csDir);
void InstallDriver();
int UpdateDriver();
void FindAllDevices(const char* asHID);
void FreeDevices();
void RemoveDevices();
BOOL Reboot();

typedef struct
{
	char asHID[MAX_PATH];
	int  nInfNum;		// g_asInfList下标号

	int  nNumCone;
	int  nNumNotCone;
	bool bNeedUpdate;

	bool bWillBeUpdate;
	bool bSuccess;
	//int  nFail;
} HID_INFO, *PHID_INFO;

typedef struct
{
	char asInfName[MAX_PATH];	// 文件名
	char asFullPath[MAX_PATH];	// 全路径
	char asDesPath[MAX_PATH];
	char *asDesName;
	char asVersion[MAX_PATH];
	char asClassName[MAX_PATH];
	char asProvider[MAX_PATH];
	char asDSign[MAX_PATH];
	bool bWillBeInstall;
	int  nResultInstall;
}INF_INFO, *PINF_INFO;

#define MAX_SYMBOL_LINK 250

typedef struct Hid_Ddvices{
	char asHID[MAX_PATH];

	char* asDeviceInstanceID[MAX_SYMBOL_LINK];
	bool bLink[MAX_SYMBOL_LINK];
	bool bSuccess[MAX_SYMBOL_LINK];
	bool bWillBeDelete[MAX_SYMBOL_LINK];

	int nMaxCount;
	int nCount;

	bool bNeedReboot;

	Hid_Ddvices()
	{
		nCount = nMaxCount = 0;
		bNeedReboot = false;

		ZeroMemory(asHID, sizeof(asHID)+ sizeof(asDeviceInstanceID) + 
			sizeof(bLink) + sizeof(bSuccess) + sizeof(bWillBeDelete));
	}
}HID_DEVICES, *PHID_DEVICES;

#define MAX_INF_COUNT 40
#define MAX_HID_COUNT 250

extern PINF_INFO g_InfList[MAX_INF_COUNT];
extern int g_nInfCount;	// INF文

extern PHID_INFO g_HIDInfo[MAX_HID_COUNT];
extern int g_nHIDCount ;	// HID个数	

extern HID_DEVICES g_HIDDevices;