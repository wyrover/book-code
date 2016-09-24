///
/// @file         sflt_smpl.c
/// @author    crazy_chu
/// @date       2009-2-20
/// @brief      使用sfilter.lib的一个例子。 
/// 
/// 免责声明
/// 本代码为示例代码。未经详尽测试，不保证可靠性。作者对
/// 任何人使用此代码导致的直接和间接损失不负责任。
/// 

#include <ntifs.h>
#include "..\inc\sfilter\sfilter.h"

SF_RET OnSfilterIrpPre(
		IN PDEVICE_OBJECT dev,
		IN PDEVICE_OBJECT next_dev,
		IN PVOID extension,
		IN PIRP irp,
		OUT NTSTATUS *status,
		PVOID *context)
{
    // 获得当前调用栈
	PIO_STACK_LOCATION irpsp = IoGetCurrentIrpStackLocation(irp);
    PFILE_OBJECT file = irpsp->FileObject;

	// 我仅仅过滤文件请求。 FileObject不存在的情况一律passthru.
	if(file == NULL)
		return SF_IRP_PASS;

    KdPrint(("IRP: file name = %wZ major function = %x\r\n",&file->FileName));
    return SF_IRP_PASS;
}

VOID OnSfilterIrpPost(
		IN PDEVICE_OBJECT dev,
		IN PDEVICE_OBJECT next_dev,
		IN PVOID extension,
		IN PIRP irp,
		IN NTSTATUS status,
		PVOID context)
{
    // 什么都不用做
}

NTSTATUS OnSfilterDriverEntry(
    IN PDRIVER_OBJECT DriverObject,
    IN PUNICODE_STRING RegistryPath,
	OUT PUNICODE_STRING userNameString,
	OUT PUNICODE_STRING syblnkString,
	OUT PULONG extensionSize)
{
 	UNICODE_STRING user_name,syb_name;
	NTSTATUS status = STATUS_SUCCESS;

  	// 确定控制设备的名字和符号链接。
	RtlInitUnicodeString(&user_name,L"sflt_smpl_cdo");
	RtlInitUnicodeString(&syb_name,L"sflt_smpl_cdo_syb");
	RtlCopyUnicodeString(userNameString,&user_name);
	RtlCopyUnicodeString(syblnkString,&syb_name);

	// 设置控制设备为所有用户可用
	sfilterSetCdoAccessForAll();

    return STATUS_SUCCESS;
}

VOID OnSfilterDriverUnload()
{
    // 没什么要做的...;
}

NTSTATUS OnSfilterCDODispatch(
		IN PDEVICE_OBJECT DeviceObject,
		IN PIRP Irp)
{
    return STATUS_UNSUCCESSFUL;
}

BOOLEAN OnSfilterAttachPre(
		IN PDEVICE_OBJECT ourDevice,
		IN PDEVICE_OBJECT theDeviceToAttach,
		IN PUNICODE_STRING DeviceName,
		IN PVOID extension)
{
    // 直接返回TRUE，所有设备都绑定
    return TRUE;
}

VOID OnSfilterAttachPost(
		IN PDEVICE_OBJECT ourDevice,
		IN PDEVICE_OBJECT theDeviceToAttach,
		IN PDEVICE_OBJECT theDeviceToAttached,
		IN PVOID extension,
		IN NTSTATUS status)
{
    // 不需要做什么。
}
