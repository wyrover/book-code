#pragma once

#ifdef __cplusplus
extern "C"
{
#endif
#include <ntifs.h>
#ifdef __cplusplus
}
#endif 

// IOCTLs

// 开启进程监控
#define IOCTL_PROCESS_MONITORDRIVER_ON CTL_CODE(\
			FILE_DEVICE_UNKNOWN, \
			0x820, \
			METHOD_BUFFERED, \
			FILE_ANY_ACCESS)

// 关闭进程监控
#define IOCTL_PROCESS_MONITORDRIVER_OFF CTL_CODE(\
	       FILE_DEVICE_UNKNOWN, \
	       0x821, \
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

extern "C" PSSDT KeServiceDescriptorTable;  // KeServiceDescriptorTable全局变量，外援

BOOLEAN pProcessMonitor= FALSE;   //是否开启进程监控，默认为未开启
char *chInteractionBuffer;	//驱动与用户的交互缓冲区
ULONG ulIndex;	//ZwCreateProcessEx在SSDT中的索引值
KEVENT event ; 
ULONG ulProcessNameOffset;
ULONG ulProcessManager = 0;

#define BOOL unsigned long
#define PROCNAMELEN     50
#define MAXPATHLEN 256
#define NT_PROCNAMELEN  50

NTSTATUS FakedZwCreateProcess(
							  OUT PHANDLE ProcessHandle,
							  IN ACCESS_MASK DesiredAccess,
							  IN POBJECT_ATTRIBUTES ObjectAttributes,
							  IN HANDLE InheritFromProcessHandle,
							  IN BOOLEAN InheritHandles,
							  IN HANDLE SectionHandle OPTIONAL,
							  IN HANDLE DebugPort OPTIONAL,
							  IN HANDLE ExceptionPort OPTIONAL,
							  IN HANDLE Unknown 
							  );

typedef NTSTATUS (*ZWCREATEPROCESSEX)(
									OUT PHANDLE ProcessHandle,
									IN ACCESS_MASK DesiredAccess,
									IN POBJECT_ATTRIBUTES ObjectAttributes,
									IN HANDLE InheritFromProcessHandle,
									IN BOOLEAN InheritHandles,
									IN HANDLE SectionHandle OPTIONAL,
									IN HANDLE DebugPort OPTIONAL,
									IN HANDLE ExceptionPort OPTIONAL,
									IN HANDLE Unknown 
									);

ZWCREATEPROCESSEX RealZwCreateProcess;