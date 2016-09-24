///
/// @file		copycat.h
/// @author	crazy_chu
/// @date		2008-6-19
/// 

#ifndef _COMCAP_HEADER_
#define _COMCAP_HEADER_

// 使能函数。
void cppSetEnable(BOOLEAN enable);

// 过滤掉所有的irp。对现有的irp
BOOLEAN ccpIrpFilter(PDEVICE_OBJECT device,PIRP irp,NTSTATUS *status);

// 在irp的时候过滤。如果返回TRUE则表示已经处理过的irp.
BOOLEAN ccpFileIrpFilter(
	PDEVICE_OBJECT next_dev,
	PIRP irp,
	PIO_STACK_LOCATION irpsp,
	NTSTATUS *status);

// 卸载的时候调用。可以解除绑定。
void ccpUnload();

// 这个函数在DriverEntry中调用，可以绑定所有的串口。
void ccpAttachAllComs(PDRIVER_OBJECT driver);

enum {
	CCP_IRP_PASS = 0,
	CCP_IRP_COMPLETED = 1,
	CCP_IRP_GO_ON = 2
};

extern int ccpIrpPreCallback(
	PDEVICE_OBJECT device,
	PDEVICE_OBJECT next_dev,
	PIRP irp,ULONG i,
	PVOID *context);

extern void ccpIrpPostCallback(
	PDEVICE_OBJECT device,
	PDEVICE_OBJECT next_dev,
	PIRP irp,
	PIO_STACK_LOCATION irpsp,
	ULONG i,
	PVOID context);

#endif