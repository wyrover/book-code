/*
    版权：CY001 WDF USB开发板项目  2009/9/1

    你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
    如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

    作者：张佩 
    联系：zhang.mibox@gmail.com

    日期：2009/9/1

    文件：DrvClass.h
    说明：驱动类定义
*/

#pragma once

#include "public.h"

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

    //==========================
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
    virtual void 
        InitDefQueue(
        OUT WDF_IO_QUEUE_CONFIG& config
        );

    virtual NTSTATUS
        CreateWDFQueues();

    virtual NTSTATUS
        ConfigureUsbDevice();

    virtual NTSTATUS
        GetUsbPipes();

    //=============================
    static DrvClass* GetDrvClassFromDriver(WDFDRIVER Driver);
    static DrvClass* GetDrvClassFromDevice(WDFDEVICE Device);

};