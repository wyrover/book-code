//////////////////////////////////////////////////
// $$root$$.cpp文件


extern "C"
{
	#include <ntddk.h>
}


// 驱动程序加载时调用DriverEntry例程
NTSTATUS DriverEntry(PDRIVER_OBJECT pDriverObj, PUNICODE_STRING pRegistryString)
{

	// 请视情况返回DriverEntry例程执行结果
	return STATUS_DEVICE_CONFIGURATION_ERROR;
}
