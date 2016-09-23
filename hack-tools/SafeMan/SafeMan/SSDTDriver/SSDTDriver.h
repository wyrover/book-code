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

// 得到SSDT
#define IOCTL_SSDTDRIVER_GETSSDT CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x810, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

// 通过索引得到函数地址
#define IOCTL_SSDTDRIVER_GETFUNADDRBYINDEX CTL_CODE(\
	       FILE_DEVICE_UNKNOWN, \
	       0x811, \
	       METHOD_BUFFERED, \
	       FILE_ANY_ACCESS)

// 修复SSDT
#define IOCTL_SSDTDRIVER_RESTOREFUNADDR CTL_CODE(\
	       FILE_DEVICE_UNKNOWN, \
	       0x812, \
	       METHOD_BUFFERED, \
	       FILE_ANY_ACCESS)


// 设备扩展
typedef struct _DEVICE_EXTENSION {
	ULONG stateVariable;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


// SSDT结构
typedef struct _tagSSDT {
	PVOID pvSSDTBase;  // SSDT基地址
	PVOID pvServiceCounterTable;  // 指向另一个表，该表包含了每个服务表项被调用的次数
	ULONG ulNumberOfServices;  // 当前系统所支持的服务个数
	PVOID pvParamTableBase;  // 指向系统服务参数表，该表包含了每个服务所需的参数字节数
} SSDT, *PSSDT;


// 重置SSDT信息
typedef struct _RESTORE_INFO {
	ULONG ulIndex;  // 服务号
	ULONG ulAddr;  // 服务函数的真实地址
} RESTORE_INFO, *PRESTORE_INFO;


extern "C" PSSDT KeServiceDescriptorTable;  // KeServiceDescriptorTable全局变量，外援