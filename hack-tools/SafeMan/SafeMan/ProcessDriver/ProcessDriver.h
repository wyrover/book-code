#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <NTDDK.h>
#ifdef __cplusplus
}
#endif 

// IOCTLs

// 得到进程数目
#define IOCTL_PROCESSDRIVER_PSPCIDTABLE_GETPROCESSNUMBER CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x800, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

// 通过PspcidTable得到进程信息
#define IOCTL_PROCESSDRIVER_PSPCIDTABLE_GETPROCESSINFO CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x801, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

// 结束进程
#define IOCTL_PROCESSDRIVER_TERMINATEPROCESS CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x802, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

// 通过ActiveProcessLinks得到进程信息
#define IOCTL_PROCESSDRIVER_ACTIVEPROCESSLINKS CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x803, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

// 通过ObjectTable得到进程信息
#define IOCTL_PROCESSDRIVER_OBJECTTABLE CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x804, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)


// _OBJECT_HEADER结构
typedef struct _OBJECT_HEADER {
	union {
		struct {
			LONG PointerCount;
			LONG HandleCount;
		};
		LIST_ENTRY Entry;
	};
	POBJECT_TYPE Type;
	UCHAR NameInfoOffset;
	UCHAR HandleInfoOffset;
	UCHAR QuotaInfoOffset;
	UCHAR Flags;

	union {
		//POBJECT_CREATE_INFORMATION ObjectCreateInfo;
		PVOID QuotaBlockCharged;
	};

	PSECURITY_DESCRIPTOR SecurityDescriptor;

	QUAD Body;
} OBJECT_HEADER, *POBJECT_HEADER;


// 实现从对象体指针获得对象头指针的宏
#define OBJECT_TO_OBJECT_HEADER(obj) CONTAINING_RECORD((obj), OBJECT_HEADER, Body)


#define TYPE 0X08  // _OBJECT_HEADER中TYPE的偏移
#define NEXTFREETABLEENTRY 0X04  // _HANDLE_TABLE_ENTRY中的NEXTFREETABLEENTRY偏移
#define UNIQUEPROCESSID 0X84  // _EPROCESS中的UNIQUEPROCESSID偏移
#define IMAGEFILENAME 0X174  // _EPROCESS中的IMAGEFILENAME偏移
#define FLAGS 0x248  // _EPROCESS中的FLAGS偏移
#define THREADLISTENTRY 0x22C  // _ETHREAD中的THREADLISTENTRY偏移
#define THREADLISTHEAD 0x190  // _EPROCESS中的THREADLISTHEAD偏移

#define ACTIVEPROCESSLINKS 0x88  // _EPROCESS中ActiveProcessLinks偏移
#define OBJECTTABLE 0xC4  // _EPROCESS中ObjectTable偏移

#define HANDLETABLELIST 0x1C  // _HANDLE_TABLE中HandleTableList偏移
#define UNIQUE_PROCESSID 0x008  // _HANDLE_TABLE中UniqueProcessId偏移
#define QUOTAPROCESS 0x004  // _HANDLE_TABLE中QuotaProcess偏移


// 记录进程信息的结构体
typedef struct tagProcessInfo     
{
	ULONG address;  // 进程地址           
	LONG pid;  // 进程ID
	UCHAR name[16];  // 进程名
	struct tagProcessInfo *next;  // 单向链表指针
}ProcessInfo, *PProcessInfo;   


// 设备扩展
typedef struct _DEVICE_EXTENSION
{
	ULONG stateVariable;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


// SYSTEM_INFORMATION_CLASS中SystemModuleInformation的结构定义

typedef struct _SYSTEM_MODULE_INFORMATION_ENTRY {
	HANDLE Section;
	PVOID MappedBase;
	PVOID Base;
	ULONG Size;
	ULONG Flags;
	USHORT LoadOrderIndex;
	USHORT InitOrderIndex;
	USHORT LoadCount;
	USHORT PathLength;
	CHAR ImageName[256];
} SYSTEM_MODULE_INFORMATION_ENTRY, *PSYSTEM_MODULE_INFORMATION_ENTRY;


typedef struct _SYSTEM_MODULE_INFORMATION {
	ULONG Count;
	SYSTEM_MODULE_INFORMATION_ENTRY Module[1];
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;


// ZwQuerySystemInformation的第一个参数
typedef enum _SYSTEM_INFORMATION_CLASS     //    Q S
{
	SystemBasicInformation,                // 00 Y N
	SystemProcessorInformation,            // 01 Y N
	SystemPerformanceInformation,          // 02 Y N
	SystemTimeOfDayInformation,            // 03 Y N
	SystemNotImplemented1,                 // 04 Y N
	SystemProcessesAndThreadsInformation,  // 05 Y N
	SystemCallCounts,                      // 06 Y N
	SystemConfigurationInformation,        // 07 Y N
	SystemProcessorTimes,                  // 08 Y N
	SystemGlobalFlag,                      // 09 Y Y
	SystemNotImplemented2,                 // 10 Y N
	SystemModuleInformation,               // 11 Y N
	SystemLockInformation,                 // 12 Y N
	SystemNotImplemented3,                 // 13 Y N
	SystemNotImplemented4,                 // 14 Y N
	SystemNotImplemented5,                 // 15 Y N
	SystemHandleInformation,               // 16 Y N
	SystemObjectInformation,               // 17 Y N
	SystemPagefileInformation,             // 18 Y N
	SystemInstructionEmulationCounts,      // 19 Y N
	SystemInvalidInfoClass1,               // 20
	SystemCacheInformation,                // 21 Y Y
	SystemPoolTagInformation,              // 22 Y N
	SystemProcessorStatistics,             // 23 Y N
	SystemDpcInformation,                  // 24 Y Y
	SystemNotImplemented6,                 // 25 Y N
	SystemLoadImage,                       // 26 N Y
	SystemUnloadImage,                     // 27 N Y
	SystemTimeAdjustment,                  // 28 Y Y
	SystemNotImplemented7,                 // 29 Y N
	SystemNotImplemented8,                 // 30 Y N
	SystemNotImplemented9,                 // 31 Y N
	SystemCrashDumpInformation,            // 32 Y N
	SystemExceptionInformation,            // 33 Y N
	SystemCrashDumpStateInformation,       // 34 Y Y/N
	SystemKernelDebuggerInformation,       // 35 Y N
	SystemContextSwitchInformation,        // 36 Y N
	SystemRegistryQuotaInformation,        // 37 Y Y
	SystemLoadAndCallImage,                // 38 N Y
	SystemPrioritySeparation,              // 39 N Y
	SystemNotImplemented10,                // 40 Y N
	SystemNotImplemented11,                // 41 Y N
	SystemInvalidInfoClass2,               // 42
	SystemInvalidInfoClass3,               // 43
	SystemTimeZoneInformation,             // 44 Y N
	SystemLookasideInformation,            // 45 Y N
	SystemSetTimeSlipEvent,                // 46 N Y
	SystemCreateSession,                   // 47 N Y
	SystemDeleteSession,                   // 48 N Y
	SystemInvalidInfoClass4,               // 49
	SystemRangeStartInformation,           // 50 Y N
	SystemVerifierInformation,             // 51 Y Y
	SystemAddVerifier,                     // 52 N Y
	SystemSessionProcessesInformation      // 53 Y N
} SYSTEM_INFORMATION_CLASS;


//------------------全局变量-----------------

ProcessInfo *head, *pCurrent;  // 头指针


//---------------------Native API-----------------------------

// 查询系统信息
extern "C" NTSTATUS ZwQuerySystemInformation(
								  IN SYSTEM_INFORMATION_CLASS SystemInformationClass,
								  IN OUT PVOID SystemInformation,
								  IN ULONG SystemInformationLength,
								  OUT PULONG ReturnLength OPTIONAL);

// 根据PID得到PEPROCESS
extern "C" NTSTATUS PsLookupProcessByProcessId(
									IN ULONG ProcessId,
									OUT PEPROCESS *Process);