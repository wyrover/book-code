#include "precomp.h"
#include "ntstrsafe.h"
PDEVICE_OBJECT gDeviceObject = NULL;

// 这是我们自己处理IRP_MJ_CREATE的方法
NTSTATUS 
myCreate(
		 IN PDEVICE_OBJECT DeviceObject, 
		 IN PIRP Irp
		 )
{
	// 如果创建我们的设备，直接返回成功
	if(DeviceObject == gDeviceObject)
		return STATUS_SUCCESS;

	// 不要忘了系统过程
	return systemCreate(DeviceObject, Irp);
}


// 这是我们自己处理IRP_MJ_WRITE的方法
NTSTATUS 
myWrite(
		IN PDEVICE_OBJECT DeviceObject, 
		IN PIRP Irp
		)
{
	// 判断是不是我们的设备
	if(DeviceObject == gDeviceObject){
		// 
		NTSTATUS status = STATUS_SUCCESS;

		//
		// 这里做处理，要对IRP中的数据进行相应的保存、解析
		//

		// 直接返回
		return status;
	}

	// 不要忘了系统过程
	return systemWrite(DeviceObject, Irp);
}


// 这是我们自己处理IRP_MJ_READ的方法
NTSTATUS 
myRead(
	   IN PDEVICE_OBJECT DeviceObject, 
	   IN PIRP Irp
	   )
{
	// 判断是不是我们的设备
	if(DeviceObject == gDeviceObject){
		// 
		NTSTATUS status = STATUS_SUCCESS;

		//
		// 这里做处理，要把一些数据填充到IRP中的数据缓冲区中
		//

		// 直接返回
		return status;
	}

	// 不要忘了系统过程
	return systemRead(DeviceObject, Irp);
}


// 这是我们自己处理IRP_MJ_DEVICE_CONTROL的方法
NTSTATUS 
myDeviceControl(
				IN PDEVICE_OBJECT DeviceObject, 
				IN PIRP Irp
				)
{
	// 判断是不是我们的设备
	if(DeviceObject == gDeviceObject){
		// 
		NTSTATUS status = STATUS_SUCCESS;

		//
		// 这里做处理，要获取Device IO Control号，处理它
		//

		// 直接返回
		return status;
	}

	// 不要忘了系统过程
	return systemDeviceControl(DeviceObject, Irp);
}

// 
// 实现我们自己的AddDevice函数
//
NTSTATUS myAddDevice(
					 IN PDRIVER_OBJECT  DriverObject,
					 IN PDEVICE_OBJECT  PhysicalDeviceObject 
					 )
{
	if(gDeviceObject != NULL)
	{
		// 在这里面创建我们自己的设备对象，或者申请所需要的资源。
		// 为了区分不同实例，将设备对象名构造成：”MyNdisDevice”+HardwareID。
		UNICODE_STRING nameString; 
		WCHAR wcsName[256];
		UNICODE_STRING preName = RTL_CONSTANT_STRING(L"\\Device\\MyNdisDevice");

		// 首先取得设备的HDID。
		ULONG nameLength = 0;
		WCHAR wcsHardwareID[256]; //足够大了
		NTSTATUS status = IoGetDeviceProperty (PhysicalDeviceObject,
			DevicePropertyHardwareID,
			256,
			wcsHardwareID,
			&nameLength);
		if(status != STATUS_SUCCESS){
			KdPrint(("Failed to get hardware ID %x\n", status));
			return status;
		}

		// 下面构造设备对象的名字，根据上面的规则：“MyNdisDevice”+ HardwareID。
		RtlInitEmptyUnicodeString( &nameString, wcsName, 256*2);
		RtlCopyUnicodeString( &nameString, &preName);
		//RtlUnicodeStringPrintf(&nameString, L"%wZ_%d_", &preName, 0);
		RtlAppendUnicodeToString( &nameString, wcsHardwareID);

		status = IoCreateDevice(DriverObject,
			0,
			&nameString,
			FILE_DEVICE_UNKNOWN,
			FILE_DEVICE_SECURE_OPEN,
			FALSE,
			&gDeviceObject); 

		// 如果创建失败了，我们有权利让函数以失败返回
		// 但这样我们的驱动加载也就失败了
		if(status != STATUS_SUCCESS){
			KdPrint(("Failed to create device %ws\n", nameString));
			return status;
		}
	}

	//ExAllocatePoolWithTag(); //申请资源及其他

	// 
	// 还可以加入其他正确的操作
	//

	// 现在调用保存的Ndis库中的AddDevice实现
	// 千万不要忘记，否则就会大错特错了
	return systemAddDevice(DriverObject, PhysicalDeviceObject);
}
