#include "Kbdclass.h"

VOID SetKbdclassHook(ULONG ulNumber,ULONG ulRealDispatch)
{
	PDRIVER_OBJECT PKbdclassDriverObject = NULL;
	ULONG ulReloadDispatch;

	if (DebugOn)
		KdPrint(("%d:%08x\r\n",ulNumber,ulRealDispatch));

	if (MmIsAddressValidEx(PKbdclassDriverObjectBakup))
	{
		PKbdclassDriverObject = PKbdclassDriverObjectBakup;
		PKbdclassDriverObject->MajorFunction[ulNumber] = ulRealDispatch;   //恢复hook

// 		if (GetWindowsVersion() == WINDOWS_VERSION_7_7000 ||
// 			GetWindowsVersion() == WINDOWS_VERSION_7_7600_UP ||
// 			GetWindowsVersion() == WINDOWS_VERSION_XP)
// 		{
			ulReloadDispatch = ulRealDispatch - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;

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
		//}
	}
}
//填充各个结构发送至ring3
VOID FixFixKbdclass(PKBDCLASSDISPATCHBAKUP KbdclassDispatchBakUp,PDRIVER_OBJECT PKbdclassDriverObject,int i,ULONG ulReal_Dispatch,WCHAR *lpwzDispatchName,ULONG Dispatch)
{
	ULONG ulCurrentKbdclassDispatch;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	BOOL bIsHooked = FALSE;
	ULONG ulReloadKbdclassDispatch;

	KbdclassDispatchBakUp->KbdclassDispatch[i].ulKbdclassDispatch = ulReal_Dispatch;
	KbdclassDispatchBakUp->KbdclassDispatch[i].ulNumber = Dispatch;

	memset(KbdclassDispatchBakUp->KbdclassDispatch[i].lpwzKbdclassDispatchName,0,sizeof(KbdclassDispatchBakUp->KbdclassDispatch[0].lpwzKbdclassDispatchName));
	wcsncpy(KbdclassDispatchBakUp->KbdclassDispatch[i].lpwzKbdclassDispatchName,lpwzDispatchName,wcslen(lpwzDispatchName));

	ulCurrentKbdclassDispatch = PKbdclassDriverObject->MajorFunction[Dispatch];

	if (DebugOn)
		KdPrint(("ulCurrentKbdclassDispatch:%08x-%08x",ulCurrentKbdclassDispatch,PKbdclassDriverObject));

	if (ulCurrentKbdclassDispatch == ulReal_Dispatch)
	{
		bIsHooked = TRUE;

		ulReloadKbdclassDispatch = ulReal_Dispatch - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;

		//检查是否inline hook
		if (GetFunctionCodeSize(ulReal_Dispatch) == GetFunctionCodeSize(ulReloadKbdclassDispatch) &&
			memcmp(ulReal_Dispatch,ulReloadKbdclassDispatch,GetFunctionCodeSize(ulReal_Dispatch)) != NULL)
		{
			KbdclassDispatchBakUp->KbdclassDispatch[i].Hooked = 2; // inline hook

			//简单的处理一下头5字节的hook的检查
			ulCurrentKbdclassDispatch = *(PULONG)(ulReal_Dispatch+1)+(ULONG)(ulReal_Dispatch+5);
			//如果不是开头jmp hook，就写回原始地址
			if (!MmIsAddressValidEx(ulCurrentKbdclassDispatch))
			{
				ulCurrentKbdclassDispatch = ulReal_Dispatch;
			}
		}
	}
	if (!bIsHooked)
	{
		KbdclassDispatchBakUp->KbdclassDispatch[i].Hooked = 1;  // hook
	}
	memset(KbdclassDispatchBakUp->KbdclassDispatch[i].lpszBaseModule,0,sizeof(KbdclassDispatchBakUp->KbdclassDispatch[0].lpszBaseModule));

	if (!IsAddressInSystem(
		ulCurrentKbdclassDispatch,
		&ulHookModuleBase,
		&ulHookModuleSize,
		KbdclassDispatchBakUp->KbdclassDispatch[i].lpszBaseModule))
	{
		strcat(KbdclassDispatchBakUp->KbdclassDispatch[i].lpszBaseModule,"Unknown");
	}
	KbdclassDispatchBakUp->KbdclassDispatch[i].ulCurrentKbdclassDispatch = ulCurrentKbdclassDispatch;
	KbdclassDispatchBakUp->KbdclassDispatch[i].ulModuleSize = ulHookModuleSize;
	KbdclassDispatchBakUp->KbdclassDispatch[i].ulModuleBase = ulHookModuleBase;

}
NTSTATUS ReLoadKbdclass(PDRIVER_OBJECT DriverObject,PKBDCLASSDISPATCHBAKUP KbdclassDispatchBakUp,int Type)
{
	PDRIVER_OBJECT PKbdclassDriverObject = NULL;
	PUCHAR i;
	WIN_VER_DETAIL WinVer;
	BOOL bInit = FALSE;
	PUCHAR KbdclassDriverEntry;
	PUCHAR ulJmpAddress,ulAddress;
	ULONG ulDispatch;
	ULONG ulDriverEntryToDispatchCodeOffset;
	ULONG ulOffset;
	BOOL bIsReLoadSuccess = FALSE;
	UNICODE_STRING UnicodeModule;
	HANDLE hSection;
	ULONG ulModuleBase;

	//获取driverobject
	if (GetDriverObject(L"\\Driver\\Kbdclass",&PKbdclassDriverObject) == STATUS_SUCCESS)
	{
		PKbdclassDriverObjectBakup = PKbdclassDriverObject;

		ulKbdclassModuleBase = PKbdclassDriverObject->DriverStart;
		ulKbdclassModuleSize = PKbdclassDriverObject->DriverSize;

		//reload
		if (PeLoad(
			L"\\SystemRoot\\system32\\drivers\\Kbdclass.sys",
			&ulReLoadKbdclassModuleBase,
			DriverObject,
			ulKbdclassModuleBase
			))
		{
			bIsReLoadSuccess = TRUE;
		}
		if (!bIsReLoadSuccess)
		{
			return STATUS_UNSUCCESSFUL;
		}
		if (GetDriverEntryPoint(ulReLoadKbdclassModuleBase,&KbdclassDriverEntry))
		{
			/*
			IRP_MJ_CREATE		0xF875FDD0		-		0xF875FDD0		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_CLOSE		0xF875FFE0		-		0xF875FFE0		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_READ C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_FLUSH_BUFFERS		0xF875FD4A		-		0xF875FD4A		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_DEVICE_CONTROL	
			IRP_MJ_INTERNAL_DEVICE_CONTROL		0xF8761386		-		0xF8761386		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_CLEANUP		0xF875FD06		-		0xF875FD06		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_POWER		0xF8762180		-		0xF8762180		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_SYSTEM_CONTROL		0xF8761842		-		0xF8761842		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			IRP_MJ_PNP_POWER		0xF876078A		-		0xF876078A		C:\WINDOWS\system32\DRIVERS\kbdclass.sys
			*/
			/*
			8105e610 8bff            mov     edi,edi
			8105e612 55              push    ebp
			8105e613 8bec            mov     ebp,esp
			8105e615 a12c4095f8      mov     eax,dword ptr ds:[F895402Ch]
			8105e61a 85c0            test    eax,eax
			8105e61c b940bb0000      mov     ecx,0BB40h
			8105e621 7404            je      8105e627
			8105e623 3bc1            cmp     eax,ecx
			8105e625 7523            jne     8105e64a
			8105e627 8b15ec3e95f8    mov     edx,dword ptr ds:[0F8953EECh]
			8105e62d b82c4095f8      mov     eax,0F895402Ch
			8105e632 c1e808          shr     eax,8
			8105e635 3302            xor     eax,dword ptr [edx]
			8105e637 25ffff0000      and     eax,0FFFFh
			8105e63c a32c4095f8      mov     dword ptr ds:[F895402Ch],eax
			8105e641 7507            jne     8105e64a
			8105e643 8bc1            mov     eax,ecx
			8105e645 a32c4095f8      mov     dword ptr ds:[F895402Ch],eax
			8105e64a f7d0            not     eax
			8105e64c a3284095f8      mov     dword ptr ds:[F8954028h],eax
			8105e651 5d              pop     ebp
			8105e652 e9d9f9ffff      jmp     8105e030          <-----获取这里的地址 即driverentry(xp)
			*/
			for (i=(ULONG)KbdclassDriverEntry;i < (ULONG)KbdclassDriverEntry+0x1000;i++)
			{
				if (*i == 0xe9)
				{
					ulJmpAddress = *(PULONG)(i+1)+(ULONG)(i+5);
					if (MmIsAddressValidEx(ulJmpAddress))
					{
						if (DebugOn)
							KdPrint(("i:%08x,DriverEntry:%08x\n",i,ulJmpAddress));
						bInit = TRUE;
						break;
					}
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
				ulDriverEntryToDispatchCodeOffset = 0x2c2;   //硬编码了，xp
				break;
			case WINDOWS_VERSION_7_7000:
				ulDriverEntryToDispatchCodeOffset = 0x27C;   //硬编码了，win7 7000
				break;
			case WINDOWS_VERSION_7_7600_UP:
				ulDriverEntryToDispatchCodeOffset = 0x2BE;   //硬编码了，win7 7600 UP
				break;
			case WINDOWS_VERSION_2K3_SP1_SP2:
				ulDriverEntryToDispatchCodeOffset = 0x2c2;   //硬编码了，2003
				//ulOffset = (ULONG)PKbdclassDriverObject->DriverStart - 0x10000;
				break;
			}
			ulDispatch = ulJmpAddress + ulDriverEntryToDispatchCodeOffset;
			if (DebugOn)
				KdPrint(("ulAddress:%08x\r\n",ulDispatch));

			ulReal_KBDCLASS_IRP_MJ_CREATE = *(PULONG)(ulDispatch+3);
			ulReal_KBDCLASS_IRP_MJ_CLOSE = *(PULONG)(ulDispatch+0xA);
			ulReal_KBDCLASS_IRP_MJ_READ = *(PULONG)(ulDispatch+0x11);
			ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS = *(PULONG)(ulDispatch+0x18);
			ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL = *(PULONG)(ulDispatch+0x1F);
			ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL = *(PULONG)(ulDispatch+0x26);
			ulReal_KBDCLASS_IRP_MJ_CLEANUP = *(PULONG)(ulDispatch+0x30);
			ulReal_KBDCLASS_IRP_MJ_PNP_POWER = *(PULONG)(ulDispatch+0x3A);
			ulReal_KBDCLASS_IRP_MJ_POWER = *(PULONG)(ulDispatch+0x44);
			ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL = *(PULONG)(ulDispatch+0x4E);

			if (DebugOn)
				KdPrint(("%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n"
				"%08x\r\n",
				ulReal_KBDCLASS_IRP_MJ_CREATE,
				ulReal_KBDCLASS_IRP_MJ_CLOSE,
				ulReal_KBDCLASS_IRP_MJ_READ,
				ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS,
				ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL,
				ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL,
				ulReal_KBDCLASS_IRP_MJ_CLEANUP,
				ulReal_KBDCLASS_IRP_MJ_POWER,
				ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL,
				ulReal_KBDCLASS_IRP_MJ_PNP_POWER
				));
			if (Type == 1)
			{
				//填充结构
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,0,ulReal_KBDCLASS_IRP_MJ_CREATE,L"IRP_MJ_CREATE",IRP_MJ_CREATE);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,1,ulReal_KBDCLASS_IRP_MJ_CLOSE,L"IRP_MJ_CLOSE",IRP_MJ_CLOSE);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,2,ulReal_KBDCLASS_IRP_MJ_READ,L"IRP_MJ_READ",IRP_MJ_READ);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,3,ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS,L"IRP_MJ_FLUSH_BUFFERS",IRP_MJ_FLUSH_BUFFERS);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,4,ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL,L"IRP_MJ_DEVICE_CONTROL",IRP_MJ_DEVICE_CONTROL);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,5,ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL,L"IRP_MJ_INTERNAL_DEVICE_CONTROL",IRP_MJ_INTERNAL_DEVICE_CONTROL);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,6,ulReal_KBDCLASS_IRP_MJ_CLEANUP,L"IRP_MJ_CLEANUP",IRP_MJ_CLEANUP);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,7,ulReal_KBDCLASS_IRP_MJ_POWER,L"IRP_MJ_POWER",IRP_MJ_POWER);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,8,ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL,L"IRP_MJ_SYSTEM_CONTROL",IRP_MJ_SYSTEM_CONTROL);
				FixFixKbdclass(KbdclassDispatchBakUp,PKbdclassDriverObjectBakup,9,ulReal_KBDCLASS_IRP_MJ_PNP_POWER,L"IRP_MJ_PNP_POWER",IRP_MJ_PNP_POWER);
				KbdclassDispatchBakUp->ulCount = 10;
				return STATUS_SUCCESS;
			}
			//所有调用都走reload
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_KBDCLASS_IRP_MJ_CREATE - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_KBDCLASS_IRP_MJ_CLOSE - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_KBDCLASS_IRP_MJ_READ - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_KBDCLASS_IRP_MJ_CLEANUP - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_KBDCLASS_IRP_MJ_POWER - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_KBDCLASS_IRP_MJ_PNP_POWER - ulKbdclassModuleBase + ulReLoadKbdclassModuleBase;
		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ReLoadKbdclassFree()
{
	WIN_VER_DETAIL WinVer;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_2K3_SP1_SP2)
		return STATUS_UNSUCCESSFUL;

	if (MmIsAddressValidEx(PKbdclassDriverObjectBakup))
	{
		if (ulReal_KBDCLASS_IRP_MJ_CREATE &&
			ulReal_KBDCLASS_IRP_MJ_CLOSE &&
			ulReal_KBDCLASS_IRP_MJ_READ &&
			ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS &&
			ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL &&
			ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL &&
			ulReal_KBDCLASS_IRP_MJ_CLEANUP &&
			ulReal_KBDCLASS_IRP_MJ_POWER &&
			ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL &&
			ulReal_KBDCLASS_IRP_MJ_PNP_POWER)
		{
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_KBDCLASS_IRP_MJ_CREATE;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_KBDCLASS_IRP_MJ_CLOSE;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_KBDCLASS_IRP_MJ_READ;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_KBDCLASS_IRP_MJ_FLUSH_BUFFERS;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_KBDCLASS_IRP_MJ_DEVICE_CONTROL;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_INTERNAL_DEVICE_CONTROL] = ulReal_KBDCLASS_IRP_MJ_INTERNAL_DEVICE_CONTROL;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_KBDCLASS_IRP_MJ_CLEANUP;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_KBDCLASS_IRP_MJ_POWER;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_KBDCLASS_IRP_MJ_SYSTEM_CONTROL;
			PKbdclassDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_KBDCLASS_IRP_MJ_PNP_POWER;
		}
	}
	return STATUS_SUCCESS;
}