#ifndef _SYSTEM_NOTIFY_H_
#define _SYSTEM_NOTIFY_H_

#include "ntifs.h"
#include "InitWindowsVersion.h"
#include "ntos.h"
#include "ldasm.h"
#include <wchar.h>

#define PNPNOTIFY_DEVICE_INTERFACE_INCLUDE_EXISTING_INTERFACES    0x00000001

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

//链表结构
typedef struct _NOTIFICATION_PACKET {
    LIST_ENTRY ListEntry;
    PDRIVER_OBJECT DriverObject;
    ULONG NotificationRoutine;
} NOTIFICATION_PACKET, *PNOTIFICATION_PACKET;

#pragma pack(push)
#pragma pack(1)           
// typedef struct _EX_FAST_REF
// {
//     union
//     {
//         PVOID Object;
//         ULONG_PTR RefCnt:3;
//         ULONG_PTR Value;
//     };
// } EX_FAST_REF, *PEX_FAST_REF;

typedef struct _EX_CALLBACK_ROUTINE_BLOCK
{
    EX_RUNDOWN_REF RundownProtect;
    //PEX_CALLBACK_FUNCTION Function;
	PVOID Function;
    PVOID Context;
} EX_CALLBACK_ROUTINE_BLOCK, *PEX_CALLBACK_ROUTINE_BLOCK;

#pragma pack(pop)

typedef struct _SHUTDOWN_PACKET {
    LIST_ENTRY ListEntry;
    PDEVICE_OBJECT DeviceObject;
} SHUTDOWN_PACKET, *PSHUTDOWN_PACKET;

typedef enum _IO_NOTIFICATION_EVENT_CATEGORY {
	EventCategoryReserved,
	EventCategoryHardwareProfileChange,
	EventCategoryDeviceInterfaceChange,
	EventCategoryTargetDeviceChange
} IO_NOTIFICATION_EVENT_CATEGORY;


typedef NTSTATUS (*PDRIVER_NOTIFICATION_CALLBACK_ROUTINE) (
	IN PVOID NotificationStructure,
	IN PVOID Context
	);

typedef struct _PNP_NOTIFY_ENTRY
{
    LIST_ENTRY PnpNotifyList;
    IO_NOTIFICATION_EVENT_CATEGORY EventCategory;
    PVOID Context;
    UNICODE_STRING Guid;
    PFILE_OBJECT FileObject;
    PDRIVER_NOTIFICATION_CALLBACK_ROUTINE PnpNotificationProc;
} PNP_NOTIFY_ENTRY, *PPNP_NOTIFY_ENTRY;


typedef NTSTATUS (*PSE_LOGON_SESSION_TERMINATED_ROUTINE) (
  IN PLUID LogonId
  );
typedef struct _SEP_LOGON_SESSION_TERMINATED_NOTIFICATION {
    struct _SEP_LOGON_SESSION_TERMINATED_NOTIFICATION *Next;
    PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine;
} SEP_LOGON_SESSION_TERMINATED_NOTIFICATION, *PSEP_LOGON_SESSION_TERMINATED_NOTIFICATION;

NTSTATUS
SeRegisterLogonSessionTerminatedRoutine(
    IN PSE_LOGON_SESSION_TERMINATED_ROUTINE CallbackRoutine
    );

NTSTATUS IoUnregisterPlugPlayNotification(
	__in  PVOID NotificationEntry
	);

NTSTATUS IoRegisterPlugPlayNotification(
	__in      IO_NOTIFICATION_EVENT_CATEGORY EventCategory,
	__in      ULONG EventCategoryFlags,
	__in_opt  PVOID EventCategoryData,
	__in      PDRIVER_OBJECT DriverObject,
	__in      PDRIVER_NOTIFICATION_CALLBACK_ROUTINE CallbackRoutine,
	__in_opt  PVOID Context,
	__out     PVOID *NotificationEntry
	);

VOID IoUnregisterFsRegistrationChange(
	__in  PDRIVER_OBJECT DriverObject,
	__in  PDRIVER_FS_NOTIFICATION DriverNotificationRoutine
	);

ULONG PsSetLegoNotifyRoutine(PVOID notifyroutine);
VOID GetListHeadAddr();

DEFINE_GUID(GUID_CLASS_USBKEY,0x36FC9E60, 0xC465, 0x11CF, 0x80, 0x56, 0x44, 0x45, 0x53, 0x54, 0x000, 0x00);

PVOID  NotificationEntry;

ULONG FsNotifyNum;//文件系统回调个数
ULONG ShutDownNum;//关机回调个数
ULONG CreateThreadNum;//创建线程回调个数
ULONG CreateProcessNum;//创建进程回调个数
ULONG LoadImageNum;//加载模块回调个数
ULONG LegoNum;//Lego回调个数
ULONG LeaveSessionNum;//注销回调个数
ULONG BugCheckNum;//BugCheck回调个数
ULONG RegNum;//注册表回调个数
ULONG PlugPlayNum;//PlugPlay回调个数

//------------------------------------------------------------------------------------------//文件系统回调
ULONG FileSystemRoutine;//回调函数地址
//文件系统回调链表头
//__declspec(dllimport) LIST_ENTRY IopFsNotifyChangeQueueHead;     //全局变量
ULONG IopFsNotifyChangeQueueHead;

ULONG ulModuleBase;
ULONG ulModuleSize;

extern BOOL DebugOn;

BOOL IsAddressInSystem(ULONG ulDriverBase,ULONG *ulSysModuleBase,ULONG *ulSize,char *lpszSysModuleImage);

#endif