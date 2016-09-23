#include "Ntfs.h"

//填充各个结构发送至ring3
VOID FixNtfs(PNTFSDISPATCHBAKUP NtfsDispatchBakUp,PDRIVER_OBJECT PNtfsDriverObject,int i,ULONG ulReal_Dispatch,WCHAR *lpwzDispatchName,ULONG Dispatch)
{
	ULONG ulCurrentNtfsDispatch;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	BOOL bIsHooked = FALSE;
	ULONG ulReloadNtfsDispatch;

	NtfsDispatchBakUp->NtfsDispatch[i].ulNtfsDispatch = ulReal_Dispatch;
	NtfsDispatchBakUp->NtfsDispatch[i].ulNumber = Dispatch;

	memset(NtfsDispatchBakUp->NtfsDispatch[i].lpwzNtfsDispatchName,0,sizeof(NtfsDispatchBakUp->NtfsDispatch[0].lpwzNtfsDispatchName));
	wcsncpy(NtfsDispatchBakUp->NtfsDispatch[i].lpwzNtfsDispatchName,lpwzDispatchName,wcslen(lpwzDispatchName));
	
	ulCurrentNtfsDispatch = PNtfsDriverObject->MajorFunction[Dispatch];

	if (DebugOn)
		KdPrint(("ulCurrentNtfsDispatch:%08x-%08x",ulCurrentNtfsDispatch,PNtfsDriverObject));

	if (ulCurrentNtfsDispatch == ulReal_Dispatch)
	{
		bIsHooked = TRUE;

		ulReloadNtfsDispatch = ulReal_Dispatch - ulNtfsModuleBase + ulReLoadNtfsModuleBase;

		//检查是否inline hook
		if (GetFunctionCodeSize(ulReal_Dispatch) == GetFunctionCodeSize(ulReloadNtfsDispatch) &&
			memcmp(ulReal_Dispatch,ulReloadNtfsDispatch,GetFunctionCodeSize(ulReal_Dispatch)) != NULL)
		{
			NtfsDispatchBakUp->NtfsDispatch[i].Hooked = 2; //fsd inline hook

			//简单的处理一下头5字节的hook的检查
			ulCurrentNtfsDispatch = *(PULONG)(ulReal_Dispatch+1)+(ULONG)(ulReal_Dispatch+5);
			//如果不是开头jmp hook，就写回原始地址
			if (!MmIsAddressValidEx(ulCurrentNtfsDispatch))
			{
				ulCurrentNtfsDispatch = ulReal_Dispatch;
			}
		}
	}
	if (!bIsHooked)
	{
		NtfsDispatchBakUp->NtfsDispatch[i].Hooked = 1;  //fsd hook
	}
	memset(NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule,0,sizeof(NtfsDispatchBakUp->NtfsDispatch[0].lpszBaseModule));

	if (!IsAddressInSystem(
		ulCurrentNtfsDispatch,
		&ulHookModuleBase,
		&ulHookModuleSize,
		NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule))
	{
		strcat(NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule,"Unknown");
	}
	NtfsDispatchBakUp->NtfsDispatch[i].ulCurrentNtfsDispatch = ulCurrentNtfsDispatch;
	NtfsDispatchBakUp->NtfsDispatch[i].ulModuleSize = ulHookModuleSize;
	NtfsDispatchBakUp->NtfsDispatch[i].ulModuleBase = ulHookModuleBase;

}
VOID SetFsdHook(ULONG ulNumber,ULONG ulRealDispatch)
{
	PDRIVER_OBJECT PNtfsDriverObject = NULL;
	ULONG ulReloadDispatch;

	if (MmIsAddressValidEx(PNtfsDriverObjectBakup))
	{
		PNtfsDriverObject = PNtfsDriverObjectBakup;
		PNtfsDriverObject->MajorFunction[ulNumber] = ulRealDispatch;   //恢复 fsd hook

		ulReloadDispatch = ulRealDispatch - ulNtfsModuleBase + ulReLoadNtfsModuleBase;

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
NTSTATUS ReLoadNtfs(PDRIVER_OBJECT DriverObject,PNTFSDISPATCHBAKUP NtfsDispatchBakUp,int IniType)
{
	ULONG ulOldNtfsDispatch;
	ULONG ulNewNtfsDispatch;
	PDRIVER_OBJECT PNtfsDriverObject = NULL;
	int i=0;
	BOOL bIsWinVer = FALSE;
	BOOL bInit = FALSE;
	WIN_VER_DETAIL WinVer;
	BOOL bIsReLoadSuccess = FALSE;
	DWORD NtfsDriverEntry=0;
	ULONG ulAddress;
	ULONG ulDriverEntryToDispatchCodeOffset;
	HANDLE hSection;
	UNICODE_STRING UnicodeModule;
	ULONG ulModuleBase;

	//获取driverobject
	if (GetDriverObject(L"\\FileSystem\\Ntfs",&PNtfsDriverObject) == STATUS_SUCCESS)
	{
		PNtfsDriverObjectBakup = PNtfsDriverObject;

		ulNtfsModuleBase = PNtfsDriverObject->DriverStart;
		ulNtfsModuleSize = PNtfsDriverObject->DriverSize;

		//reload 已经兼容了2003
		if (PeLoad(
			L"\\SystemRoot\\system32\\drivers\\ntfs.sys",
			&ulReLoadNtfsModuleBase,
			DriverObject,
			ulNtfsModuleBase
			))
		{
			bIsReLoadSuccess = TRUE;
		}
		if (!bIsReLoadSuccess)
		{
			return STATUS_UNSUCCESSFUL;
		}
		if (GetDriverEntryPoint(ulReLoadNtfsModuleBase,&NtfsDriverEntry))
		{
			//KdPrint(("NtfsDriverEntry:%08x\r\n",NtfsDriverEntry));

			WinVer = GetWindowsVersion();
			switch (WinVer)
			{
			case WINDOWS_VERSION_XP:
				ulDriverEntryToDispatchCodeOffset = 0xFA;   //硬编码了，xp
				break;
			case WINDOWS_VERSION_7_7000:
				ulDriverEntryToDispatchCodeOffset = 0x233;   //硬编码了，win7_7000
				break;
			case WINDOWS_VERSION_7_7600_UP:
				ulDriverEntryToDispatchCodeOffset = 0x205;   //硬编码了，win7_7600_UP
				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				ulDriverEntryToDispatchCodeOffset = 0x101;   //硬编码了，2003
				//ulOffset = (ULONG)PNtfsDriverObject->DriverStart - 0x10000;
				break;
			}
			ulAddress = NtfsDriverEntry + ulDriverEntryToDispatchCodeOffset;
			ulReal_IRP_MJ_LOCK_CONTROL = *(PULONG)(ulAddress+3);
			ulReal_IRP_MJ_DIRECTORY_CONTROL = *(PULONG)(ulAddress+0xA);
			ulReal_IRP_MJ_SET_INFORMATION = *(PULONG)(ulAddress+0x11);
			ulReal_IRP_MJ_CREATE = *(PULONG)(ulAddress+0x18);
			ulReal_IRP_MJ_CLOSE = *(PULONG)(ulAddress+0x1F);
			ulReal_IRP_MJ_READ = *(PULONG)(ulAddress+0x26);
			ulReal_IRP_MJ_WRITE = *(PULONG)(ulAddress+0x2D);
			ulReal_IRP_MJ_FLUSH_BUFFERS = *(PULONG)(ulAddress+0x34);
			ulReal_IRP_MJ_FILE_SYSTEM_CONTROL = *(PULONG)(ulAddress+0x3B);

			ulReal_IRP_MJ_CLEANUP  = *(PULONG)(ulAddress+0x45);
			ulReal_IRP_MJ_SHUTDOWN  = *(PULONG)(ulAddress+0x4c);
			ulReal_IRP_MJ_PNP_POWER  = *(PULONG)(ulAddress+0x56);

			if (IniType == 1)
			{
				//填充结构
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,0,ulReal_IRP_MJ_LOCK_CONTROL,L"IRP_MJ_LOCK_CONTROL",IRP_MJ_LOCK_CONTROL);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,1,ulReal_IRP_MJ_DIRECTORY_CONTROL,L"IRP_MJ_DIRECTORY_CONTROL",IRP_MJ_DIRECTORY_CONTROL);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,2,ulReal_IRP_MJ_SET_INFORMATION,L"IRP_MJ_SET_INFORMATION",IRP_MJ_SET_INFORMATION);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,3,ulReal_IRP_MJ_CREATE,L"IRP_MJ_CREATE",IRP_MJ_CREATE);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,4,ulReal_IRP_MJ_CLOSE,L"IRP_MJ_CLOSE",IRP_MJ_CLOSE);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,5,ulReal_IRP_MJ_READ,L"IRP_MJ_READ",IRP_MJ_READ);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,6,ulReal_IRP_MJ_WRITE,L"IRP_MJ_WRITE",IRP_MJ_WRITE);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,7,ulReal_IRP_MJ_FLUSH_BUFFERS,L"IRP_MJ_FLUSH_BUFFERS",IRP_MJ_FLUSH_BUFFERS);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,8,ulReal_IRP_MJ_FILE_SYSTEM_CONTROL,L"IRP_MJ_FILE_SYSTEM_CONTROL",IRP_MJ_FILE_SYSTEM_CONTROL);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,9,ulReal_IRP_MJ_CLEANUP,L"IRP_MJ_CLEANUP",IRP_MJ_CLEANUP);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,10,ulReal_IRP_MJ_SHUTDOWN,L"IRP_MJ_SHUTDOWN",IRP_MJ_SHUTDOWN);
				FixNtfs(NtfsDispatchBakUp,PNtfsDriverObjectBakup,11,ulReal_IRP_MJ_PNP_POWER,L"IRP_MJ_PNP_POWER",IRP_MJ_PNP_POWER);
				NtfsDispatchBakUp->ulCount = 12;
				goto DebugPrintOn;
			}

			//让所有的调用都走reload ntfs
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_IRP_MJ_LOCK_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_IRP_MJ_DIRECTORY_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_IRP_MJ_SET_INFORMATION - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_IRP_MJ_CREATE - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_IRP_MJ_CLOSE - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_IRP_MJ_READ - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_IRP_MJ_WRITE - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_IRP_MJ_FLUSH_BUFFERS - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_IRP_MJ_FILE_SYSTEM_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_IRP_MJ_CLEANUP - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_IRP_MJ_SHUTDOWN - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
			PNtfsDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_IRP_MJ_PNP_POWER - ulNtfsModuleBase + ulReLoadNtfsModuleBase;
DebugPrintOn:
			if (DebugOn)
				KdPrint(("[%08x]ulReal_IRP_MJ_LOCK_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_DIRECTORY_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_SET_INFORMATION:%08x\r\n"
				"ulReal_IRP_MJ_CREATE:%08x\r\n"
				"ulReal_IRP_MJ_CLOSE:%08x\r\n"
				"ulReal_IRP_MJ_READ:%08x\r\n"
				"ulReal_IRP_MJ_WRITE:%08x\r\n"
				"ulReal_IRP_MJ_FLUSH_BUFFERS:%08x\r\n"
				"ulReal_IRP_MJ_FILE_SYSTEM_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_CLEANUP:%08x\r\n"
				"ulReal_IRP_MJ_SHUTDOWN:%08x\r\n"
				"ulReal_IRP_MJ_PNP_POWER:%08x\r\n",
				ulAddress,
				ulReal_IRP_MJ_LOCK_CONTROL,
				ulReal_IRP_MJ_DIRECTORY_CONTROL,
				ulReal_IRP_MJ_SET_INFORMATION,
				ulReal_IRP_MJ_CREATE,
				ulReal_IRP_MJ_CLOSE,
				ulReal_IRP_MJ_READ,
				ulReal_IRP_MJ_WRITE,
				ulReal_IRP_MJ_FLUSH_BUFFERS,
				ulReal_IRP_MJ_FILE_SYSTEM_CONTROL,
				ulReal_IRP_MJ_CLEANUP,
				ulReal_IRP_MJ_SHUTDOWN,
				ulReal_IRP_MJ_PNP_POWER
				));
			if (DebugOn)
				KdPrint(("[%08x]ulReal_IRP_MJ_LOCK_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_DIRECTORY_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_SET_INFORMATION:%08x\r\n"
				"ulReal_IRP_MJ_CREATE:%08x\r\n"
				"ulReal_IRP_MJ_CLOSE:%08x\r\n"
				"ulReal_IRP_MJ_READ:%08x\r\n"
				"ulReal_IRP_MJ_WRITE:%08x\r\n"
				"ulReal_IRP_MJ_FLUSH_BUFFERS:%08x\r\n"
				"ulReal_IRP_MJ_FILE_SYSTEM_CONTROL:%08x\r\n"
				"ulReal_IRP_MJ_CLEANUP:%08x\r\n"
				"ulReal_IRP_MJ_SHUTDOWN:%08x\r\n"
				"ulReal_IRP_MJ_PNP_POWER:%08x\r\n",
				ulAddress,
				ulReal_IRP_MJ_LOCK_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_DIRECTORY_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_SET_INFORMATION - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_CREATE - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_CLOSE - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_READ - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_WRITE - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_FLUSH_BUFFERS - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_FILE_SYSTEM_CONTROL - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_CLEANUP - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_SHUTDOWN - ulNtfsModuleBase + ulReLoadNtfsModuleBase,
				ulReal_IRP_MJ_PNP_POWER - ulNtfsModuleBase + ulReLoadNtfsModuleBase
				));
		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ReLoadNtfsFree()
{
	int i=0;
 	BOOL bIsWinVer = FALSE;
 	WIN_VER_DETAIL WinVer;
	PDRIVER_OBJECT PNtfsDriverObject = NULL;

	if (MmIsAddressValidEx(PNtfsDriverObjectBakup))
	{
		PNtfsDriverObject = PNtfsDriverObjectBakup;

		if (DebugOn)
			KdPrint(("ReLoadNtfsFree success"));

		if (ulReal_IRP_MJ_LOCK_CONTROL)
			PNtfsDriverObject->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_IRP_MJ_LOCK_CONTROL;

		if (ulReal_IRP_MJ_DIRECTORY_CONTROL)
			PNtfsDriverObject->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_IRP_MJ_DIRECTORY_CONTROL;

		if (ulReal_IRP_MJ_SET_INFORMATION)
			PNtfsDriverObject->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_IRP_MJ_SET_INFORMATION;

		if (ulReal_IRP_MJ_CREATE)
			PNtfsDriverObject->MajorFunction[IRP_MJ_CREATE] = ulReal_IRP_MJ_CREATE;

		if (ulReal_IRP_MJ_CLOSE)
			PNtfsDriverObject->MajorFunction[IRP_MJ_CLOSE] = ulReal_IRP_MJ_CLOSE;

		if (ulReal_IRP_MJ_READ)
			PNtfsDriverObject->MajorFunction[IRP_MJ_READ] = ulReal_IRP_MJ_READ;

		if (ulReal_IRP_MJ_WRITE)
			PNtfsDriverObject->MajorFunction[IRP_MJ_WRITE] = ulReal_IRP_MJ_WRITE;

		if (ulReal_IRP_MJ_FLUSH_BUFFERS)
			PNtfsDriverObject->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_IRP_MJ_FLUSH_BUFFERS;

		if (ulReal_IRP_MJ_FILE_SYSTEM_CONTROL)
			PNtfsDriverObject->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_IRP_MJ_FILE_SYSTEM_CONTROL;

		if (ulReal_IRP_MJ_CLEANUP)
			PNtfsDriverObject->MajorFunction[IRP_MJ_CLEANUP] = ulReal_IRP_MJ_CLEANUP;

		if (ulReal_IRP_MJ_SHUTDOWN)
			PNtfsDriverObject->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_IRP_MJ_SHUTDOWN;

		if (ulReal_IRP_MJ_PNP_POWER)
			PNtfsDriverObject->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_IRP_MJ_PNP_POWER;

	}
	return STATUS_SUCCESS;
}