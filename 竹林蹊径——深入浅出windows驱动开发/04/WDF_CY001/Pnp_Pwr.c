/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：Pnp_Pwr.c
	说明：PNP与电源处理
	历史：
	创建：
*/

#include "public.h"
#pragma code_seg("PAGE")

//下面两个Power回调，和WDM中的PnpSetPower类似。
NTSTATUS PwrD0Entry(IN WDFDEVICE  Device, IN WDF_POWER_DEVICE_STATE  PreviousState)
{
	PDEVICE_CONTEXT pContext = GetDeviceContext(Device);

	KDBG(DPFLTR_INFO_LEVEL, "[PwrD0Entry] from %s", PowerName(PreviousState));

	if(PreviousState == PowerDeviceD2)
	{
		SetDigitron(Device, pContext->byPre7Seg);
		SetLEDs(Device, pContext->byPreLEDs);
	}

	CompleteSyncRequest(Device, ENTERD0, PreviousState);

	return STATUS_SUCCESS;
}

// 离开D0状态
NTSTATUS PwrD0Exit(IN WDFDEVICE  Device, IN WDF_POWER_DEVICE_STATE  TargetState)
{
	PDEVICE_CONTEXT pContext = GetDeviceContext(Device);

	KDBG(DPFLTR_INFO_LEVEL, "[PwrD0Exit] to %s", PowerName(TargetState));

	if(TargetState == PowerDeviceD2)
	{		
		GetDigitron(Device, &pContext->byPre7Seg);
		GetLEDs(Device, &pContext->byPreLEDs);
	}

	CompleteSyncRequest(Device, EXITD0, TargetState);

	// 停止中断读操作
	InterruptReadStop(Device);
	ClearSyncQueue(Device);
	return STATUS_SUCCESS;
}

VOID PnpSurpriseRemove(IN WDFDEVICE  Device)
{
	CompleteSyncRequest(Device, SURPRISE_REMOVE, 0);
}

// 配置设备驱动的电源管理功能
NTSTATUS InitPowerManagement(IN WDFDEVICE  Device)
{
	NTSTATUS status = STATUS_SUCCESS;
	WDF_USB_DEVICE_INFORMATION usbInfo;
	PDEVICE_CONTEXT pContext = GetDeviceContext(Device);

	KDBG(DPFLTR_INFO_LEVEL, "[InitPowerManagement]");

	// 获取设备信息
	WDF_USB_DEVICE_INFORMATION_INIT(&usbInfo);
	WdfUsbTargetDeviceRetrieveInformation(pContext->UsbDevice, &usbInfo);

	// USB设备信息以掩码形式被保存在Traits中。
	KDBG( DPFLTR_INFO_LEVEL, "Device self powered: %s", 
		usbInfo.Traits & WDF_USB_DEVICE_TRAIT_SELF_POWERED ? "TRUE" : "FALSE");
	KDBG( DPFLTR_INFO_LEVEL, "Device remote wake capable: %s",
		usbInfo.Traits & WDF_USB_DEVICE_TRAIT_REMOTE_WAKE_CAPABLE ? "TRUE" : "FALSE");
	KDBG( DPFLTR_INFO_LEVEL, "Device high speed: %s",
		usbInfo.Traits & WDF_USB_DEVICE_TRAIT_AT_HIGH_SPEED ? "TRUE" : "FALSE");

	// 设置设备的休眠和远程唤醒功能
	if(usbInfo.Traits & WDF_USB_DEVICE_TRAIT_REMOTE_WAKE_CAPABLE)
	{
		WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS idleSettings;
		WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS wakeSettings;

		// 设置设备为闲时休眠。闲时超过10S，自动进入休眠状态。
		WDF_DEVICE_POWER_POLICY_IDLE_SETTINGS_INIT(&idleSettings, IdleUsbSelectiveSuspend);
		idleSettings.IdleTimeout = 10000;
		status = WdfDeviceAssignS0IdleSettings(Device, &idleSettings);
		if(!NT_SUCCESS(status))
		{
			KDBG( DPFLTR_ERROR_LEVEL, "WdfDeviceAssignS0IdleSettings failed with status 0x%0.8x!!!", status);
			return status;
		}

		// 设置为可远程唤醒。包含设备自身醒来，已经当PC系统进入休眠后，设备可以将系统唤醒，两个方面。
		WDF_DEVICE_POWER_POLICY_WAKE_SETTINGS_INIT(&wakeSettings);
		status = WdfDeviceAssignSxWakeSettings(Device, &wakeSettings);
		if(!NT_SUCCESS(status))
		{
			KDBG( DPFLTR_ERROR_LEVEL, "WdfDeviceAssignSxWakeSettings failed with status 0x%0.8x!!!", status);
			return status;
		}
	}

	return status;
}

char* PowerName(WDF_POWER_DEVICE_STATE PowerState)
{
	char *name;

	switch(PowerState)
	{
	case WdfPowerDeviceInvalid :	
		name = "PowerDeviceUnspecified"; 
		break;
	case WdfPowerDeviceD0:			
		name = "WdfPowerDeviceD0"; 
		break;
	case WdfPowerDeviceD1:			
		name = "WdfPowerDeviceD1"; 
		break;
	case WdfPowerDeviceD2:		
		name = "WdfPowerDeviceD2"; 
		break;
	case WdfPowerDeviceD3:		
		name = "WdfPowerDeviceD3"; 
		break;
	case WdfPowerDeviceD3Final:	
		name = "WdfPowerDeviceD3Final";
		break;
	case WdfPowerDevicePrepareForHibernation:									
		name = "WdfPowerDevicePrepareForHibernation"; 
		break;
	case WdfPowerDeviceMaximum:		
		name = "WdfPowerDeviceMaximum";
		break;
	default:					
		name = "Unknown Power State";
		break;
	}

	return name;
}