/************************************************************************
* 文件名称:HelloWDM.h                                                 
* 作    者:张帆
* 完成日期:2007-11-1
*************************************************************************/

#ifdef __cplusplus
extern "C"
{
#endif
#include <NTDDk.h>
#ifdef __cplusplus
}
#endif 

typedef struct _DEVICE_EXTENSION
{
	PDEVICE_OBJECT fdo;
	PDEVICE_OBJECT NextStackDevice;
	UNICODE_STRING interfaceName;   //设备接口

	PKINTERRUPT InterruptObject;			// address of interrupt object
	PUCHAR portbase;						// IO端口地址
	ULONG nports;							//IO端口地址的数量
	PVOID MemBar0;							//内存基地址0
	ULONG nMem0;							//基地址BAR0占用字节数
	PVOID MemBar1;							//内存基地址1
	ULONG nMem1;							//基地址BAR1占用字节数
	PVOID MemForImage;						//连续物理地址用来dma读取图像
	BOOLEAN mappedport;						//如果为真需要做IO端口映射
} DEVICE_EXTENSION, *PDEVICE_EXTENSION;

#define PAGEDCODE code_seg("PAGE")
#define LOCKEDCODE code_seg()
#define INITCODE code_seg("INIT")

#define PAGEDDATA data_seg("PAGE")
#define LOCKEDDATA data_seg()
#define INITDATA data_seg("INIT")

#define arraysize(p) (sizeof(p)/sizeof((p)[0]))

NTSTATUS HelloWDMAddDevice(IN PDRIVER_OBJECT DriverObject,
                           IN PDEVICE_OBJECT PhysicalDeviceObject);
NTSTATUS HelloWDMPnp(IN PDEVICE_OBJECT fdo,
                        IN PIRP Irp);
NTSTATUS HelloWDMDispatchRoutine(IN PDEVICE_OBJECT fdo,
								 IN PIRP Irp);
void HelloWDMUnload(IN PDRIVER_OBJECT DriverObject);

extern "C"
NTSTATUS DriverEntry(IN PDRIVER_OBJECT DriverObject,
                     IN PUNICODE_STRING RegistryPath);