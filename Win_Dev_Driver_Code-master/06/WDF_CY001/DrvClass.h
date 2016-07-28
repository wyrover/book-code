
#pragma once

extern "C"{
#include "public.h"
}

class DrvClass{
protected:
	int m_nSize;
	WDFDRIVER m_hDriver;
	WDFDEVICE m_hDevice;
	WDFUSBDEVICE m_hUsbDevice;
	WDFUSBINTERFACE   m_hUsbInterface;
	WDFUSBINTERFACE   m_hMulInterfaces[MAX_INTERFACES];

	PDRIVER_CONTEXT m_pDriverContext;
	PDEVICE_CONTEXT m_pDeviceContext;

	WDFQUEUE m_hDefQueue;

	USB_BUS_INTERFACE_USBDI_V1 m_busInterface;// 总线接口
	BOOLEAN m_bIsDeviceHighSpeed;

public:	
	DrvClass(){m_nSize = sizeof(DrvClass);KdPrint(("DrvClass"));}
	int GetSize(){return m_nSize;}

	virtual NTSTATUS DriverEntry(IN PDRIVER_OBJECT  DriverObject, IN PUNICODE_STRING  RegistryPath);

	static NTSTATUS PnpAdd_sta(IN WDFDRIVER  Driver, IN PWDFDEVICE_INIT  DeviceInit);
	virtual NTSTATUS PnpAdd(IN PWDFDEVICE_INIT  DeviceInit);

	virtual void InitPnpPwrEvents(WDF_PNPPOWER_EVENT_CALLBACKS*);
	virtual void InitPnpCap(WDF_DEVICE_PNP_CAPABILITIES*);	

	static NTSTATUS
		PnpPrepareHardware_sta(
		IN WDFDEVICE    Device,
		IN WDFCMRESLIST ResourceList,
		IN WDFCMRESLIST ResourceListTranslated
		);
	
	virtual NTSTATUS
		PnpPrepareHardware(
		IN WDFCMRESLIST ResourceList,
		IN WDFCMRESLIST ResourceListTranslated
		);
	
	static NTSTATUS
		PnpReleaseHardware_sta(
		IN WDFDEVICE    Device,
		IN WDFCMRESLIST ResourceList
		);

	virtual NTSTATUS
		PnpReleaseHardware(
		IN WDFCMRESLIST ResourceList
		);

	static VOID 
		PnpSurpriseRemove_sta(
		IN WDFDEVICE  Device
		);

	virtual VOID 
		PnpSurpriseRemove();

	virtual NTSTATUS InitPowerManagement();

	static NTSTATUS 
		PwrD0Entry_sta(
		IN WDFDEVICE  Device, 
		IN WDF_POWER_DEVICE_STATE  PreviousState
		);

	virtual NTSTATUS 
		PwrD0Entry(
		IN WDF_POWER_DEVICE_STATE  PreviousState
		);

	static NTSTATUS 
		PwrD0Exit_sta(
		IN WDFDEVICE  Device, 
		IN WDF_POWER_DEVICE_STATE  TargetState
		);

	virtual NTSTATUS 
		PwrD0Exit(
		IN WDF_POWER_DEVICE_STATE  TargetState
		);

	static VOID
		EventDefault_sta(
		IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request
		);

	virtual VOID
		EventDefault(
		IN WDFQUEUE  Queue,
		IN WDFREQUEST  Request
		);

protected:
	virtual NTSTATUS
		CreateWDFQueues();

	virtual NTSTATUS
		ConfigureUsbDevice();

	virtual NTSTATUS
		GetUsbPipes();

	static DrvClass* GetDrvClassFromDriver(WDFDRIVER Driver);
	static DrvClass* GetDrvClassFromDevice(WDFDEVICE Device);

};