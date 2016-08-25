/************************************************************************
* 文件名称:Driver.h                                                 
* 作    者:张帆
* 完成日期:2007-11-1
*************************************************************************/
#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <NTDDK.h>
#ifdef __cplusplus
}
#endif 

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#define arraysize(p) (sizeof(p)/sizeof((p)[0]))

typedef struct _DEVICE_EXTENSION {
	PDEVICE_OBJECT pDevice;
	UNICODE_STRING ustrDeviceName;	//设备名称
	PDEVICE_OBJECT TargetDevice;
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

typedef struct _MyDriver_RW_CONTEXT 
{
    PMDL              NewMdl;			//新MDL
	PMDL              PreviousMdl;		//旧MDL
    ULONG             Length;			//剩下没有读取的字节
    ULONG             Numxfer;			//已经传送过的字节数
    ULONG_PTR         VirtualAddress;	//后续传送的虚拟地址
    PDEVICE_EXTENSION DeviceExtension;	//链接到设备扩展
} MYDRIVER_RW_CONTEXT, * PMYDRIVER_RW_CONTEXT;

// 函数声明

NTSTATUS CreateDevice (IN PDRIVER_OBJECT pDriverObject);
VOID HelloDDKUnload (IN PDRIVER_OBJECT pDriverObject);
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp);
NTSTATUS HelloDDKCreate(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp);
NTSTATUS HelloDDKClose(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) ;
NTSTATUS HelloDDKRead(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) ;

