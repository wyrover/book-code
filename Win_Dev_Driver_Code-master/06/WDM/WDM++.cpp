extern "C" {
#include <ntddk.h>
}
	
__forceinline void* operator new(size_t size,
				   POOL_TYPE pool_type,
				   ULONG pool_tag) 
{
	ASSERT((pool_type < MaxPoolType) && (0 != size));
	if(size == 0)return NULL;

	ASSERT(pool_type ==  NonPagedPool || 
		(KeGetCurrentIrql() < DISPATCH_LEVEL));

	return ExAllocatePoolWithTag(pool_type,
		static_cast<ULONG>(size),
		pool_tag);
}

class WDMDrvClass
{
public:
	WDMDrvClass(){};
	NTSTATUS Initialize(PDRIVER_OBJECT Driver);
	void Uninitialze( );

	static NTSTATUS  DispatchFunc_sta(
		DEVICE_OBJECT*  Device,
		PIRP Irp);

	virtual NTSTATUS  DispatchFunc(
		DEVICE_OBJECT*  Device,
		PIRP Irp);

private:
	DEVICE_OBJECT* m_pDevice;
	DRIVER_OBJECT* m_pDriver;
	UNICODE_STRING m_symName;
	UNICODE_STRING m_devName;
};

typedef struct{
	WDMDrvClass* pThis;
	//……
}DEVICE_EXTENSION, *PDEVICE_EXTENSION;

NTSTATUS  WDMDrvClass::DispatchFunc_sta(DEVICE_OBJECT*  Device, PIRP Irp)
{
	PDEVICE_EXTENSION pContext = (PDEVICE_EXTENSION)Device->DeviceExtension;
	WDMDrvClass* pThis = pContext->pThis;
	return pThis-> DispatchFunc(Device, Irp);
}

NTSTATUS  WDMDrvClass::DispatchFunc(DEVICE_OBJECT*  Device, PIRP Irp)
{
    Irp->IoStatus.Status = STATUS_SUCCESS;
    Irp->IoStatus.Information = 0;
    IoCompleteRequest(Irp, IO_NO_INCREMENT);
	return STATUS_SUCCESS;
}

VOID DriverUnload (PDRIVER_OBJECT Driver)
{
	DEVICE_OBJECT* pDev = Driver->DeviceObject;
	PDEVICE_EXTENSION pContext = (PDEVICE_EXTENSION)pDev->DeviceExtension;
	WDMDrvClass* pThis = pContext->pThis;
	pThis-> Uninitialze();
}

VOID WDMDrvClass::Uninitialze()
{
	ASSERT(m_pDriver);
	IoDeleteSymbolicLink(&m_symName);
	IoDeleteDevice(m_pDevice);
}

NTSTATUS  WDMDrvClass::Initialize(PDRIVER_OBJECT Driver)
{
	DEVICE_EXTENSION* pExtension;

	ASSERT(NULL == m_pDriver);
	ASSERT(Driver);

	m_pDriver = Driver;
	m_pDriver->DriverUnload = DriverUnload;
	RtlInitUnicodeString(&m_devName, L"\\Device\\WDM++");
	RtlInitUnicodeString(&m_symName, L"\\??\\WDM++");

	// 设置分发函数，全部指向DispatchFunc_sta
	for(int i = 0; i < IRP_MJ_MAXIMUM_FUNCTION; i++){
		m_pDriver->MajorFunction[i] = DispatchFunc_sta;
	}

	// 创建设备 
	NTSTATUS status = IoCreateDevice( m_pDriver,
		sizeof(DEVICE_EXTENSION),
		&m_devName,
		FILE_DEVICE_UNKNOWN,
		0, TRUE,
		&m_pDevice);
	
	if(!NT_SUCCESS(status))
	{
		return status;
	}

	m_pDevice->Flags = DO_BUFFERED_IO;
	pExtension = (PDEVICE_EXTENSION)m_pDevice->DeviceExtension;
	pExtension->pThis = this;

	// 创建符号链接 
	status = IoCreateSymbolicLink(&m_symName, &m_devName);

	if(!NT_SUCCESS(status))
	{
		IoDeleteDevice(m_pDevice);
		return status;
	}
	
	return STATUS_SUCCESS;
}


extern "C" NTSTATUS DriverEntry(PDRIVER_OBJECT Driver, 
					 PUNICODE_STRING Register)
{
	UNREFERENCED_PARAMETER(Register); 
	
	// 创建动态对象
	WDMDrvClass* pDrv = new(NonPagedPool, 'SAMP') WDMDrvClass();
	return pDrv->Initialize(Driver);
}
