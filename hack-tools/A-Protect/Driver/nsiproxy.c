#include "Nsiproxy.h"

VOID SetNsiproxyHook(ULONG ulNumber,ULONG ulRealDispatch)
{
	PDRIVER_OBJECT PNsiproxyDriverObject = NULL;
	ULONG ulReloadDispatch;

	if (MmIsAddressValidEx(PNsiproxyDriverObjectBakup))
	{
		PNsiproxyDriverObject = PNsiproxyDriverObjectBakup;
		PNsiproxyDriverObject->MajorFunction[ulNumber] = ulRealDispatch;   //恢复 fsd hook

		ulReloadDispatch = ulRealDispatch - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;

		//恢复 fsd inline hook
		if (!MmIsAddressValidEx(ulReloadDispatch) ||
			GetFunctionCodeSize(ulRealDispatch) != GetFunctionCodeSize(ulReloadDispatch))
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
//填充各个结构发送至ring3
VOID FixFixNsiproxy(PNSIPROXYDISPATCHBAKUP NsiproxyDispatchBakUp,PDRIVER_OBJECT PNsiproxyDriverObject,int i,ULONG ulReal_Dispatch,WCHAR *lpwzDispatchName,ULONG Dispatch)
{
	ULONG ulCurrentNsiproxyDispatch;
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	BOOL bIsHooked = FALSE;
	ULONG ulReloadNsiproxyDispatch;

	NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNsiproxyDispatch = ulReal_Dispatch;
	NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNumber = Dispatch;

	memset(NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpwzNsiproxyDispatchName,0,sizeof(NsiproxyDispatchBakUp->NsiproxyDispatch[0].lpwzNsiproxyDispatchName));
	wcsncpy(NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpwzNsiproxyDispatchName,lpwzDispatchName,wcslen(lpwzDispatchName));

	ulCurrentNsiproxyDispatch = PNsiproxyDriverObject->MajorFunction[Dispatch];

	if (DebugOn)
		KdPrint(("ulCurrentNsiproxyDispatch:%08x-%08x",ulCurrentNsiproxyDispatch,PNsiproxyDriverObject));

	if (ulCurrentNsiproxyDispatch == ulReal_Dispatch)
	{
		bIsHooked = TRUE;

		ulReloadNsiproxyDispatch = ulReal_Dispatch - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;

		//检查是否inline hook
		if (GetFunctionCodeSize(ulReal_Dispatch) == GetFunctionCodeSize(ulReloadNsiproxyDispatch) &&
			memcmp(ulReal_Dispatch,ulReloadNsiproxyDispatch,GetFunctionCodeSize(ulReal_Dispatch)) != NULL)
		{
			NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked = 2; //fsd inline hook

			//简单的处理一下头5字节的hook的检查
			ulCurrentNsiproxyDispatch = *(PULONG)(ulReal_Dispatch+1)+(ULONG)(ulReal_Dispatch+5);
			//如果不是开头jmp hook，就写回原始地址
			if (!MmIsAddressValidEx(ulCurrentNsiproxyDispatch))
			{
				ulCurrentNsiproxyDispatch = ulReal_Dispatch;
			}
		}
	}

	if (!bIsHooked)
	{
		NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked = 1;  //fsd hook
	}
	memset(NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule,0,sizeof(NsiproxyDispatchBakUp->NsiproxyDispatch[0].lpszBaseModule));

	if (!IsAddressInSystem(
		ulCurrentNsiproxyDispatch,
		&ulHookModuleBase,
		&ulHookModuleSize,
		NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule))
	{
		strcat(NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule,"Unknown");
	}
	NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulCurrentNsiproxyDispatch = ulCurrentNsiproxyDispatch;
	NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulModuleSize = ulHookModuleSize;
	NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulModuleBase = ulHookModuleBase;

}
NTSTATUS ReLoadNsiproxy(PDRIVER_OBJECT DriverObject,PNSIPROXYDISPATCHBAKUP NsiproxyDispatchBakUp,int Type)
{
	PDRIVER_OBJECT PNsiproxyDriverObject = NULL;
	PUCHAR i;
	WIN_VER_DETAIL WinVer;
	BOOL bInit = FALSE;
	PUCHAR NsiproxyDriverEntry;
	PUCHAR ulJmpAddress,ulAddress;
	ULONG ulDispatch;
	BOOL bIsWin7 = FALSE;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 ||
		WinVer == WINDOWS_VERSION_7_7600_UP){
			bIsWin7 = TRUE;
	}
	if (!bIsWin7){
		return STATUS_UNSUCCESSFUL;
	}

	//获取driverobject
	if (GetDriverObject(L"\\Driver\\nsiproxy",&PNsiproxyDriverObject) == STATUS_SUCCESS)
	{
		PNsiproxyDriverObjectBakup = PNsiproxyDriverObject;

		ulNsiproxyModuleBase = PNsiproxyDriverObject->DriverStart;
		ulNsiproxyModuleSize = PNsiproxyDriverObject->DriverSize;

		//reload
		if (PeLoad(
			L"\\SystemRoot\\system32\\drivers\\Nsiproxy.sys",
			&ulReLoadNsiproxyModuleBase,
			DriverObject,
			ulNsiproxyModuleBase
			))
		{
			if (GetDriverEntryPoint(ulReLoadNsiproxyModuleBase,&NsiproxyDriverEntry))
			{
				//KdPrint(("dispatch:%08x\n",NsiproxyDriverEntry));
				/*
				lkd> u 8720f071
				8720f071 8bff            mov     edi,edi
				8720f073 55              push    ebp
				8720f074 8bec            mov     ebp,esp
				8720f076 e8bdffffff      call    8720f038
				8720f07b 5d              pop     ebp
				8720f07c e985ffffff      jmp     8720f006              <-----获取这里的地址
				8720f081 cc              int     3
				8720f082 cc              int     3
				*/
				for (i=(ULONG)NsiproxyDriverEntry;i < (ULONG)NsiproxyDriverEntry+0x1000;i++)
				{
					if (*i == 0xe9)
					{
						ulJmpAddress = *(PULONG)(i+1)+(ULONG)(i+5);
						if (MmIsAddressValidEx(ulJmpAddress))
						{
							//KdPrint(("i:%08x,ulJmpAddress:%08x\n",i,ulJmpAddress));
							bInit = TRUE;
							break;
						}
					}
				}
				if (bInit)
				{
					/*
					lkd> u 8720f006
					8720f006 8bff            mov     edi,edi
					8720f008 55              push    ebp
					8720f009 8bec            mov     ebp,esp
					8720f00b 8b4508          mov     eax,dword ptr [ebp+8]
					8720f00e 56              push    esi
					8720f00f 50              push    eax
					8720f010 c740340600b48f  mov     dword ptr [eax+34h],8FB40006h
					8720f017 e836c0ffff      call    8720b052            <-------------获取
					*/
					for (i=(ULONG)ulJmpAddress;i < (ULONG)ulJmpAddress+0x1000;i++)
					{
						if (*i == 0xe8)
						{
							ulAddress = *(PULONG)(i+1)+(ULONG)(i+5);
							//KdPrint(("i:%08x,ulAddress:%08x\n",i,ulAddress));
							/*
							lkd> u  8720b052 l 50
							8720b052 8bff            mov     edi,edi
							8720b054 55              push    ebp
							8720b055 8bec            mov     ebp,esp
							8720b057 83ec10          sub     esp,10h
							8720b05a 56              push    esi
							8720b05b 8b353ce0b38f    mov     esi,dword ptr ds:[8FB3E03Ch]
							8720b061 57              push    edi
							8720b062 6836d5b38f      push    8FB3D536h
							8720b067 8d45f8          lea     eax,[ebp-8]
							8720b06a 50              push    eax
							8720b06b ffd6            call    esi
							8720b06d 8b7d08          mov     edi,dword ptr [ebp+8]
							8720b070 6820f0b38f      push    8FB3F020h
							8720b075 6a00            push    0
							8720b077 6800010000      push    100h
							8720b07c 6a12            push    12h
							8720b07e 8d45f8          lea     eax,[ebp-8]
							8720b081 50              push    eax
							8720b082 6a00            push    0
							8720b084 57              push    edi
							8720b085 ff1560e0b38f    call    dword ptr ds:[8FB3E060h]
							8720b08b 85c0            test    eax,eax
							8720b08d 7c4b            jl      8720b0da
							8720b08f 6826d5b38f      push    8FB3D526h
							8720b094 8d45f0          lea     eax,[ebp-10h]
							8720b097 50              push    eax
							8720b098 ffd6            call    esi
							8720b09a 8d45f8          lea     eax,[ebp-8]
							8720b09d 50              push    eax
							8720b09e 8d45f0          lea     eax,[ebp-10h]
							8720b0a1 50              push    eax
							8720b0a2 ff155ce0b38f    call    dword ptr ds:[8FB3E05Ch]
							8720b0a8 8bf0            mov     esi,eax
							8720b0aa a120f0b38f      mov     eax,dword ptr ds:[8FB3F020h]
							8720b0af 85f6            test    esi,esi
							8720b0b1 7d14            jge     8720b0c7
							8720b0b3 85c0            test    eax,eax
							8720b0b5 7421            je      8720b0d8
							8720b0b7 50              push    eax
							8720b0b8 ff156ce0b38f    call    dword ptr ds:[8FB3E06Ch]
							8720b0be 832520f0b38f00  and     dword ptr ds:[8FB3F020h],0
							8720b0c5 eb11            jmp     8720b0d8
							8720b0c7 83481c10        or      dword ptr [eax+1Ch],10h
							8720b0cb 6a1c            push    1Ch
							8720b0cd 83c738          add     edi,38h
							8720b0d0 59              pop     ecx
							8720b0d1 b8b2cfb38f      mov     eax,8FB3CFB2h     <--------------------------

							*/
							ulAddress = ulAddress + 0x7F;
							ulDispatch = *(PULONG)(ulAddress+1);
							break;
						}
					}
				}
				ulReal_NSIPROXY_IRP_MJ_CREATE = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_CREATE_NAMED_PIPE = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_CLOSE = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_READ = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_WRITE = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_QUERY_INFORMATION = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SET_INFORMATION = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_QUERY_EA = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SET_EA = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_FLUSH_BUFFERS = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_QUERY_VOLUME_INFORMATION = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SET_VOLUME_INFORMATION = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_DIRECTORY_CONTROL = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_FILE_SYSTEM_CONTROL = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_DEVICE_CONTROL = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SHUTDOWN = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_LOCK_CONTROL = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_CLEANUP = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_CREATE_MAILSLOT = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_QUERY_SECURITY = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SET_SECURITY = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_POWER = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SYSTEM_CONTROL = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_DEVICE_CHANGE = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_QUERY_QUOTA = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_SET_QUOTA = ulDispatch;
				ulReal_NSIPROXY_IRP_MJ_PNP_POWER = ulDispatch;

				if (Type == 1)
				{
					//填充结构
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,0,ulReal_NSIPROXY_IRP_MJ_CREATE,L"IRP_MJ_CREATE",IRP_MJ_CREATE);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,1,ulReal_NSIPROXY_IRP_MJ_CREATE_NAMED_PIPE,L"IRP_MJ_CREATE_NAMED_PIPE",IRP_MJ_CREATE_NAMED_PIPE);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,2,ulReal_NSIPROXY_IRP_MJ_CLOSE,L"IRP_MJ_CLOSE",IRP_MJ_CLOSE);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,3,ulReal_NSIPROXY_IRP_MJ_READ,L"IRP_MJ_READ",IRP_MJ_READ);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,4,ulReal_NSIPROXY_IRP_MJ_WRITE,L"IRP_MJ_WRITE",IRP_MJ_WRITE);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,5,ulReal_NSIPROXY_IRP_MJ_QUERY_INFORMATION,L"IRP_MJ_QUERY_INFORMATION",IRP_MJ_QUERY_INFORMATION);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,6,ulReal_NSIPROXY_IRP_MJ_SET_INFORMATION,L"IRP_MJ_SET_INFORMATION",IRP_MJ_SET_INFORMATION);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,7,ulReal_NSIPROXY_IRP_MJ_QUERY_EA,L"IRP_MJ_QUERY_EA",IRP_MJ_QUERY_EA);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,8,ulReal_NSIPROXY_IRP_MJ_SET_EA,L"IRP_MJ_SET_EA",IRP_MJ_SET_EA);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,9,ulReal_NSIPROXY_IRP_MJ_FLUSH_BUFFERS,L"IRP_MJ_FLUSH_BUFFERS",IRP_MJ_FLUSH_BUFFERS);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,10,ulReal_NSIPROXY_IRP_MJ_QUERY_VOLUME_INFORMATION,L"IRP_MJ_QUERY_VOLUME_INFORMATION",IRP_MJ_QUERY_VOLUME_INFORMATION);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,11,ulReal_NSIPROXY_IRP_MJ_SET_VOLUME_INFORMATION,L"IRP_MJ_SET_VOLUME_INFORMATION",IRP_MJ_SET_VOLUME_INFORMATION);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,12,ulReal_NSIPROXY_IRP_MJ_DIRECTORY_CONTROL,L"IRP_MJ_DIRECTORY_CONTROL",IRP_MJ_DIRECTORY_CONTROL);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,13,ulReal_NSIPROXY_IRP_MJ_FILE_SYSTEM_CONTROL,L"IRP_MJ_FILE_SYSTEM_CONTROL",IRP_MJ_FILE_SYSTEM_CONTROL);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,14,ulReal_NSIPROXY_IRP_MJ_DEVICE_CONTROL,L"IRP_MJ_DEVICE_CONTROL",IRP_MJ_DEVICE_CONTROL);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,15,ulReal_NSIPROXY_IRP_MJ_SHUTDOWN,L"IRP_MJ_SHUTDOWN",IRP_MJ_SHUTDOWN);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,16,ulReal_NSIPROXY_IRP_MJ_LOCK_CONTROL,L"IRP_MJ_LOCK_CONTROL",IRP_MJ_LOCK_CONTROL);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,17,ulReal_NSIPROXY_IRP_MJ_CLEANUP,L"IRP_MJ_CLEANUP",IRP_MJ_CLEANUP);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,18,ulReal_NSIPROXY_IRP_MJ_CREATE_MAILSLOT,L"IRP_MJ_CREATE_MAILSLOT",IRP_MJ_CREATE_MAILSLOT);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,19,ulReal_NSIPROXY_IRP_MJ_QUERY_SECURITY,L"IRP_MJ_QUERY_SECURITY",IRP_MJ_QUERY_SECURITY);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,20,ulReal_NSIPROXY_IRP_MJ_SET_SECURITY,L"IRP_MJ_SET_SECURITY",IRP_MJ_SET_SECURITY);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,21,ulReal_NSIPROXY_IRP_MJ_POWER,L"IRP_MJ_POWER",IRP_MJ_POWER);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,22,ulReal_NSIPROXY_IRP_MJ_SYSTEM_CONTROL,L"IRP_MJ_SYSTEM_CONTROL",IRP_MJ_SYSTEM_CONTROL);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,23,ulReal_NSIPROXY_IRP_MJ_DEVICE_CHANGE,L"IRP_MJ_DEVICE_CHANGE",IRP_MJ_DEVICE_CHANGE);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,24,ulReal_NSIPROXY_IRP_MJ_QUERY_QUOTA,L"IRP_MJ_QUERY_QUOTA",IRP_MJ_QUERY_QUOTA);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,25,ulReal_NSIPROXY_IRP_MJ_SET_QUOTA,L"IRP_MJ_SET_QUOTA",IRP_MJ_SET_QUOTA);
					FixFixNsiproxy(NsiproxyDispatchBakUp,PNsiproxyDriverObjectBakup,26,ulReal_NSIPROXY_IRP_MJ_PNP_POWER,L"IRP_MJ_PNP_POWER",IRP_MJ_PNP_POWER);
					NsiproxyDispatchBakUp->ulCount = 27;
					return STATUS_SUCCESS;
				}
				//所有调用都走reload
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_NSIPROXY_IRP_MJ_CREATE - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = ulReal_NSIPROXY_IRP_MJ_CREATE_NAMED_PIPE - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_NSIPROXY_IRP_MJ_CLOSE - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_NSIPROXY_IRP_MJ_READ - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_NSIPROXY_IRP_MJ_WRITE - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_QUERY_INFORMATION - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_SET_INFORMATION - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_EA] = ulReal_NSIPROXY_IRP_MJ_QUERY_EA - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_EA] = ulReal_NSIPROXY_IRP_MJ_SET_EA - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_NSIPROXY_IRP_MJ_FLUSH_BUFFERS - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_QUERY_VOLUME_INFORMATION - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_VOLUME_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_SET_VOLUME_INFORMATION - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_NSIPROXY_IRP_MJ_DIRECTORY_CONTROL - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_NSIPROXY_IRP_MJ_FILE_SYSTEM_CONTROL - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_NSIPROXY_IRP_MJ_DEVICE_CONTROL - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_NSIPROXY_IRP_MJ_SHUTDOWN - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_NSIPROXY_IRP_MJ_LOCK_CONTROL - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_NSIPROXY_IRP_MJ_CLEANUP - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = ulReal_NSIPROXY_IRP_MJ_CREATE_MAILSLOT - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_SECURITY] = ulReal_NSIPROXY_IRP_MJ_QUERY_SECURITY - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_SECURITY] = ulReal_NSIPROXY_IRP_MJ_SET_SECURITY - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_NSIPROXY_IRP_MJ_POWER - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_NSIPROXY_IRP_MJ_SYSTEM_CONTROL - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CHANGE] = ulReal_NSIPROXY_IRP_MJ_DEVICE_CHANGE - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_QUOTA] = ulReal_NSIPROXY_IRP_MJ_QUERY_QUOTA - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_QUOTA] = ulReal_NSIPROXY_IRP_MJ_SET_QUOTA - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
				PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_NSIPROXY_IRP_MJ_PNP_POWER - ulNsiproxyModuleBase + ulReLoadNsiproxyModuleBase;
			}
		}
	}
	return STATUS_SUCCESS;
}
NTSTATUS ReLoadNsiproxyFree()
{
	WIN_VER_DETAIL WinVer;

	BOOL bIsWin7 = FALSE;

	WinVer = GetWindowsVersion();
	if (WinVer == WINDOWS_VERSION_7_7000 ||
		WinVer == WINDOWS_VERSION_7_7600_UP){
			bIsWin7 = TRUE;
	}
	if (!bIsWin7){
		return STATUS_UNSUCCESSFUL;
	}

	if (MmIsAddressValidEx(PNsiproxyDriverObjectBakup))
	{
		if (ulReal_NSIPROXY_IRP_MJ_CREATE &&
			ulReal_NSIPROXY_IRP_MJ_CREATE_NAMED_PIPE &&
			ulReal_NSIPROXY_IRP_MJ_CLOSE &&
			ulReal_NSIPROXY_IRP_MJ_READ &&
			ulReal_NSIPROXY_IRP_MJ_WRITE &&
			ulReal_NSIPROXY_IRP_MJ_QUERY_INFORMATION &&
			ulReal_NSIPROXY_IRP_MJ_SET_INFORMATION &&
			ulReal_NSIPROXY_IRP_MJ_QUERY_EA &&
			ulReal_NSIPROXY_IRP_MJ_SET_EA &&
			ulReal_NSIPROXY_IRP_MJ_FLUSH_BUFFERS &&
			ulReal_NSIPROXY_IRP_MJ_QUERY_VOLUME_INFORMATION &&
			ulReal_NSIPROXY_IRP_MJ_SET_VOLUME_INFORMATION &&
			ulReal_NSIPROXY_IRP_MJ_DIRECTORY_CONTROL &&
			ulReal_NSIPROXY_IRP_MJ_FILE_SYSTEM_CONTROL &&
			ulReal_NSIPROXY_IRP_MJ_DEVICE_CONTROL &&
			ulReal_NSIPROXY_IRP_MJ_SHUTDOWN &&
			ulReal_NSIPROXY_IRP_MJ_LOCK_CONTROL &&
			ulReal_NSIPROXY_IRP_MJ_CLEANUP &&
			ulReal_NSIPROXY_IRP_MJ_CREATE_MAILSLOT &&
			ulReal_NSIPROXY_IRP_MJ_QUERY_SECURITY &&
			ulReal_NSIPROXY_IRP_MJ_SET_SECURITY &&
			ulReal_NSIPROXY_IRP_MJ_POWER &&
			ulReal_NSIPROXY_IRP_MJ_SYSTEM_CONTROL &&
			ulReal_NSIPROXY_IRP_MJ_DEVICE_CHANGE &&
			ulReal_NSIPROXY_IRP_MJ_QUERY_QUOTA &&
			ulReal_NSIPROXY_IRP_MJ_SET_QUOTA &&
			ulReal_NSIPROXY_IRP_MJ_PNP_POWER)
		{
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CREATE] = ulReal_NSIPROXY_IRP_MJ_CREATE;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_NAMED_PIPE] = ulReal_NSIPROXY_IRP_MJ_CREATE_NAMED_PIPE;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CLOSE] = ulReal_NSIPROXY_IRP_MJ_CLOSE;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_READ] = ulReal_NSIPROXY_IRP_MJ_READ;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_WRITE] = ulReal_NSIPROXY_IRP_MJ_WRITE;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_QUERY_INFORMATION;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_SET_INFORMATION;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_EA] = ulReal_NSIPROXY_IRP_MJ_QUERY_EA;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_EA] = ulReal_NSIPROXY_IRP_MJ_SET_EA;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_FLUSH_BUFFERS] = ulReal_NSIPROXY_IRP_MJ_FLUSH_BUFFERS;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_VOLUME_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_QUERY_VOLUME_INFORMATION;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_VOLUME_INFORMATION] = ulReal_NSIPROXY_IRP_MJ_SET_VOLUME_INFORMATION;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_DIRECTORY_CONTROL] = ulReal_NSIPROXY_IRP_MJ_DIRECTORY_CONTROL;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_FILE_SYSTEM_CONTROL] = ulReal_NSIPROXY_IRP_MJ_FILE_SYSTEM_CONTROL;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CONTROL] = ulReal_NSIPROXY_IRP_MJ_DEVICE_CONTROL;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SHUTDOWN] = ulReal_NSIPROXY_IRP_MJ_SHUTDOWN;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_LOCK_CONTROL] = ulReal_NSIPROXY_IRP_MJ_LOCK_CONTROL;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CLEANUP] = ulReal_NSIPROXY_IRP_MJ_CLEANUP;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_CREATE_MAILSLOT] = ulReal_NSIPROXY_IRP_MJ_CREATE_MAILSLOT;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_SECURITY] = ulReal_NSIPROXY_IRP_MJ_QUERY_SECURITY;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_SECURITY] = ulReal_NSIPROXY_IRP_MJ_SET_SECURITY;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_POWER] = ulReal_NSIPROXY_IRP_MJ_POWER;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SYSTEM_CONTROL] = ulReal_NSIPROXY_IRP_MJ_SYSTEM_CONTROL;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_DEVICE_CHANGE] = ulReal_NSIPROXY_IRP_MJ_DEVICE_CHANGE;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_QUERY_QUOTA] = ulReal_NSIPROXY_IRP_MJ_QUERY_QUOTA;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_SET_QUOTA] = ulReal_NSIPROXY_IRP_MJ_SET_QUOTA;
			PNsiproxyDriverObjectBakup->MajorFunction[IRP_MJ_PNP_POWER] = ulReal_NSIPROXY_IRP_MJ_PNP_POWER;
		}
	}
	return STATUS_SUCCESS;
}