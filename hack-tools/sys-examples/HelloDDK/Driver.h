/**
 * Driver.h
 */

#pragma one

#ifdef __cplusplus
extern "C"
{
#endif

#include <ntddk.h>

#ifdef __cplusplus
}
#endif

#define PAGEDCODE  code_seg("PAGE")  // 作用是将此部分代码放入分页内存中运行
#define LOCKEDCODE code_seg()        // 将代码段设置为默认的代码段
#define INITCODE   code_seg("INIT")  // 加载到 INIT 内存区域中，成功加载后，可以退出内存


#define PAGEDDATA  data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA   data_seg("INIT")

#define arrarysize(P)  (sizeof(P) / sizeof((P)[0]))

typedef struct _DEVICE_EXTENSION {
    PDEVICE_OBJECT pDevice;
    UNICODE_STRING ustrDeviceName;   // 设备名称
    UNICODE_STRING ustrSymLinkName;  // 符号链接名
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;


typedef struct _MYDATASTRUCT {
    // List Entry 需要作为_MYDATASTRUCT 结构体的一部分
    LIST_ENTRY ListEntry;

    // 自定义数据结构
    ULONG x;
} MYDATASTRUCT, *PMYDATASTRUCT;

// 函数声明
NTSTATUS CreateDevice(IN PDRIVER_OBJECT pDriverObject);
VOID HelloDDKUnload(IN PDRIVER_OBJECT pDriverObject);
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj, IN PIRP pIrp);