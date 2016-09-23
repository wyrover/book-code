#include "Tcpip.h"

//填充各个结构发送至ring3
VOID FixFixTcp(PTCPDISPATCHBAKUP TcpDispatchBakUp,PDRIVER_OBJECT PTcpDriverObject,int i,ULONG ulReal_Dispatch,WCHAR *lpwzDispatchName,ULONG Dispatch)
{
	ULONG ulCurrentTcpDispatch;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	BOOL bIsHooked = FALSE;
	ULONG ulReloadTcpDispatch;

	TcpDispatchBakUp->TcpDispatch[i].ulTcpDispatch = ulReal_Dispatch;
	TcpDispatchBakUp->TcpDispatch[i].ulNumber = Dispatch;

	memset(TcpDispatchBakUp->TcpDispatch[i].lpwzTcpDispatchName,0,sizeof(TcpDispatchBakUp->TcpDispatch[0].lpwzTcpDispatchName));
	wcsncpy(TcpDispatchBakUp->TcpDispatch[i].lpwzTcpDispatchName,lpwzDispatchName,wcslen(lpwzDispatchName));

	ulCurrentTcpDispatch = PTcpDriverObject->MajorFunction[Dispatch];

	if (DebugOn)
		KdPrint(("ulCurrentTcpDispatch:%08x-%08x",ulCurrentTcpDispatch,PTcpDriverObject));

	if (ulCurrentTcpDispatch == ulReal_Dispatch)
	{
		bIsHooked = TRUE;

		ulReloadTcpDispatch = ulReal_Dispatch - ulTcpipModuleBase + ulReLoadTcpipModuleBase;

		//检查是否inline hook
		if (GetFunctionCodeSize(ulReal_Dispatch) == GetFunctionCodeSize(ulReloadTcpDispatch) &&
			memcmp(ulReal_Dispatch,ulReloadTcpDispatch,GetFunctionCodeSize(ulReal_Dispatch)) != NULL)
		{
			TcpDispatchBakUp->TcpDispatch[i].Hooked = 2; //fsd inline hook

			//简单的处理一下头5字节的hook的检查
			ulCurrentTcpDispatch = *(PULONG)(ulReal_Dispatch+1)+(ULONG)(ulReal_Dispatch+5);
			//如果不是开头jmp hook，就写回原始地址
			if (!MmIsAddressValidEx(ulCurrentTcpDispatch))
			{
				ulCurrentTcpDispatch = ulReal_Dispatch;
			}
		}
	}
	if (!bIsHooked)
	{
		TcpDispatchBakUp->TcpDispatch[i].Hooked = 1;  //fsd hook
	}
	memset(TcpDispatchBakUp->TcpDispatch[i].lpszBaseModule,0,sizeof(TcpDispatchBakUp->TcpDispatch[0].lpszBaseModule));

	if (!IsAddressInSystem(
		ulCurrentTcpDispatch,
		&ulHookModuleBase,
		&ulHookModuleSize,
		TcpDispatchBakUp->TcpDispatch[i].lpszBaseModule))
	{
		strcat(TcpDispatchBakUp->TcpDispatch[i].lpszBaseModule,"Unknown");
	}
	TcpDispatchBakUp->TcpDispatch[i].ulCurrentTcpDispatch = ulCurrentTcpDispatch;
	TcpDispatchBakUp->TcpDispatch[i].ulModuleSize = ulHookModuleSize;
	TcpDispatchBakUp->TcpDispatch[i].ulModuleBase = ulHookModuleBase;

}
VOID SetTcpHook(ULONG ulNumber,ULONG ulRealDispatch)
{
	PDRIVER_OBJECT PTcpDriverObject = NULL;
	ULONG ulReloadDispatch;

	if (MmIsAddressValidEx(PTcpDriverObjectBakup))
	{
		PTcpDriverObject = PTcpDriverObjectBakup;
		PTcpDriverObject->MajorFunction[ulNumber] = ulRealDispatch;   //恢复 fsd hook

		if (GetWindowsVersion() == WINDOWS_VERSION_XP ||
			GetWindowsVersion() == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			ulReloadDispatch = ulRealDispatch - ulTcpipModuleBase + ulReLoadTcpipModuleBase;

			//恢复 fsd inline hook
			if (GetFunctionCodeSize(ulRealDispatch) != GetFunctionCodeSize(ulReloadDispatch))
			{
				return;
			}
			if (memcmp(ulRealDispatch,ulReloadDispatch,GetFunctionCodeSize(ulRealDispatch)) == NULL)
			{
				return;
			}
			__asm
			{
				cli
					push eax
					mov eax,cr0
					and eax,not 0x10000
					mov cr0,eax
					pop eax
			}
			memcpy(ulRealDispatch,ulReloadDispatch,GetFunctionCodeSize(ulRealDispatch));
			__asm
			{
				push eax
					mov eax,cr0
					or eax,0x10000
					mov cr0,eax
					pop eax
					sti
			}
		}
	}
}
NTSTATUS ReLoadTcpip(PDRIVER_OBJECT DriverObject,PTCPDISPATCHBAKUP TcpDispatchBakUp,int Type)
{
	ULONG ulOldTcpipDispatch;
	ULONG ulNewTcpipDispatch;
	PDRIVER_OBJECT PTcpipDriverObject = NULL;
	int i=0,count=0;
	WIN_VER_DETAIL WinVer;
	BOOL bIsReLoadSuccess = FALSE;
	UNICODE_STRING UnicodeModule;
	HANDLE hSection;
	DWORD TcpDriverEntry = 0;
	ULONG ulDriverEntryToDispatchCodeOffset;
	ULONG ulAddress;
	ULONG ulModuleBase;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		return STATUS_UNSUCCESSFUL;
	
	//获取driverobject
	if (GetDriverObject(L"\\Driver\\Tcpip",&PTcpipDriverObject) == STATUS_SUCCESS)
	{
		PTcpDriverObjectBakup = PTcpipDriverObject;

		ulTcpipModuleBase = PTcpipDriverObject->DriverStart;
		ulTcpipModuleSize = PTcpipDriverObject->DriverSize;

		//reload
		if (PeLoad(
			L"\\SystemRoot\\system32\\drivers\\Tcpip.sys",
			&ulReLoadTcpipModuleBase,
			DriverObject,
			ulTcpipModuleBase
			))
		{
			bIsReLoadSuccess = TRUE;
		}
		if (!bIsReLoadSuccess)
		{
			return STATUS_UNSUCCESSFUL;
		}
		if (GetDriverEntryPoint(ulReLoadTcpipModuleBase,&TcpDriverEntry))
		{
			WinVer = GetWindowsVersion();
			switch (WinVer)
			{
			case WINDOWS_VERSION_XP:
				ulDriverEntryToDispatchCodeOffset = 0x155;   //硬编码了，xp
				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				ulDriverEntryToDispatchCodeOffset = 0x174;   //硬编码了，2003
				//ulOffset = (ULONG)PTcpipDriverObject->DriverStart - 0x10000;
				break;
			}
			ulAddress = TcpDriverEntry + ulDriverEntryToDispatchCodeOffset;
			ulReal_TCP_IRP_MJ_CREATE = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_CREATE_NAMED_PIPE = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_CLOSE = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_READ = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_WRITE = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_QUERY_INFORMATION = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SET_INFORMATION = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_QUERY_EA = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SET_EA = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_FLUSH_BUFFERS = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_QUERY_VOLUME_INFORMATION = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SET_VOLUME_INFORMATION = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_DIRECTORY_CONTROL = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_FILE_SYSTEM_CONTROL = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_DEVICE_CONTROL = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SHUTDOWN = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_LOCK_CONTROL = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_CLEANUP = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_CREATE_MAILSLOT = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_QUERY_SECURITY = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SET_SECURITY = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_POWER = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SYSTEM_CONTROL = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_DEVICE_CHANGE = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_QUERY_QUOTA = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_SET_QUOTA = *(PULONG)(ulAddress+1);
			ulReal_TCP_IRP_MJ_PNP_POWER = *(PULONG)(ulAddress+1);

			//KdPrint(("dispatch:%08x\n",*(PULONG)(ulAddress+1) + ulOffset));

			if (Type == 1)
			{
				//填充结构
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,0,ulReal_TCP_IRP_MJ_CREATE,L"IRP_MJ_CREATE",IRP_MJ_CREATE);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,1,ulReal_TCP_IRP_MJ_CREATE_NAMED_PIPE,L"IRP_MJ_CREATE_NAMED_PIPE",IRP_MJ_CREATE_NAMED_PIPE);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,2,ulReal_TCP_IRP_MJ_CLOSE,L"IRP_MJ_CLOSE",IRP_MJ_CLOSE);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,3,ulReal_TCP_IRP_MJ_READ,L"IRP_MJ_READ",IRP_MJ_READ);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,4,ulReal_TCP_IRP_MJ_WRITE,L"IRP_MJ_WRITE",IRP_MJ_WRITE);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,5,ulReal_TCP_IRP_MJ_QUERY_INFORMATION,L"IRP_MJ_QUERY_INFORMATION",IRP_MJ_QUERY_INFORMATION);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,6,ulReal_TCP_IRP_MJ_SET_INFORMATION,L"IRP_MJ_SET_INFORMATION",IRP_MJ_SET_INFORMATION);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,7,ulReal_TCP_IRP_MJ_QUERY_EA,L"IRP_MJ_QUERY_EA",IRP_MJ_QUERY_EA);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,8,ulReal_TCP_IRP_MJ_SET_EA,L"IRP_MJ_SET_EA",IRP_MJ_SET_EA);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,9,ulReal_TCP_IRP_MJ_FLUSH_BUFFERS,L"IRP_MJ_FLUSH_BUFFERS",IRP_MJ_FLUSH_BUFFERS);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,10,ulReal_TCP_IRP_MJ_QUERY_VOLUME_INFORMATION,L"IRP_MJ_QUERY_VOLUME_INFORMATION",IRP_MJ_QUERY_VOLUME_INFORMATION);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,11,ulReal_TCP_IRP_MJ_SET_VOLUME_INFORMATION,L"IRP_MJ_SET_VOLUME_INFORMATION",IRP_MJ_SET_VOLUME_INFORMATION);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,12,ulReal_TCP_IRP_MJ_DIRECTORY_CONTROL,L"IRP_MJ_DIRECTORY_CONTROL",IRP_MJ_DIRECTORY_CONTROL);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,13,ulReal_TCP_IRP_MJ_FILE_SYSTEM_CONTROL,L"IRP_MJ_FILE_SYSTEM_CONTROL",IRP_MJ_FILE_SYSTEM_CONTROL);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,14,ulReal_TCP_IRP_MJ_DEVICE_CONTROL,L"IRP_MJ_DEVICE_CONTROL",IRP_MJ_DEVICE_CONTROL);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,15,ulReal_TCP_IRP_MJ_SHUTDOWN,L"IRP_MJ_SHUTDOWN",IRP_MJ_SHUTDOWN);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,16,ulReal_TCP_IRP_MJ_LOCK_CONTROL,L"IRP_MJ_LOCK_CONTROL",IRP_MJ_LOCK_CONTROL);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,17,ulReal_TCP_IRP_MJ_CLEANUP,L"IRP_MJ_CLEANUP",IRP_MJ_CLEANUP);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,18,ulReal_TCP_IRP_MJ_CREATE_MAILSLOT,L"IRP_MJ_CREATE_MAILSLOT",IRP_MJ_CREATE_MAILSLOT);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,19,ulReal_TCP_IRP_MJ_QUERY_SECURITY,L"IRP_MJ_QUERY_SECURITY",IRP_MJ_QUERY_SECURITY);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,20,ulReal_TCP_IRP_MJ_SET_SECURITY,L"IRP_MJ_SET_SECURITY",IRP_MJ_SET_SECURITY);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,21,ulReal_TCP_IRP_MJ_POWER,L"IRP_MJ_POWER",IRP_MJ_POWER);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,22,ulReal_TCP_IRP_MJ_SYSTEM_CONTROL,L"IRP_MJ_SYSTEM_CONTROL",IRP_MJ_SYSTEM_CONTROL);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,23,ulReal_TCP_IRP_MJ_DEVICE_CHANGE,L"IRP_MJ_DEVICE_CHANGE",IRP_MJ_DEVICE_CHANGE);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,24,ulReal_TCP_IRP_MJ_QUERY_QUOTA,L"IRP_MJ_QUERY_QUOTA",IRP_MJ_QUERY_QUOTA);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,25,ulReal_TCP_IRP_MJ_SET_QUOTA,L"IRP_MJ_SET_QUOTA",IRP_MJ_SET_QUOTA);
				FixFixTcp(TcpDispatchBakUp,PTcpDriverObjectBakup,26,ulReal_TCP_IRP_MJ_PNP_POWER,L"IRP_MJ_PNP_POWER",IRP_MJ_PNP_POWER);
				TcpDispatchBakUp->ulCount = 27;
				return STATUS_SUCCESS;
			}
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_TCP_IRP_MJ_CREATE - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = ulReal_TCP_IRP_MJ_CREATE_NAMED_PIPE - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_TCP_IRP_MJ_CLOSE - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_TCP_IRP_MJ_READ - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_TCP_IRP_MJ_WRITE - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_INFORMATION] = ulReal_TCP_IRP_MJ_QUERY_INFORMATION - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_TCP_IRP_MJ_SET_INFORMATION - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_EA] = ulReal_TCP_IRP_MJ_QUERY_EA - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_EA] = ulReal_TCP_IRP_MJ_SET_EA - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_TCP_IRP_MJ_FLUSH_BUFFERS - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION] = ulReal_TCP_IRP_MJ_QUERY_VOLUME_INFORMATION - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_VOLUME_INFORMATION] = ulReal_TCP_IRP_MJ_SET_VOLUME_INFORMATION - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_TCP_IRP_MJ_DIRECTORY_CONTROL - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_TCP_IRP_MJ_FILE_SYSTEM_CONTROL - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_TCP_IRP_MJ_DEVICE_CONTROL - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_TCP_IRP_MJ_SHUTDOWN - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_TCP_IRP_MJ_LOCK_CONTROL - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_TCP_IRP_MJ_CLEANUP - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = ulReal_TCP_IRP_MJ_CREATE_MAILSLOT - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_SECURITY] = ulReal_TCP_IRP_MJ_QUERY_SECURITY - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_SECURITY] = ulReal_TCP_IRP_MJ_SET_SECURITY - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_TCP_IRP_MJ_POWER - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_TCP_IRP_MJ_SYSTEM_CONTROL - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CHANGE] = ulReal_TCP_IRP_MJ_DEVICE_CHANGE - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_QUOTA] = ulReal_TCP_IRP_MJ_QUERY_QUOTA - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_QUOTA] = ulReal_TCP_IRP_MJ_SET_QUOTA - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_TCP_IRP_MJ_PNP_POWER - ulTcpipModuleBase + ulReLoadTcpipModuleBase;
		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ReLoadTcpipFree()
{
	int i=0;
	BOOL bIsWinVer = FALSE;
	WIN_VER_DETAIL WinVer;
	PDRIVER_OBJECT PTcpDriverObject = NULL;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		return STATUS_UNSUCCESSFUL;

	if (MmIsAddressValidEx(PTcpDriverObjectBakup))
	{
		PTcpDriverObject = PTcpDriverObjectBakup;

		if (DebugOn)
			KdPrint(("ReLoadTcpFree success"));

		if (ulReal_TCP_IRP_MJ_CREATE &&
			ulReal_TCP_IRP_MJ_CREATE_NAMED_PIPE &&
			ulReal_TCP_IRP_MJ_CLOSE &&
			ulReal_TCP_IRP_MJ_READ &&
			ulReal_TCP_IRP_MJ_WRITE &&
			ulReal_TCP_IRP_MJ_QUERY_INFORMATION &&
			ulReal_TCP_IRP_MJ_SET_INFORMATION &&
			ulReal_TCP_IRP_MJ_QUERY_EA &&
			ulReal_TCP_IRP_MJ_SET_EA &&
			ulReal_TCP_IRP_MJ_FLUSH_BUFFERS &&
			ulReal_TCP_IRP_MJ_QUERY_VOLUME_INFORMATION &&
			ulReal_TCP_IRP_MJ_SET_VOLUME_INFORMATION &&
			ulReal_TCP_IRP_MJ_DIRECTORY_CONTROL &&
			ulReal_TCP_IRP_MJ_FILE_SYSTEM_CONTROL &&
			ulReal_TCP_IRP_MJ_DEVICE_CONTROL &&
			ulReal_TCP_IRP_MJ_SHUTDOWN &&
			ulReal_TCP_IRP_MJ_LOCK_CONTROL &&
			ulReal_TCP_IRP_MJ_CLEANUP &&
			ulReal_TCP_IRP_MJ_CREATE_MAILSLOT &&
			ulReal_TCP_IRP_MJ_QUERY_SECURITY &&
			ulReal_TCP_IRP_MJ_SET_SECURITY &&
			ulReal_TCP_IRP_MJ_POWER &&
			ulReal_TCP_IRP_MJ_SYSTEM_CONTROL &&
			ulReal_TCP_IRP_MJ_DEVICE_CHANGE &&
			ulReal_TCP_IRP_MJ_QUERY_QUOTA &&
			ulReal_TCP_IRP_MJ_SET_QUOTA &&
			ulReal_TCP_IRP_MJ_PNP_POWER)
		{
			//恢复
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_TCP_IRP_MJ_CREATE;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = ulReal_TCP_IRP_MJ_CREATE_NAMED_PIPE;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_TCP_IRP_MJ_CLOSE;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_TCP_IRP_MJ_READ;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_TCP_IRP_MJ_WRITE;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_INFORMATION] = ulReal_TCP_IRP_MJ_QUERY_INFORMATION;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_TCP_IRP_MJ_SET_INFORMATION;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_EA] = ulReal_TCP_IRP_MJ_QUERY_EA;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_EA] = ulReal_TCP_IRP_MJ_SET_EA;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_TCP_IRP_MJ_FLUSH_BUFFERS;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION] = ulReal_TCP_IRP_MJ_QUERY_VOLUME_INFORMATION;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_VOLUME_INFORMATION] = ulReal_TCP_IRP_MJ_SET_VOLUME_INFORMATION;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_TCP_IRP_MJ_DIRECTORY_CONTROL;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_TCP_IRP_MJ_FILE_SYSTEM_CONTROL;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_TCP_IRP_MJ_DEVICE_CONTROL;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_TCP_IRP_MJ_SHUTDOWN;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_TCP_IRP_MJ_LOCK_CONTROL;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_TCP_IRP_MJ_CLEANUP;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = ulReal_TCP_IRP_MJ_CREATE_MAILSLOT;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_SECURITY] = ulReal_TCP_IRP_MJ_QUERY_SECURITY;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_SECURITY] = ulReal_TCP_IRP_MJ_SET_SECURITY;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_TCP_IRP_MJ_POWER;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_TCP_IRP_MJ_SYSTEM_CONTROL;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CHANGE] = ulReal_TCP_IRP_MJ_DEVICE_CHANGE;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_QUOTA] = ulReal_TCP_IRP_MJ_QUERY_QUOTA;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_SET_QUOTA] = ulReal_TCP_IRP_MJ_SET_QUOTA;
			PTcpDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_TCP_IRP_MJ_PNP_POWER;
		}

	}
	return STATUS_SUCCESS;
}