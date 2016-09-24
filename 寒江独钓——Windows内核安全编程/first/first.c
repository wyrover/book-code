///
/// @file first.c
/// @author crazy_chu
/// @date2008-11-1
/// 

#include <ntddk.h>
	
// 提供一个Unload函数只是为了
VOID DriverUnload(PDRIVER_OBJECT driver)
{
	// 但是实际上我们什么都不做，只打印一句话:
	DbgPrint("first: Our driver is unloading…\r\n");
}

// DriverEntry，入口函数。相当于main。
NTSTATUS DriverEntry(PDRIVER_OBJECT driver, PUNICODE_STRING reg_path)
{
#if DBG
//       _asm int 3
#endif
	// 这是我们的内核模块的入口，可以在这里写入我们想写的东西。
	// 我在这里打印一句话。因为”Hello,world” 常常被高手耻笑，所以
	// 我们打印一点别的。
	DbgPrint("first: Hello, my salary!");

	// 设置一个卸载函数便于这个函数能退出。
	driver->DriverUnload = DriverUnload;
	return STATUS_SUCCESS;
}
