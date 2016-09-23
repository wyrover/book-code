#include "port.h"

///////////////////////////////////////////////////////////////////////////////////
//
//  功能实现：XP-2003下枚举网络连接端口信息
//  输入参数：OutLength为输出缓冲区的大小
//        PortType为要枚举的端口类型
//        TCPPORT-TCP端口
//        UDPPORT-UDP端口
//  输出参数：返回NTSTATUS类型的值
//
///////////////////////////////////////////////////////////////////////////////////
PVOID EnumPortInformation(OUT PULONG  OutLength,IN USHORT  PortType)
{
	ULONG  BufLen=PAGE_SIZE;
	PVOID  pInputBuff=NULL;
	PVOID  pOutputBuff=NULL;
	PVOID  pOutBuf=NULL;
	NTSTATUS status = STATUS_SUCCESS;
	UNICODE_STRING  DeviceName;
	PFILE_OBJECT pFileObject=NULL;
	PDEVICE_OBJECT pDeviceObject=NULL;
	HANDLE FileHandle = NULL;
	KEVENT  Event ;
	IO_STATUS_BLOCK StatusBlock;
	PIRP    pIrp;
	PIO_STACK_LOCATION StackLocation ;
	ULONG    NumOutputBuffers;
	ULONG    i;
	TCP_REQUEST_QUERY_INFORMATION_EX    TdiId;
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCallDriver,L"IoCallDriver",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoGetRelatedDeviceObject,L"IoGetRelatedDeviceObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInitializeEvent,L"KeInitializeEvent",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeWaitForSingleObject,L"KeWaitForSingleObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RZwClose &&
		RIoCallDriver &&
		RIoGetRelatedDeviceObject &&
		RKeInitializeEvent &&
		RKeWaitForSingleObject &&
		RExAllocatePool &&
		RExFreePool)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return NULL;


	RtlZeroMemory(&TdiId,sizeof(TCP_REQUEST_QUERY_INFORMATION_EX));

	if(TCPPORT==PortType)
	{
		TdiId.ID.toi_entity.tei_entity= CO_TL_ENTITY;
	}

	if(UDPPORT==PortType)
	{
		TdiId.ID.toi_entity.tei_entity= CL_TL_ENTITY;
	}

	TdiId.ID.toi_entity.tei_instance = ENTITY_LIST_ID;
	TdiId.ID.toi_class = INFO_CLASS_PROTOCOL;
	TdiId.ID.toi_type = INFO_TYPE_PROVIDER;
	TdiId.ID.toi_id = 0x102;

	pInputBuff=(PVOID)&TdiId;

	__try
	{
		if(UDPPORT==PortType)
		{
			BufLen*=3;
		}
		pOutputBuff=RExAllocatePool(NonPagedPool, BufLen);
		if(NULL==pOutputBuff)
		{
			if (DebugOn)
				KdPrint(("输出缓冲区内存分配失败！\n"));
			*OutLength=0;
			return NULL;
		}
		memset(pOutputBuff,0,BufLen);

		if(TCPPORT==PortType)
		{
			status = GetObjectByName(
				&FileHandle,
				&pFileObject,
				L"\\Device\\Tcp"
				);
		}

		if(UDPPORT==PortType)
		{
			status = GetObjectByName(
				&FileHandle,
				&pFileObject,
				L"\\Device\\Udp"
				);
		}
		if (!NT_SUCCESS(status))
		{
			if (DebugOn)
				KdPrint(("获取设备名失败！\n"));
			*OutLength=0;
			return NULL;
		}
		pDeviceObject = RIoGetRelatedDeviceObject(pFileObject);
		if (NULL == pDeviceObject)
		{
			if (DebugOn)
				KdPrint(("获取设备对象失败！\n"));
			*OutLength=0;
			__leave;
		}

		if (DebugOn)
			KdPrint(("Tcpip Driver Object:%08lX\n", pDeviceObject->DriverObject));
		RKeInitializeEvent(&Event, 0, FALSE);

		pIrp = IoBuildDeviceIoControlRequest(
			IOCTL_TCP_QUERY_INFORMATION_EX, \
			pDeviceObject, 
			pInputBuff, 
			sizeof(TCP_REQUEST_QUERY_INFORMATION_EX), \
			pOutputBuff,
			BufLen, 
			FALSE,
			&Event,
			&StatusBlock
			);
		if (NULL == pIrp)
		{
			if (DebugOn)
				KdPrint(("IRP生成失败！\n"));
			*OutLength=0;
			__leave;
		}

		StackLocation = IoGetNextIrpStackLocation(pIrp);
		StackLocation->FileObject = pFileObject;//不设置这里会蓝屏
		StackLocation->DeviceObject = pDeviceObject;

		status  = RIoCallDriver(pDeviceObject, pIrp);

		if (DebugOn)
			KdPrint(("STATUS:%d\n", RtlNtStatusToDosError(status)));

		if (STATUS_BUFFER_OVERFLOW == status)
		{
			if (DebugOn)
				KdPrint(("缓冲区太小！%d\n",StatusBlock.Information));
		}

		if (STATUS_PENDING == status)
		{
			if (DebugOn)
				KdPrint(("STATUS_PENDING"));

			status = RKeWaitForSingleObject(
				&Event, 
				0, 
				0, 
				0, 
				0
				);
		}
		if(STATUS_CANCELLED == status)
		{
			if (DebugOn)
				KdPrint(("STATUS_CANCELLED"));
		}

		if(status==STATUS_SUCCESS)
		{
			if (DebugOn)
				KdPrint(("Information:%d\r\n",StatusBlock.Information));
			*OutLength=StatusBlock.Information;
			pOutBuf=pOutputBuff;
		}
	}
	__except(EXCEPTION_EXECUTE_HANDLER){ 
		ObDereferenceObject(pFileObject);
		RZwClose(FileHandle);
		return NULL;
	}
	ObDereferenceObject(pFileObject);
	RZwClose(FileHandle);
	return pOutBuf;
}
BOOL PrintTcpIp(PTCPUDPINFO TCPUDPInfo)
{ 
	PVOID pTcpBuf = NULL;
	PVOID pUdpBuf = NULL;

	ULONG ulTcpBuf = 0;
	ULONG ulUdpBuf = 0;
	int i = 0;
	ULONG ulLocalPort;
	ULONG ulRemotePort;

	ULONG ulPid;
	BOOL bInit = FALSE;

	PEPROCESS EProcess;

	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RExFreePool)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return FALSE;

	if (KeGetCurrentIrql() > PASSIVE_LEVEL)
	{
		return FALSE;
	}
	pTcpBuf = EnumPortInformation(&ulTcpBuf,TCPPORT);
	if (pTcpBuf != NULL)
	{
		PMIB_TCPROW_OWNER_PID pTcpInfo = NULL;
		// 打印tcp端口信息
		pTcpInfo = (PMIB_TCPROW_OWNER_PID)pTcpBuf;

		for(;pTcpInfo->dwOwningPid; pTcpInfo++)
		{
			if (DebugOn)
				KdPrint(("Pid:%d,Port:%d\n", pTcpInfo->dwOwningPid, ntohs(pTcpInfo->dwLocalPort)));

			if (!MmIsAddressValidEx(pTcpInfo))
			{
				continue;
			}
			ulLocalPort = ntohs(pTcpInfo->dwLocalPort);
			ulRemotePort = ntohs(pTcpInfo->dwRemotePort);
			if (pTcpInfo->dwRemoteAddr == 0)
			{
				ulRemotePort = 0;
			}

			//排除一些垃圾的东西吧
			if (pTcpInfo->dwState > 20 ||
				ulLocalPort > 65535 ||
				ulLocalPort < NULL)
			{
				continue;
			}

			TCPUDPInfo->TCPUDP[i].ulType = 0;  //tcp
			TCPUDPInfo->TCPUDP[i].ulConnectType = pTcpInfo->dwState;
			TCPUDPInfo->TCPUDP[i].ulLocalAddress = pTcpInfo->dwLocalAddr;
			TCPUDPInfo->TCPUDP[i].ulLocalPort = ulLocalPort;

			memset(TCPUDPInfo->TCPUDP[i].lpwzFullPath,0,sizeof(TCPUDPInfo->TCPUDP[i].lpwzFullPath));

			if (pTcpInfo->dwOwningPid == 4)
			{
				wcsncpy(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"system",wcslen(L"system"));
			}else
			{
				if (LookupProcessByPid(pTcpInfo->dwOwningPid,&EProcess) == STATUS_SUCCESS)
				{
					if (!GetProcessFullImagePath(EProcess,TCPUDPInfo->TCPUDP[i].lpwzFullPath))
					{
						wcsncpy(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"Unknown",wcslen(L"Unknown"));
					}
				}
			}

			memset(TCPUDPInfo->TCPUDP[i].lpszFileMd5,0,sizeof(TCPUDPInfo->TCPUDP[i].lpszFileMd5));
			wcsncpy(TCPUDPInfo->TCPUDP[i].lpszFileMd5,"MD5",strlen("MD5"));

			TCPUDPInfo->TCPUDP[i].ulPid = pTcpInfo->dwOwningPid;
			TCPUDPInfo->TCPUDP[i].ulRemoteAddress = pTcpInfo->dwRemoteAddr;
			TCPUDPInfo->TCPUDP[i].ulRemotePort = ulRemotePort;
			i++;
		}
		//TCPUDPInfo->ulCount = i;

		RExFreePool(pTcpBuf);
	}
	//-------------------------------------------------------
	pUdpBuf = EnumPortInformation(&ulUdpBuf,UDPPORT);
	if (pUdpBuf != NULL)
	{
		PMIB_UDPROW_OWNER_PID pUdpInfo = NULL;
		// 打印udp端口信息
		pUdpInfo = (PMIB_UDPROW_OWNER_PID)pUdpBuf;

		for(;pUdpInfo->dwOwningPid; pUdpInfo++)
		{
			if (!MmIsAddressValidEx(pUdpInfo))
			{
				continue;
			}
			if (DebugOn)
				KdPrint(("Pid:%d,Port:%d\n", pUdpInfo->dwOwningPid, ntohs(pUdpInfo->dwLocalPort)));
			ulLocalPort = ntohs(pUdpInfo->dwLocalPort);
			ulPid = pUdpInfo->dwOwningPid;

			if (ulPid > 10 &&
				ulPid < 88888 &&
				ulLocalPort < 65535 &&
				ulLocalPort > 0)
			{
				TCPUDPInfo->TCPUDP[i].ulType = 1;  //udp
				TCPUDPInfo->TCPUDP[i].ulConnectType = 1;
				TCPUDPInfo->TCPUDP[i].ulLocalAddress = pUdpInfo->dwLocalAddr;
				TCPUDPInfo->TCPUDP[i].ulLocalPort = ulLocalPort;

				memset(TCPUDPInfo->TCPUDP[i].lpwzFullPath,0,sizeof(TCPUDPInfo->TCPUDP[i].lpwzFullPath));
				wcsncpy(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"-",wcslen(L"-"));

				memset(TCPUDPInfo->TCPUDP[i].lpszFileMd5,0,sizeof(TCPUDPInfo->TCPUDP[i].lpszFileMd5));
				memcpy(TCPUDPInfo->TCPUDP[i].lpszFileMd5,"-",strlen("-"));

				TCPUDPInfo->TCPUDP[i].ulPid = ulPid;
				TCPUDPInfo->TCPUDP[i].ulRemoteAddress = 0;
				TCPUDPInfo->TCPUDP[i].ulRemotePort = 0;
				i++;
			}
		}
		RExFreePool(pUdpBuf);
	}
	TCPUDPInfo->ulCount = i;

	return TRUE; 
} 
//////////////////////////////////////////////////////////////////////////////////////////////////////
NTSTATUS  
	EnumTcpPortInformationWin7(
	PMIB_TCPROW_OWNER_PID  *TcpRow,
	ULONG_PTR  *len
	)
{
	PINTERNAL_TCP_TABLE_ENTRY  pBuf1;
	PNSI_STATUS_ENTRY  pBuf2;
	PNSI_PROCESSID_INFO  pBuf3;
	PMIB_TCPROW_OWNER_PID pOutputBuff;
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE FileHandle;
	PFILE_OBJECT pFileObject;
	PDEVICE_OBJECT pDeviceObject;
	KEVENT    Event;
	IO_STATUS_BLOCK  StatusBlock;
	PIRP        pIrp;
	PIO_STACK_LOCATION StackLocation;
	ULONG_PTR  retLen=0;
	ULONG   i;
	NSI_PARAM   nsiStruct={0};

	NPI_MODULEID NPI_MS_TCP_MODULEID = 
	{
		sizeof(NPI_MODULEID),
		MIT_GUID,
		{0xEB004A03, 0x9B1A, 0x11D4,
		{0x91, 0x23, 0x00, 0x50, 0x04, 0x77, 0x59, 0xBC}}
	};
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCallDriver,L"IoCallDriver",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoGetRelatedDeviceObject,L"IoGetRelatedDeviceObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInitializeEvent,L"KeInitializeEvent",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeWaitForSingleObject,L"KeWaitForSingleObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RZwClose &&
		RIoCallDriver &&
		RIoGetRelatedDeviceObject &&
		RKeInitializeEvent &&
		RKeWaitForSingleObject &&
		RExAllocatePool &&
		RExFreePool)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;


	nsiStruct.UnknownParam3=&NPI_MS_TCP_MODULEID;
	nsiStruct.UnknownParam4=3;
	nsiStruct.UnknownParam5=1;
	nsiStruct.UnknownParam6=1;
	// nsiStruct.ConnCount=retLen;

	status = GetObjectByName(&FileHandle, &pFileObject,  L"\\device\\nsi");

	if (!NT_SUCCESS(status))
	{
		if (DebugOn)
			DbgPrint("获取设备名失败！\n");
		goto __end;
	}
	pDeviceObject = RIoGetRelatedDeviceObject(pFileObject);

	if (!NT_SUCCESS(status))
	{
		if (DebugOn)
			DbgPrint("获取设备对象或文件对象失败！\n");
		goto __end;
	}
	if (DebugOn)
		DbgPrint("Tcpip IRP_MJ_DEVICE_CONTROL:%08lX\n", pDeviceObject->DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]);

	RKeInitializeEvent(&Event,NotificationEvent, FALSE);

	pIrp = IoBuildDeviceIoControlRequest(IOCTL_NSI_GETALLPARAM, 
		pDeviceObject,&nsiStruct,sizeof(NSI_PARAM), 
		&nsiStruct,sizeof(NSI_PARAM), FALSE, &Event, &StatusBlock);
	if (NULL == pIrp)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		if (DebugOn)
			DbgPrint("IRP生成失败！\n");
	}
	StackLocation = IoGetNextIrpStackLocation(pIrp);
	StackLocation->FileObject = pFileObject; 
	// pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	StackLocation->DeviceObject = pDeviceObject;
	pIrp->RequestorMode = KernelMode;
	//StackLocation->MinorFunction = IRP_MN_USER_FS_REQUEST;

	status  = RIoCallDriver(pDeviceObject, pIrp);

	if (DebugOn)
		DbgPrint("1STATUS:%08lX\n", status);


	if (STATUS_PENDING == status)
	{
		status = RKeWaitForSingleObject(&Event, Executive,KernelMode,TRUE, 0);
	}
	retLen=nsiStruct.ConnCount;
	retLen+=2;

	pBuf1 =(PINTERNAL_TCP_TABLE_ENTRY)RExAllocatePool(PagedPool,56*retLen);
	if (NULL == pBuf1)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pBuf1, 56*retLen);

	pBuf2 =(PNSI_STATUS_ENTRY)RExAllocatePool(PagedPool,16*retLen);
	if (NULL == pBuf2)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pBuf2, 16*retLen);

	pBuf3= (PNSI_PROCESSID_INFO)RExAllocatePool(PagedPool,32*retLen);
	if (NULL == pBuf3)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pBuf3, 32*retLen);

	pOutputBuff =(PMIB_TCPROW_OWNER_PID)RExAllocatePool(NonPagedPool,retLen*sizeof(MIB_TCPROW_OWNER_PID));
	if (NULL == pOutputBuff)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pOutputBuff, retLen*sizeof(MIB_TCPROW_OWNER_PID));

	nsiStruct.UnknownParam7=pBuf1;
	nsiStruct.UnknownParam8=56;
	nsiStruct.UnknownParam11=pBuf2;
	nsiStruct.UnknownParam12=16;
	nsiStruct.UnknownParam13=pBuf3;
	nsiStruct.UnknownParam14=32;

	pIrp = IoBuildDeviceIoControlRequest(IOCTL_NSI_GETALLPARAM, 
		pDeviceObject,&nsiStruct,sizeof(NSI_PARAM), 
		&nsiStruct,sizeof(NSI_PARAM), FALSE, &Event, &StatusBlock);
	if (NULL == pIrp)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		if (DebugOn)
			DbgPrint("IRP生成失败！\n");
	}
	StackLocation = IoGetNextIrpStackLocation(pIrp);
	StackLocation->FileObject = pFileObject; 
	// pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	StackLocation->DeviceObject = pDeviceObject;
	pIrp->RequestorMode = KernelMode;
	//StackLocation->MinorFunction = IRP_MN_USER_FS_REQUEST;

	status = RIoCallDriver(pDeviceObject, pIrp);

	if (DebugOn)
		DbgPrint("2STATUS:%08lX-%d\n",status, RtlNtStatusToDosError(status));

	if (STATUS_PENDING == status)
	{
		status = RKeWaitForSingleObject(&Event, Executive,KernelMode,TRUE, 0);

	}
	for(i=0; i<nsiStruct.ConnCount; i++)
	{

		pOutputBuff[i].dwState=pBuf2[i].dwState;
		//if ( pBuf1[i].localEntry.bytesfill0 == 2 )
		//{
		pOutputBuff[i].dwLocalAddr =pBuf1[i].localEntry.dwIP;
		pOutputBuff[i].dwLocalPort =pBuf1[i].localEntry.Port;
		pOutputBuff[i].dwRemoteAddr =pBuf1[i].remoteEntry.dwIP;
		// }
		//  else
		/* {
		pOutputBuff[i].dwLocalAddr = 0;
		pOutputBuff[i].dwLocalPort=pBuf1[i].localEntry.Port;
		pOutputBuff[i].dwRemoteAddr= 0;

		}*/
		pOutputBuff[i].dwRemotePort=pBuf1[i].remoteEntry.Port;
		pOutputBuff[i].dwOwningPid=pBuf3[i].dwProcessId;

	}
	*TcpRow=pOutputBuff;
	*len=retLen;

__end:
	if(NULL != pBuf1)
		RExFreePool(pBuf1);

	if(NULL != pBuf2)
		RExFreePool(pBuf2);

	if(NULL != pBuf3)
		RExFreePool(pBuf3);

	if (NULL != pFileObject)
		ObDereferenceObject(pFileObject);

	if ((HANDLE)-1 != FileHandle)
	{
		RZwClose(FileHandle);
	}
	return status;
}
/////
//UDP
NTSTATUS  
	EnumUdpPortInformationWin7(
	PMIB_UDPROW_OWNER_PID  *udpRow,
	ULONG_PTR  *len
	)
{
	PINTERNAL_UDP_TABLE_ENTRY  pBuf1;
	PNSI_PROCESSID_INFO  pBuf2;
	PMIB_UDPROW_OWNER_PID pOutputBuff;
	NTSTATUS status = STATUS_SUCCESS;
	HANDLE FileHandle;
	PFILE_OBJECT pFileObject;
	PDEVICE_OBJECT pDeviceObject;
	KEVENT    Event;
	IO_STATUS_BLOCK  StatusBlock;
	PIRP        pIrp;
	PIO_STACK_LOCATION StackLocation;
	ULONG_PTR  retLen=0;
	NSI_PARAM   nsiStruct={0};
	ULONG i;

	NPI_MODULEID NPI_MS_UDP_MODULEID = 
	{
		sizeof(NPI_MODULEID),
		MIT_GUID,
		{0xEB004A02, 0x9B1A, 0x11D4,
		{0x91, 0x23, 0x00, 0x50, 0x04, 0x77, 0x59, 0xBC}}
	};
	BOOL bInit = FALSE;

	ReLoadNtosCALL(&RZwClose,L"ZwClose",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoCallDriver,L"IoCallDriver",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RIoGetRelatedDeviceObject,L"IoGetRelatedDeviceObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeInitializeEvent,L"KeInitializeEvent",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RKeWaitForSingleObject,L"KeWaitForSingleObject",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExAllocatePool,L"ExAllocatePool",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RZwClose &&
		RIoCallDriver &&
		RIoGetRelatedDeviceObject &&
		RKeInitializeEvent &&
		RKeWaitForSingleObject &&
		RExAllocatePool &&
		RExFreePool)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return STATUS_UNSUCCESSFUL;

	nsiStruct.UnknownParam3=&NPI_MS_UDP_MODULEID;
	nsiStruct.UnknownParam4=1;
	nsiStruct.UnknownParam5=1;
	nsiStruct.UnknownParam6=1;
	// nsiStruct.ConnCount=retLen;

	status = GetObjectByName(&FileHandle, &pFileObject,  L"\\device\\nsi");

	if (!NT_SUCCESS(status))
	{
		if (DebugOn)
			DbgPrint("获取设备名失败！\n");
		goto __end;
	}
	pDeviceObject = RIoGetRelatedDeviceObject(pFileObject);
	if (!NT_SUCCESS(status))
	{
		if (DebugOn)
			DbgPrint("获取设备对象或文件对象失败！\n");
		goto __end;
	}

	if (DebugOn)
		DbgPrint("nsi IRP_MJ_DEVICE_CONTROL:%08lX\n", pDeviceObject->DriverObject->MajorFunction[IRP_MJ_DEVICE_CONTROL]);
	RKeInitializeEvent(&Event,NotificationEvent, FALSE);

	pIrp = IoBuildDeviceIoControlRequest(IOCTL_NSI_GETALLPARAM, 
		pDeviceObject,&nsiStruct,sizeof(NSI_PARAM), 
		&nsiStruct,sizeof(NSI_PARAM), FALSE, &Event, &StatusBlock);
	if (NULL == pIrp)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		if (DebugOn)
			DbgPrint("IRP生成失败！\n");
	}

	StackLocation = IoGetNextIrpStackLocation(pIrp);
	StackLocation->FileObject = pFileObject; 
	//pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	StackLocation->DeviceObject = pDeviceObject;
	pIrp->RequestorMode = KernelMode;

	//StackLocation->MinorFunction = IRP_MN_USER_FS_REQUEST;

	status  = RIoCallDriver(pDeviceObject, pIrp);

	if (DebugOn)
		DbgPrint("STATUS:%08lX\n", status);

	if (STATUS_PENDING == status)
	{
		status = RKeWaitForSingleObject(&Event, Executive,KernelMode,TRUE, 0);
	}
	retLen=nsiStruct.ConnCount;
	retLen+=2;

	pBuf1 =(PINTERNAL_UDP_TABLE_ENTRY)RExAllocatePool(PagedPool,28*retLen);
	if (NULL == pBuf1)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pBuf1, 28*retLen);

	pBuf2= (PNSI_PROCESSID_INFO)RExAllocatePool(PagedPool,32*retLen);
	if (NULL == pBuf1)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pBuf2, 32*retLen);

	pOutputBuff =(PMIB_UDPROW_OWNER_PID)RExAllocatePool(NonPagedPool,retLen*sizeof(MIB_UDPROW_OWNER_PID));
	if (NULL == pOutputBuff)
	{
		if (DebugOn)
			DbgPrint("输出缓冲区内存分配失败！\n");
		goto __end;
	}
	RtlZeroMemory(pOutputBuff, retLen*sizeof(MIB_UDPROW_OWNER_PID));

	nsiStruct.UnknownParam7=pBuf1;
	nsiStruct.UnknownParam8=28;
	nsiStruct.UnknownParam13=pBuf2;
	nsiStruct.UnknownParam14=32;

	pIrp = IoBuildDeviceIoControlRequest(IOCTL_NSI_GETALLPARAM, 
		pDeviceObject,&nsiStruct,sizeof(NSI_PARAM), 
		&nsiStruct,sizeof(NSI_PARAM), FALSE, &Event, &StatusBlock);
	if (NULL == pIrp)
	{
		status = STATUS_INSUFFICIENT_RESOURCES;
		if (DebugOn)
			DbgPrint("IRP生成失败！\n");
	}
	StackLocation = IoGetNextIrpStackLocation(pIrp);
	StackLocation->FileObject = pFileObject; 
	// pIrp->Tail.Overlay.Thread = PsGetCurrentThread();
	StackLocation->DeviceObject = pDeviceObject;
	pIrp->RequestorMode = KernelMode;
	//StackLocation->MinorFunction = IRP_MN_USER_FS_REQUEST;

	status = RIoCallDriver(pDeviceObject, pIrp);

	if (DebugOn)
		DbgPrint("STATUS:%08lX\n", status);

	if (STATUS_PENDING == status)
	{
		status = RKeWaitForSingleObject(&Event, Executive,KernelMode,TRUE, 0);
	}
	for(i=0; i<nsiStruct.ConnCount; i++)
	{
		//if ( pBuf1[i].localEntry.bytesfill0 == 2 )
		//{
		pOutputBuff[i].dwLocalAddr =pBuf1[i].dwIP;
		pOutputBuff[i].dwLocalPort =pBuf1[i].Port;

		// }
		//  else
		/* {
		pOutputBuff[i].dwLocalAddr = 0;
		pOutputBuff[i].dwLocalPort=pBuf1[i].localEntry.Port;
		pOutputBuff[i].dwRemoteAddr= 0;

		}*/
		pOutputBuff[i].dwOwningPid=pBuf2[i].dwUdpProId;

	}
	*udpRow=pOutputBuff;
	*len=retLen;

__end:
	if(NULL != pBuf1)
		RExFreePool(pBuf1);

	if(NULL != pBuf2)
		RExFreePool(pBuf2);

	if (NULL != pFileObject)
		ObDereferenceObject(pFileObject);

	if ((HANDLE)-1 != FileHandle)
	{
		RZwClose(FileHandle);
	}
	return status;
}
////
BOOL PrintTcpIpInWin7(PTCPUDPINFO TCPUDPInfo)
{
	PMIB_TCPROW_OWNER_PID tcpRow=0;
	PMIB_UDPROW_OWNER_PID  udpRow=0;

	NTSTATUS Status;
	ULONG_PTR  num=0;
	int i = 0;
	ULONG ulLocalPort;
	ULONG ulRemotePort;

	ULONG ulPid;
	BOOL bInit = FALSE;

	PEPROCESS EProcess;

	ReLoadNtosCALL(&RExFreePool,L"ExFreePool",SystemKernelModuleBase,ImageModuleBase);
	if (RExFreePool)
	{
		bInit = TRUE;
	}
	if (!bInit)
		return FALSE;

	Status=EnumTcpPortInformationWin7(&tcpRow, &num);
	if(NT_SUCCESS(Status))
	{
		PMIB_TCPROW_OWNER_PID pTcpInfo = NULL;
		// 打印tcp端口信息
		pTcpInfo = (PMIB_TCPROW_OWNER_PID)tcpRow;

		for(;pTcpInfo->dwOwningPid; pTcpInfo++)
		{
			if (!MmIsAddressValidEx(pTcpInfo))
			{
				continue;
			}
			ulLocalPort = ntohs(pTcpInfo->dwLocalPort);
			ulRemotePort = ntohs(pTcpInfo->dwRemotePort);
			if (pTcpInfo->dwRemoteAddr == 0)
			{
				ulRemotePort = 0;
			}

			//排除一些垃圾的东西吧
			if (pTcpInfo->dwState > 20 ||
				ulLocalPort > 65535 ||
				ulLocalPort < NULL)
			{
				continue;
			}

			TCPUDPInfo->TCPUDP[i].ulType = 0;  //tcp
			TCPUDPInfo->TCPUDP[i].ulConnectType = pTcpInfo->dwState;
			TCPUDPInfo->TCPUDP[i].ulLocalAddress = pTcpInfo->dwLocalAddr;
			TCPUDPInfo->TCPUDP[i].ulLocalPort = ulLocalPort;

			memset(TCPUDPInfo->TCPUDP[i].lpwzFullPath,0,sizeof(TCPUDPInfo->TCPUDP[i].lpwzFullPath));

			if (pTcpInfo->dwOwningPid == 4)
			{
				wcsncpy(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"system",wcslen(L"system"));
			}else
			{
				if (LookupProcessByPid(pTcpInfo->dwOwningPid,&EProcess) == STATUS_SUCCESS)
				{
					if (!GetProcessFullImagePath(EProcess,TCPUDPInfo->TCPUDP[i].lpwzFullPath))
					{
						wcsncpy(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"Unknown",wcslen(L"Unknown"));
					}
				}
			}
			if (DebugOn)
				KdPrint(("%ws,Pid:%d,Port:%d\n", TCPUDPInfo->TCPUDP[i].lpwzFullPath,pTcpInfo->dwOwningPid, ntohs(pTcpInfo->dwLocalPort)));

			memset(TCPUDPInfo->TCPUDP[i].lpszFileMd5,0,sizeof(TCPUDPInfo->TCPUDP[i].lpszFileMd5));
			memcpy(TCPUDPInfo->TCPUDP[i].lpszFileMd5,"MD5",strlen("MD5"));

			TCPUDPInfo->TCPUDP[i].ulPid = pTcpInfo->dwOwningPid;
			TCPUDPInfo->TCPUDP[i].ulRemoteAddress = pTcpInfo->dwRemoteAddr;
			TCPUDPInfo->TCPUDP[i].ulRemotePort = ulRemotePort;
			i++;
		}
		//TCPUDPInfo->ulCount = i;

		RExFreePool(tcpRow);
	}
	//---------------------------------
	//-------------------------------------------------------

	Status=EnumUdpPortInformationWin7(&udpRow, &num);
	if(NT_SUCCESS(Status))
	{
		PMIB_UDPROW_OWNER_PID pUdpInfo = NULL;
		// 打印udp端口信息
		pUdpInfo = (PMIB_UDPROW_OWNER_PID)udpRow;

		for(;pUdpInfo->dwOwningPid; pUdpInfo++)
		{
			if (!MmIsAddressValidEx(pUdpInfo))
			{
				continue;
			}
			if (DebugOn)
				KdPrint(("UDP-Pid:%d,Port:%d\n", pUdpInfo->dwOwningPid, ntohs(pUdpInfo->dwLocalPort)));
			ulLocalPort = ntohs(pUdpInfo->dwLocalPort);
			ulPid = pUdpInfo->dwOwningPid;

			if (ulPid > 10 &&
				ulPid < 88888 &&
				ulLocalPort < 65535 &&
				ulLocalPort > 0)
			{
				TCPUDPInfo->TCPUDP[i].ulType = 1;  //udp
				TCPUDPInfo->TCPUDP[i].ulConnectType = 1;
				TCPUDPInfo->TCPUDP[i].ulLocalAddress = pUdpInfo->dwLocalAddr;
				TCPUDPInfo->TCPUDP[i].ulLocalPort = ulLocalPort;

				memset(TCPUDPInfo->TCPUDP[i].lpwzFullPath,0,sizeof(TCPUDPInfo->TCPUDP[i].lpwzFullPath));
				wcsncpy(TCPUDPInfo->TCPUDP[i].lpwzFullPath,L"-",wcslen(L"-"));

				memset(TCPUDPInfo->TCPUDP[i].lpszFileMd5,0,sizeof(TCPUDPInfo->TCPUDP[i].lpszFileMd5));
				memcpy(TCPUDPInfo->TCPUDP[i].lpszFileMd5,"-",strlen("-"));

				TCPUDPInfo->TCPUDP[i].ulPid = ulPid;
				TCPUDPInfo->TCPUDP[i].ulRemoteAddress = 0;
				TCPUDPInfo->TCPUDP[i].ulRemotePort = 0;
				i++;
			}
		}
		RExFreePool(udpRow);
	}
	TCPUDPInfo->ulCount = i;

	return TRUE; 
}