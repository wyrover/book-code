#include "Atapi.h"

VOID SetAtapiHook(ULONG ulNumber,ULONG ulRealDispatch)
{
	PDRIVER_OBJECT PAtapiDriverObject = NULL;
	ULONG ulReloadDispatch;


	if (DebugOn)
		KdPrint(("%d:%08x\r\n",ulNumber,ulRealDispatch));

	if (MmIsAddressValidEx(PAtapiDriverObjectBakup))
	{
		PAtapiDriverObject = PAtapiDriverObjectBakup;
		PAtapiDriverObject->MajorFunction[ulNumber] = ulRealDispatch;   //恢复hook

		if (GetWindowsVersion() == WINDOWS_VERSION_XP ||
			GetWindowsVersion() == WINDOWS_VERSION_2K3_SP1_SP2)
		{
			ulReloadDispatch = ulRealDispatch - ulAtapiModuleBase + ulReLoadAtapiModuleBase;

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
//填充各个结构发送至ring3
VOID FixFixAtapi(PATAPIDISPATCHBAKUP AtapiDispatchBakUp,PDRIVER_OBJECT PAtapiDriverObject,int i,ULONG ulReal_Dispatch,WCHAR *lpwzDispatchName,ULONG Dispatch)
{
	ULONG ulCurrentAtapiDispatch;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	BOOL bIsHooked = FALSE;
	ULONG ulReloadAtapiDispatch;

	AtapiDispatchBakUp->AtapiDispatch[i].ulAtapiDispatch = ulReal_Dispatch;
	AtapiDispatchBakUp->AtapiDispatch[i].ulNumber = Dispatch;

	memset(AtapiDispatchBakUp->AtapiDispatch[i].lpwzAtapiDispatchName,0,sizeof(AtapiDispatchBakUp->AtapiDispatch[0].lpwzAtapiDispatchName));
	wcsncpy(AtapiDispatchBakUp->AtapiDispatch[i].lpwzAtapiDispatchName,lpwzDispatchName,wcslen(lpwzDispatchName));

	ulCurrentAtapiDispatch = PAtapiDriverObject->MajorFunction[Dispatch];
	if (ulCurrentAtapiDispatch == (ULONG)PAtapiDriverObject)
	{
		ulCurrentAtapiDispatch = ulReal_Dispatch;
	}
	if (DebugOn)
		KdPrint(("[%d]ulCurrentAtapiDispatch:%08x-%08x",Dispatch,ulCurrentAtapiDispatch,ulReal_Dispatch));

	if (ulCurrentAtapiDispatch == ulReal_Dispatch)
	{
		if (DebugOn)
			KdPrint(("UnHook:%08x:%ws\r\n",ulReal_Dispatch,lpwzDispatchName));

		bIsHooked = TRUE;

		ulReloadAtapiDispatch = ulReal_Dispatch - ulAtapiModuleBase + ulReLoadAtapiModuleBase;

		//检查是否inline hook
		if (GetFunctionCodeSize(ulReal_Dispatch) == GetFunctionCodeSize(ulReloadAtapiDispatch) &&
			memcmp(ulReal_Dispatch,ulReloadAtapiDispatch,GetFunctionCodeSize(ulReal_Dispatch)) != NULL)
		{
			AtapiDispatchBakUp->AtapiDispatch[i].Hooked = 2; // inline hook

			//简单的处理一下头5字节的hook的检查
			ulCurrentAtapiDispatch = *(PULONG)(ulReal_Dispatch+1)+(ULONG)(ulReal_Dispatch+5);
			//如果不是开头jmp hook，就写回原始地址
			if (!MmIsAddressValidEx(ulCurrentAtapiDispatch))
			{
				ulCurrentAtapiDispatch = ulReal_Dispatch;
			}
		}
	}
	if (!bIsHooked)
	{
		AtapiDispatchBakUp->AtapiDispatch[i].Hooked = 1;  // hook
	}
	memset(AtapiDispatchBakUp->AtapiDispatch[i].lpszBaseModule,0,sizeof(AtapiDispatchBakUp->AtapiDispatch[0].lpszBaseModule));

	if (!IsAddressInSystem(
		ulCurrentAtapiDispatch,
		&ulHookModuleBase,
		&ulHookModuleSize,
		AtapiDispatchBakUp->AtapiDispatch[i].lpszBaseModule))
	{
		strcat(AtapiDispatchBakUp->AtapiDispatch[i].lpszBaseModule,"Unknown");
	}
	AtapiDispatchBakUp->AtapiDispatch[i].ulCurrentAtapiDispatch = ulCurrentAtapiDispatch;
	AtapiDispatchBakUp->AtapiDispatch[i].ulModuleSize = ulHookModuleSize;
	AtapiDispatchBakUp->AtapiDispatch[i].ulModuleBase = ulHookModuleBase;

}
NTSTATUS ReLoadAtapi(PDRIVER_OBJECT DriverObject,PATAPIDISPATCHBAKUP AtapiDispatchBakUp,int Type)
{
	PDRIVER_OBJECT PAtapiDriverObject = NULL;
	PUCHAR i;
	WIN_VER_DETAIL WinVer;
	BOOL bInit = FALSE;
	PUCHAR AtapiDriverEntry;
	PUCHAR ulJmpAddress,ulAddress;
	ULONG ulDispatch;
	ULONG ulDriverEntryToDispatchCodeOffset;
	ULONG ulOffset;
	BOOL bIsReLoadSuccess = FALSE;
	UNICODE_STRING UnicodeModule;
	HANDLE hSection;
	ULONG ulModuleBase;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		return STATUS_UNSUCCESSFUL;

	//获取driverobject
	if (GetDriverObject(L"\\Driver\\atapi",&PAtapiDriverObject) == STATUS_SUCCESS)
	{
		PAtapiDriverObjectBakup = PAtapiDriverObject;

		ulAtapiModuleBase = PAtapiDriverObject->DriverStart;
		ulAtapiModuleSize = PAtapiDriverObject->DriverSize;

		//reload 已经兼容了2003
		if (PeLoad(
			L"\\SystemRoot\\system32\\drivers\\Atapi.sys",
			&ulReLoadAtapiModuleBase,
			DriverObject,
			ulAtapiModuleBase
			))
		{
			bIsReLoadSuccess = TRUE;
		}
		if (!bIsReLoadSuccess)
		{
			return STATUS_UNSUCCESSFUL;
		}
		if (GetDriverEntryPoint(ulReLoadAtapiModuleBase,&AtapiDriverEntry))
		{
			if (DebugOn)
				KdPrint(("AtapiDriverEntry:%08x\r\n",AtapiDriverEntry));

			for (i=(ULONG)AtapiDriverEntry;i < (ULONG)AtapiDriverEntry+0x1000;i++)
			{
				if (*i == 0xe9)
				{
					ulJmpAddress = *(PULONG)(i+1)+(ULONG)(i+5);
					//if (MmIsAddressValidEx(ulJmpAddress))
					//{
						if (DebugOn)
							KdPrint(("found:i:%08x,DriverEntry:%08x\n",i,ulJmpAddress));
						bInit = TRUE;
						break;
					//}
				}
			}
			if (!bInit)
			{
				return;
			}
			WinVer = GetWindowsVersion();
			switch (WinVer)
			{
			case WINDOWS_VERSION_XP:
				IRP_DRIVER_START_IO = 0x1c;  //1c == 28
				ulDriverEntryToDispatchCodeOffset = 0x90;   //硬编码了，xp
				ulOffset = 0;
				break;
// 			case WINDOWS_VERSION_7:
// 				ulDriverEntryToDispatchCodeOffset = 0x2D6;   //硬编码了，win7
// 				ulOffset = 0;
// 				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				IRP_DRIVER_START_IO = 0x1c;  //1c == 28
				ulDriverEntryToDispatchCodeOffset = 0x90;   //硬编码了，2003
				//ulOffset = (ULONG)PAtapiDriverObject->DriverStart - 0x10000;
				break;
			}
			ulDispatch = ulJmpAddress + ulDriverEntryToDispatchCodeOffset;

			if (DebugOn)
				KdPrint(("ulAddress:%08x\r\n",ulDispatch));

			ulReal_ATAPI_IRP_MJ_CREATE = *(PULONG)(ulDispatch+0x3D);
			ulReal_ATAPI_IRP_MJ_CLOSE = *(PULONG)(ulDispatch+0x3D);

			ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL = *(PULONG)(ulDispatch+0x18);
			ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL = *(PULONG)(ulDispatch+0x11);

			ulReal_ATAPI_IRP_MJ_POWER = *(PULONG)(ulDispatch+0x22);
			ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL = *(PULONG)(ulDispatch+0x36);

			ulReal_ATAPI_IRP_MJ_PNP_POWER = *(PULONG)(ulDispatch+0x2C);
			ulReal_DriverStartIo = *(PULONG)(ulDispatch+0x3);

			if (DebugOn)
				KdPrint(("%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n",
				ulReal_ATAPI_IRP_MJ_CREATE,
				ulReal_ATAPI_IRP_MJ_CLOSE,
				ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL,
				ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL,
				ulReal_ATAPI_IRP_MJ_POWER,
				ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL,
				ulReal_ATAPI_IRP_MJ_PNP_POWER,
				ulReal_DriverStartIo
				));
			if (Type == 1)
			{
				//填充结构
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,0,ulReal_ATAPI_IRP_MJ_CREATE,L"IRP_MJ_CREATE",IRP_MJ_CREATE);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,1,ulReal_ATAPI_IRP_MJ_CLOSE,L"IRP_MJ_CLOSE",IRP_MJ_CLOSE);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,2,ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL,L"IRP_MJ_DEVICE_CONTROL",IRP_MJ_DEVICE_CONTROL);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,3,ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL,L"IRP_MJ_INTERNAL_DEVICE_CONTROL",IRP_MJ_INTERNAL_DEVICE_CONTROL);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,4,ulReal_ATAPI_IRP_MJ_POWER,L"IRP_MJ_POWER",IRP_MJ_POWER);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,5,ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL,L"IRP_MJ_SYSTEM_CONTROL",IRP_MJ_SYSTEM_CONTROL);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,6,ulReal_ATAPI_IRP_MJ_PNP_POWER,L"IRP_MJ_PNP_POWER",IRP_MJ_PNP_POWER);
				FixFixAtapi(AtapiDispatchBakUp,PAtapiDriverObjectBakup,7,ulReal_DriverStartIo,L"DriverStartIo",IRP_DRIVER_START_IO);

				AtapiDispatchBakUp->ulCount = 8;
				return STATUS_SUCCESS;
			}
			//所有调用都走reload
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_ATAPI_IRP_MJ_CREATE - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_ATAPI_IRP_MJ_CLOSE - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_ATAPI_IRP_MJ_POWER - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_ATAPI_IRP_MJ_PNP_POWER - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
			PAtapiDriverObjectBakup->MajorFunction[IRP_DRIVER_START_IO] = ulReal_DriverStartIo - ulAtapiModuleBase + ulReLoadAtapiModuleBase;
		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ReLoadAtapiFree()
{
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
		return STATUS_UNSUCCESSFUL;

	if (MmIsAddressValidEx(PAtapiDriverObjectBakup))
	{
		if (ulReal_ATAPI_IRP_MJ_CREATE &&
			ulReal_ATAPI_IRP_MJ_CLOSE &&
			ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL &&
			ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL &&
			ulReal_ATAPI_IRP_MJ_POWER &&
			ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL &&
			ulReal_ATAPI_IRP_MJ_PNP_POWER &&
			ulReal_DriverStartIo)
		{
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_ATAPI_IRP_MJ_CREATE;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_ATAPI_IRP_MJ_CLOSE;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_ATAPI_IRP_MJ_DEVICE_CONTROL;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = ulReal_ATAPI_IRP_MJ_INTERNAL_DEVICE_CONTROL;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_ATAPI_IRP_MJ_POWER;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_ATAPI_IRP_MJ_SYSTEM_CONTROL;
			PAtapiDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_ATAPI_IRP_MJ_PNP_POWER;
			PAtapiDriverObjectBakup->MajorFunction[IRP_DRIVER_START_IO] = ulReal_DriverStartIo;
		}
	}
	return STATUS_SUCCESS;
}