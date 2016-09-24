/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：public.h
	说明：共用头文件
	历史：
	创建：
*/

#ifndef __PUBLIC_H__
#define __PUBLIC_H__

#ifdef __cplusplus
extern "C"{
#endif

#pragma warning(disable:4200)  // nameless struct/union
#pragma warning(disable:4201)  // nameless struct/union
#pragma warning(disable:4115)  // named typedef in parenthesis
#pragma warning(disable:4214)  // bit field types other than int

#include <ntddk.h>
#include <usbdi.h>
#include <wdf.h>
#include <Wdfusb.h>
#include <usbdlib.h>
#include <usbbusif.h>
#include <ntstrsafe.h>

#ifndef _WORD_DEFINED
#define _WORD_DEFINED
typedef unsigned short  WORD;
typedef unsigned int	UINT;
#endif 

#include "structure.h"

#define DRIVER_CY001	"CY001: "
#define POOLTAG			'00YC'

#define  MAX_INSTANCE_NUMBER 8
#define  MAX_INTERFACES 8

#define REGISTER_DRV_CLASS(DriverName) \
DrvClass* GetDrvClass(){\
	return  (DrvClass*)new(NonPagedPool, 'GETD') DriverName();\
}

#define REGISTER_DRV_CLASS_NULL()\
	DrvClass* GetDrvClass(){\
	return  new(NonPagedPool, 'GETD') DrvClass();\
}

// WDF设备环境
typedef struct _DEVICE_CONTEXT {
	PVOID pThis;// used by base class

	// below parameters are reserved for subclass.
	PVOID par2;
	PVOID par3;
	PVOID par4;
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

// WDF驱动环境
typedef struct {
	PVOID pThis;// used by base class

	// below parameters are reserved for subclass.
	PVOID par2;
	PVOID par3;
	PVOID par4;
}DRIVER_CONTEXT, *PDRIVER_CONTEXT;


//===============================函数声明

// 定义析取环境块的函数指针，
// 通过GetDeviceContext能从任一框架对象取得一个指向结构体DEVICE_CONTEXT的指针。
// 使用GetDeviceContext的前提是，目标对象已经定义了一个名称为DEVICE_CONTEXT的环境块。
// 在本工程中，我们在PnpAdd函数中为设备对象定义了DEVICE_CONTEXT环境块。
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext);

// 定义析取环境块的函数指针，
// 通过GetDeviceContext能从任一框架对象取得一个指向结构体DEVICE_CONTEXT的指针。
// 使用GetDeviceContext的前提是，目标对象已经定义了一个名称为DEVICE_CONTEXT的环境块。
// 在本工程中，我们在PnpAdd函数中为设备对象定义了DEVICE_CONTEXT环境块。
WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DRIVER_CONTEXT, GetDriverContext);

//===============================配置函数
NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject, 
    IN PUNICODE_STRING  RegistryPath
	);

char* 
PowerName(
	WDF_POWER_DEVICE_STATE PowerState
	);

#if (DBG)
__inline KDBG(int nLevel, char* msg, ...)
{
	static char csmsg[1024];

	va_list argp;
	va_start(argp, msg);
	vsprintf(csmsg, msg, argp);
	va_end(argp);

	KdPrintEx((DPFLTR_DEFAULT_ID, nLevel, "CY001:"));// 格式化头
	KdPrintEx((DPFLTR_DEFAULT_ID, nLevel, csmsg));	 // Log body
	KdPrintEx((DPFLTR_DEFAULT_ID, nLevel, "\n"));	 // 换行
}
#else
__inline KDBG(int nLevel, char* msg, ...)
{
}
#endif


#ifdef __cplusplus
}
#endif

#endif