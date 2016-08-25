/************************************************************************
* 文件名称:Driver.cpp                                                 
* 作    者:张帆
* 完成日期:2007-11-1
*************************************************************************/

#include "Driver.h"


#define  MY_REG_SOFTWARE_KEY_NAME		L"\\Registry\\Machine\\Software\\Zhangfan"
#pragma INITCODE
VOID CreateRegTest() 
{
	//创建或打开某注册表项目
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感 
							NULL, 
							NULL );
	ULONG ulResult;
	//创建或带开注册表项目
	NTSTATUS ntStatus = ZwCreateKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes,
							0,
							NULL,
							REG_OPTION_NON_VOLATILE,
							&ulResult);

	if (NT_SUCCESS(ntStatus))
	{
		//判断是被新创建，还是已经被创建
		if(ulResult==REG_CREATED_NEW_KEY)
		{
			KdPrint(("The register item is created\n"));
		}else if(ulResult==REG_OPENED_EXISTING_KEY)
		{
			KdPrint(("The register item has been created,and now is opened\n"));
		}
	}

	//(2)创建或打开某注册表项目的子项
	UNICODE_STRING subRegUnicodeString;
	HANDLE hSubRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &subRegUnicodeString, 
		L"SubItem");

	OBJECT_ATTRIBUTES subObjectAttributes;
	//初始化subObjectAttributes
	InitializeObjectAttributes(&subObjectAttributes,
							&subRegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感 
							hRegister, 
							NULL );
	//创建或带开注册表项目
	ntStatus = ZwCreateKey( &hSubRegister,
							KEY_ALL_ACCESS,
							&subObjectAttributes,
							0,
							NULL,
							REG_OPTION_NON_VOLATILE,
							&ulResult);

	if (NT_SUCCESS(ntStatus))
	{
		//判断是被新创建，还是已经被创建
		if(ulResult==REG_CREATED_NEW_KEY)
		{
			KdPrint(("The sub register item is created\n"));
		}else if(ulResult==REG_OPENED_EXISTING_KEY)
		{
			KdPrint(("The sub register item has been created,and now is opened\n"));
		}
	}

	//关闭注册表句柄
	ZwClose(hRegister);
	ZwClose(hSubRegister);
}

#pragma INITCODE
VOID OpenRegTest()
{
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感
							NULL, 
							NULL );
	//打开注册表
	NTSTATUS ntStatus = ZwOpenKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Open register successfully\n"));
	}

	ZwClose(hRegister);
}

#pragma INITCODE
VOID DeleteItemRegTest()
{
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

#define MY_REG_SOFTWARE_KEY_NAME1 L"\\Registry\\Machine\\Software\\Zhangfan\\SubItem"
	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME1);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感
							NULL, 
							NULL );
	//打开注册表
	NTSTATUS ntStatus = ZwOpenKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Open register successfully\n"));
	}

	ntStatus = ZwDeleteKey(hRegister);
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Delete the item successfully\n"));
	}else if(ntStatus == STATUS_ACCESS_DENIED)
	{
		KdPrint(("STATUS_ACCESS_DENIED\n"));

	}else if(ntStatus == STATUS_INVALID_HANDLE)
	{
		KdPrint(("STATUS_INVALID_HANDLE\n"));
	}else
	{
		KdPrint(("Maybe the item has sub item to delete\n"));
	}

	ZwClose(hRegister);
}


#pragma INITCODE
VOID SetRegTest()
{
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感
							NULL, 
							NULL );
	//打开注册表
	NTSTATUS ntStatus = ZwOpenKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Open register successfully\n"));
	}

	UNICODE_STRING ValueName;
	//初始化ValueName
	RtlInitUnicodeString( &ValueName, L"REG_DWORD value");

	//设置REG_DWORD子键
	ULONG ulValue = 1000;
	ZwSetValueKey(hRegister,
				&ValueName,
				0,
				REG_DWORD,
				&ulValue,
				sizeof(ulValue));

	//初始化ValueName
	RtlInitUnicodeString( &ValueName, L"REG_SZ value");
	WCHAR* strValue = L"hello world";

	//设置REG_SZ子键
	ZwSetValueKey(hRegister,
				&ValueName,
				0,
				REG_SZ,
				strValue,
				wcslen(strValue)*2+2);


	//初始化ValueName
	RtlInitUnicodeString( &ValueName, L"REG_BINARY value");
	
	UCHAR buffer[10];
	RtlFillMemory(buffer,sizeof(buffer),0xFF);

	//设置REG_MULTI_SZ子键
	ZwSetValueKey(hRegister,
				&ValueName,
				0,
				REG_BINARY,
				buffer,
				sizeof(buffer));

	//关闭注册表句柄
	ZwClose(hRegister);
}

#pragma INITCODE
VOID QueryRegTest()
{
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感
							NULL, 
							NULL );
	//打开注册表
	NTSTATUS ntStatus = ZwOpenKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Open register successfully\n"));
	}

	UNICODE_STRING ValueName;
	//初始化ValueName
	RtlInitUnicodeString( &ValueName, L"REG_DWORD value");

	//读取REG_DWORD子键
	ULONG ulSize;
	ntStatus = ZwQueryValueKey(hRegister,
				&ValueName,
				KeyValuePartialInformation ,
				NULL,
				0,
				&ulSize);

	if (ntStatus==STATUS_OBJECT_NAME_NOT_FOUND || ulSize==0)
	{
		ZwClose(hRegister);
		KdPrint(("The item is not exist\n"));
		return;
	}
	PKEY_VALUE_PARTIAL_INFORMATION pvpi = 
		(PKEY_VALUE_PARTIAL_INFORMATION)
		ExAllocatePool(PagedPool,ulSize);

	ntStatus = ZwQueryValueKey(hRegister,
				&ValueName,
				KeyValuePartialInformation ,
				pvpi,
				ulSize,
				&ulSize);
	if (!NT_SUCCESS(ntStatus))
	{
		ZwClose(hRegister);
		KdPrint(("Read regsiter error\n"));
		return;
	}
	//判断是否为REG_DWORD类型
	if (pvpi->Type==REG_DWORD && pvpi->DataLength==sizeof(ULONG))
	{
		PULONG pulValue = (PULONG) pvpi->Data;
		KdPrint(("The value:%d\n",*pulValue));
	}

	ExFreePool(pvpi);

	//初始化ValueName
	RtlInitUnicodeString( &ValueName, L"REG_SZ value");
	//读取REG_SZ子键
	ntStatus = ZwQueryValueKey(hRegister,
				&ValueName,
				KeyValuePartialInformation ,
				NULL,
				0,
				&ulSize);

	if (ntStatus==STATUS_OBJECT_NAME_NOT_FOUND || ulSize==0)
	{
		ZwClose(hRegister);
		KdPrint(("The item is not exist\n"));
		return;
	}
	pvpi = 
		(PKEY_VALUE_PARTIAL_INFORMATION)
		ExAllocatePool(PagedPool,ulSize);

	ntStatus = ZwQueryValueKey(hRegister,
				&ValueName,
				KeyValuePartialInformation ,
				pvpi,
				ulSize,
				&ulSize);
	if (!NT_SUCCESS(ntStatus))
	{
		ZwClose(hRegister);
		KdPrint(("Read regsiter error\n"));
		return;
	}
	//判断是否为REG_SZ类型
	if (pvpi->Type==REG_SZ)
	{
		KdPrint(("The value:%S\n",pvpi->Data));
	}

	ZwClose(hRegister);
}

#pragma INITCODE
VOID EnumerateSubItemRegTest()
{
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感
							NULL, 
							NULL );
	//打开注册表
	NTSTATUS ntStatus = ZwOpenKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Open register successfully\n"));
	}

	ULONG ulSize;
	//第一次调用ZwQueryKey为了获取KEY_FULL_INFORMATION数据的长度
	ZwQueryKey(hRegister,
		KeyFullInformation,
		NULL,
		0,
		&ulSize);

	PKEY_FULL_INFORMATION pfi = 
		(PKEY_FULL_INFORMATION)
		ExAllocatePool(PagedPool,ulSize);

	//第二次调用ZwQueryKey为了获取KEY_FULL_INFORMATION数据的数据
	ZwQueryKey(hRegister,
		KeyFullInformation,
		pfi,
		ulSize,
		&ulSize);

	for (ULONG i=0;i<pfi->SubKeys;i++)
	{
		//第一次调用ZwEnumerateKey为了获取KEY_BASIC_INFORMATION数据的长度
		ZwEnumerateKey(hRegister,
				i,
				KeyBasicInformation,
				NULL,
				0,
				&ulSize);

		PKEY_BASIC_INFORMATION pbi =
			(PKEY_BASIC_INFORMATION)
			ExAllocatePool(PagedPool,ulSize);

		//第二次调用ZwEnumerateKey为了获取KEY_BASIC_INFORMATION数据的数据
		ZwEnumerateKey(hRegister,
				i,
				KeyBasicInformation,
				pbi,
				ulSize,
				&ulSize);

		UNICODE_STRING uniKeyName;
		uniKeyName.Length = 
		uniKeyName.MaximumLength =
		(USHORT)pbi->NameLength;

		uniKeyName.Buffer = pbi->Name;

		KdPrint(("The %d sub item name:%wZ\n",i,&uniKeyName));

		ExFreePool(pbi);
	}

	ExFreePool(pfi);
	ZwClose(hRegister);
}

#pragma INITCODE
VOID EnumerateSubValueRegTest()
{
	UNICODE_STRING RegUnicodeString;
	HANDLE hRegister;

	//初始化UNICODE_STRING字符串
	RtlInitUnicodeString( &RegUnicodeString, 
		MY_REG_SOFTWARE_KEY_NAME);
	
	OBJECT_ATTRIBUTES objectAttributes;
	//初始化objectAttributes
	InitializeObjectAttributes(&objectAttributes,
							&RegUnicodeString,
							OBJ_CASE_INSENSITIVE,//对大小写敏感
							NULL, 
							NULL );
	//打开注册表
	NTSTATUS ntStatus = ZwOpenKey( &hRegister,
							KEY_ALL_ACCESS,
							&objectAttributes);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Open register successfully\n"));
	}

	ULONG ulSize;
	ZwQueryKey(hRegister,
		KeyFullInformation,
		NULL,
		0,
		&ulSize);

	PKEY_FULL_INFORMATION pfi = 
		(PKEY_FULL_INFORMATION)
		ExAllocatePool(PagedPool,ulSize);

	ZwQueryKey(hRegister,
		KeyFullInformation,
		pfi,
		ulSize,
		&ulSize);

	for (ULONG i=0;i<pfi->Values;i++)
	{
		ZwEnumerateValueKey(hRegister,
				i,
				KeyValueBasicInformation,
				NULL,
				0,
				&ulSize);

		PKEY_VALUE_BASIC_INFORMATION pvbi =
			(PKEY_VALUE_BASIC_INFORMATION)
			ExAllocatePool(PagedPool,ulSize);

		ZwEnumerateValueKey(hRegister,
				i,
				KeyValueBasicInformation,
				pvbi,
				ulSize,
				&ulSize);

		UNICODE_STRING uniKeyName;
		uniKeyName.Length = 
		uniKeyName.MaximumLength =
		(USHORT)pvbi->NameLength;

		uniKeyName.Buffer = pvbi->Name;

		KdPrint(("The %d sub value name:%wZ\n",i,&uniKeyName));

		if (pvbi->Type==REG_SZ)
		{
			KdPrint(("The sub value type:REG_SZ\n"));
		}else if (pvbi->Type==REG_MULTI_SZ)
		{
			KdPrint(("The sub value type:REG_MULTI_SZ\n")); 

		}else if (pvbi->Type==REG_DWORD)
		{
			KdPrint(("The sub value type:REG_DWORD\n")); 
		}else if (pvbi->Type==REG_BINARY)
		{
			KdPrint(("The sub value type:REG_BINARY\n")); 
		}

		ExFreePool(pvbi);
	}

	ExFreePool(pfi);
	ZwClose(hRegister);
}

#pragma INITCODE
void RtlRegTest()
{
	//创建子项目
	NTSTATUS ntStatus =
		RtlCreateRegistryKey(RTL_REGISTRY_SERVICES,L"HelloDDK\\Zhangfan");
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Create the item successfully\n"));
	}

	//检查某项是否存在
	ntStatus =
		RtlCheckRegistryKey(RTL_REGISTRY_SERVICES,L"HelloDDK\\Zhangfan");
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("The item is exist\n"));
	}

	//写入REG_DWORD的数据
	ULONG value1 = 100;
	ntStatus = 
		RtlWriteRegistryValue(RTL_REGISTRY_SERVICES,
							L"HelloDDK\\Zhangfan",
							L"DWORD_Value",
							REG_DWORD,
							&value1,
							sizeof(value1));
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Write the DWORD value succuessfully\n"));
	}

	PWCHAR szString = L"Hello DDK";
	ntStatus = 
		RtlWriteRegistryValue(RTL_REGISTRY_SERVICES,
							L"HelloDDK\\Zhangfan",
							L"SZ_Value",
							REG_SZ,
							szString,
							wcslen(szString)*2+2);

	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Write the REG_SZ value succuessfully\n"));
	}

    RTL_QUERY_REGISTRY_TABLE paramTable[2];
    RtlZeroMemory(paramTable, sizeof(paramTable));

	ULONG defaultData=0;
	ULONG uQueryValue;
    paramTable[0].Flags = RTL_QUERY_REGISTRY_DIRECT;
    paramTable[0].Name = L"DWORD_Value";
    paramTable[0].EntryContext = &uQueryValue;
    paramTable[0].DefaultType = REG_DWORD;
    paramTable[0].DefaultData = &defaultData;
    paramTable[0].DefaultLength = sizeof(ULONG);

	//查询REG_DWORD的数据
	ntStatus = RtlQueryRegistryValues(RTL_REGISTRY_SERVICES,
						L"HelloDDK\\Zhangfan",
						paramTable,
						NULL,
						NULL);
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("Query the item successfully\n"));
		KdPrint(("The item is :%d\n",uQueryValue));
	}
	
	//删除子键
	ntStatus = RtlDeleteRegistryValue(RTL_REGISTRY_SERVICES,
					L"HelloDDK\\Zhangfan",
					L"DWORD_Value");
	if (NT_SUCCESS(ntStatus))
	{
		KdPrint(("delete the value successfully\n"));
	}
}

#pragma INITCODE
VOID RegTest() 
{
 	CreateRegTest();
// 	OpenRegTest();
// 	SetRegTest();
//	QueryRegTest();
//	EnumerateSubItemRegTest();
//	EnumerateSubValueRegTest();
	DeleteItemRegTest();
//	RtlRegTest();
}

/************************************************************************
* 函数名称:DriverEntry
* 功能描述:初始化驱动程序，定位和申请硬件资源，创建内核对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
      pRegistryPath:驱动程序在注册表的中的路径
* 返回 值:返回初始化驱动状态
*************************************************************************/
#pragma INITCODE
extern "C" NTSTATUS DriverEntry (
			IN PDRIVER_OBJECT pDriverObject,
			IN PUNICODE_STRING pRegistryPath	) 
{
	NTSTATUS status;
	KdPrint(("Enter DriverEntry\n"));

	//注册其他驱动调用函数入口
	pDriverObject->DriverUnload = HelloDDKUnload;
	pDriverObject->MajorFunction[IRP_MJ_CREATE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_CLOSE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_WRITE] = HelloDDKDispatchRoutine;
	pDriverObject->MajorFunction[IRP_MJ_READ] = HelloDDKDispatchRoutine;
	
	//创建驱动设备对象
	status = CreateDevice(pDriverObject);

	RegTest();

	KdPrint(("DriverEntry end\n"));
	return status;
}

/************************************************************************
* 函数名称:CreateDevice
* 功能描述:初始化设备对象
* 参数列表:
      pDriverObject:从I/O管理器中传进来的驱动对象
* 返回 值:返回初始化状态
*************************************************************************/
#pragma INITCODE
NTSTATUS CreateDevice (
		IN PDRIVER_OBJECT	pDriverObject) 
{
	NTSTATUS status;
	PDEVICE_OBJECT pDevObj;
	PDEVICE_EXTENSION pDevExt;
	
	//创建设备名称
	UNICODE_STRING devName;
	RtlInitUnicodeString(&devName,L"\\Device\\MyDDKDevice");
	
	//创建设备
	status = IoCreateDevice( pDriverObject,
						sizeof(DEVICE_EXTENSION),
						&(UNICODE_STRING)devName,
						FILE_DEVICE_UNKNOWN,
						0, TRUE,
						&pDevObj );
	if (!NT_SUCCESS(status))
		return status;

	pDevObj->Flags |= DO_BUFFERED_IO;
	pDevExt = (PDEVICE_EXTENSION)pDevObj->DeviceExtension;
	pDevExt->pDevice = pDevObj;
	pDevExt->ustrDeviceName = devName;
	//创建符号链接
	UNICODE_STRING symLinkName;
	RtlInitUnicodeString(&symLinkName,L"\\??\\HelloDDK");
	pDevExt->ustrSymLinkName = symLinkName;
	status = IoCreateSymbolicLink( &symLinkName,&devName );
	if (!NT_SUCCESS(status)) 
	{
		IoDeleteDevice( pDevObj );
		return status;
	}

	return STATUS_SUCCESS;
}

/************************************************************************
* 函数名称:HelloDDKUnload
* 功能描述:负责驱动程序的卸载操作
* 参数列表:
      pDriverObject:驱动对象
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
VOID HelloDDKUnload (IN PDRIVER_OBJECT pDriverObject) 
{
	PDEVICE_OBJECT	pNextObj;
	KdPrint(("Enter DriverUnload\n"));
	pNextObj = pDriverObject->DeviceObject;
	while (pNextObj != NULL) 
	{
		PDEVICE_EXTENSION pDevExt = (PDEVICE_EXTENSION)
			pNextObj->DeviceExtension;

		//删除符号链接
		UNICODE_STRING pLinkName = pDevExt->ustrSymLinkName;
		IoDeleteSymbolicLink(&pLinkName);
		pNextObj = pNextObj->NextDevice;
		IoDeleteDevice( pDevExt->pDevice );
	}
}

/************************************************************************
* 函数名称:HelloDDKDispatchRoutine
* 功能描述:对读IRP进行处理
* 参数列表:
      pDevObj:功能设备对象
      pIrp:从IO请求包
* 返回 值:返回状态
*************************************************************************/
#pragma PAGEDCODE
NTSTATUS HelloDDKDispatchRoutine(IN PDEVICE_OBJECT pDevObj,
								 IN PIRP pIrp) 
{
	KdPrint(("Enter HelloDDKDispatchRoutine\n"));
	NTSTATUS status = STATUS_SUCCESS;
	// 完成IRP
	pIrp->IoStatus.Status = status;
	pIrp->IoStatus.Information = 0;	// bytes xfered
	IoCompleteRequest( pIrp, IO_NO_INCREMENT );
	KdPrint(("Leave HelloDDKDispatchRoutine\n"));
	return status;
}