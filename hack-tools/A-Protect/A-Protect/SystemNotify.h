#include "stdafx.h"

//---------------------------------------------------------------------------------------
//system notify
//---------------------------------------------------------------------------------------
typedef struct _SYSTEM_NOTIFY_INFORMATION {          //SYSTEM_NOTIFY_INFORMATION
	WCHAR lpwzType[256];
	ULONG ulNotifyBase;
	CHAR lpszModule[256];
	WCHAR lpwzObject[256];
	ULONG ulObject;
} SYSTEM_NOTIFY_INFORMATION, *PSYSTEM_NOTIFY_INFORMATION;

typedef struct _SYSTEM_NOTIFY {          //Notify
	ULONG ulCount;
	SYSTEM_NOTIFY_INFORMATION NotifyInfo[1];
} SYSTEM_NOTIFY, *PSYSTEM_NOTIFY;

PSYSTEM_NOTIFY SystemNotify;

CImageList SystemNotiImg;//½ø³ÌÍ¼±ê

CHAR* setClipboardText(CHAR* str);