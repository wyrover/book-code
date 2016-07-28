/*
	版权：CY001 WDF USB开发板项目  2009/9/1
	
	你所拥有的此份代码拷贝，仅可用于个人学习，任何商业或与利益相关途径的使用，都不被允许。
	如果你未经特许，请不要将代码在网络上传播。项目开发者已将代码发布，并长期维护。

	作者：麦盒数码		张佩 
		  驱动开发网	马勇
		  AMD			夏浩翔

	日期：2009/9/1

	文件：Main.c
	说明：入口函数
	历史：
	创建：
*/
#ifdef __cplusplus
extern "C"{
#endif

#include "public.h"
#pragma alloc_text(INIT, DriverEntry)

#ifdef __cplusplus
}
#endif

#include "NewDelete.h"
#include "DrvClass.h"


VOID DrvClassDestroy(IN WDFOBJECT  Object)
{
	PVOID pBuf = WdfMemoryGetBuffer((WDFMEMORY)Object, NULL);
	delete pBuf;
}

NTSTATUS DrvClass::DriverEntry(IN PDRIVER_OBJECT DriverObject, IN PUNICODE_STRING RegistryPath)
{
	KDBG(DPFLTR_INFO_LEVEL, "[DrvClass::DriverEntry] this = %p", this);

	WDF_OBJECT_ATTRIBUTES attributes;
	WDF_DRIVER_CONFIG config;

	// 设定设备环境块长度
	// 宏内部会调用sizeof(DEVICE_CONTEXT)求结构体长度
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DRIVER_CONTEXT);

	// 创建WDF框架驱动对象。这个对象我们永远用不到，也不被暴露。但他确实存在着。
	// 我们可以把它看成一只隐藏着的手吧。
	// DriverEntry里面的这种初始化工作，和所有的小端口驱动一模一样，简直别无二致。
	// 读者可以据此参看诸如Scasi、文件过滤、NDIS、StreamPort等类型的小端口驱动，是如何运作的。
	WDF_DRIVER_CONFIG_INIT(&config, DrvClass::PnpAdd_sta);

	NTSTATUS status = WdfDriverCreate(DriverObject, // WDF驱动对象
		RegistryPath,
		&attributes,
		&config, // 配置参数
		&m_hDriver);

	if(NT_SUCCESS(status))
	{
		m_pDriverContext = GetDriverContext(m_hDriver);
		ASSERT(m_pDriverContext);
		m_pDriverContext->par1 = (PVOID)this;	

		WDFMEMORY hMemDrv;
		WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
		attributes.ParentObject = m_hDriver;
		attributes.EvtDestroyCallback  = DrvClassDestroy;
		WdfMemoryCreatePreallocated(&attributes, (PVOID)this, GetSize(), &hMemDrv);
	}

	return status;
}

DrvClass* GetDrvClass();

extern "C" NTSTATUS DriverEntry(
    IN PDRIVER_OBJECT  DriverObject, 
    IN PUNICODE_STRING  RegistryPath
    )
{
	DrvClass* myDriver = GetDrvClass();//new(NonPagedPool, 'CY01')DrvClass();
	if(myDriver == NULL)return STATUS_UNSUCCESSFUL;
	return myDriver->DriverEntry(DriverObject, RegistryPath);
}
