#include "Protect.h"


///////////////////////////////////////////////////////////////////////////
//搜素KiInsertQueueApc的地址，硬邦邦的编码啊~
///////////////////////////////////////////////////////////////////////////
ULONG QueryKiInsertQueueApcAddress()
{
	UNICODE_STRING UnicodeFunctionName;
	ULONG ulKeInsertQueueApc=NULL;

	ULONG ulKiInsertQueueApc=NULL;
	WIN_VER_DETAIL WinVer;
	BOOL bIsOK = FALSE;
	PUCHAR i;
	int count=0;
	ULONG ulCodeSize;

	RtlInitUnicodeString(&UnicodeFunctionName,L"KeInsertQueueApc");
	ulKeInsertQueueApc = MmGetSystemRoutineAddress(&UnicodeFunctionName);
	if (ulKeInsertQueueApc)
	{
		ulCodeSize = SizeOfProc((PVOID)ulKeInsertQueueApc);
		if (DebugOn)
			DbgPrint("ulKeInsertQueueApc success:%08x %x\r\n",ulKeInsertQueueApc,ulCodeSize);

		for (i=(ULONG)ulKeInsertQueueApc;i < i+ulCodeSize;i++)
		{
			WinVer=GetWindowsVersion();
			switch (WinVer)
			{
			case WINDOWS_VERSION_7_7000:
			case WINDOWS_VERSION_7_7600_UP:
				if (*i == 0xe8)
				{
					count++;
					if (count == 2)  //第二个call（e8）就是了
					{
						ulKiInsertQueueApc = *(PULONG)(i+1)+(ULONG)(i+5);
						if (MmIsAddressValidEx(ulKiInsertQueueApc))
						{
							if (DebugOn)
								DbgPrint("ulKiInsertQueueApc:%08x\r\n",ulKiInsertQueueApc);
							bIsOK = TRUE;
						}
					}
				}
				break;
			case WINDOWS_VERSION_XP:
			case WINDOWS_VERSION_2K3_SP1_SP2:
				if (*i == 0x89 &&
					*(i+2) == 0x28 &&
					*(i+3) == 0xe8)
				{
					i = i+3;

					ulKiInsertQueueApc = *(PULONG)(i+1)+(ULONG)(i+5);
					if (MmIsAddressValidEx(ulKiInsertQueueApc))
					{
						if (DebugOn)
							DbgPrint("ulKiInsertQueueApc:%08x\r\n",ulKiInsertQueueApc);
						bIsOK = TRUE;
					}
				}
				break;
			}
			if (bIsOK)
				break;
		}
	}else
		DbgPrint("ulKeInsertQueueApc:%08x\r\n",ulKeInsertQueueApc);

	return ulKiInsertQueueApc;
}
__declspec(naked) VOID KiInsertQueueApcHookZone(,...)
{
	_asm
	{
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		jmp [KiInsertQueueApcRet];
	}
}
__declspec(naked) VOID KeInsertQueueApcHookZone(,...)
{
	_asm
	{
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		_emit 0x90;
		jmp [KeInsertQueueApcRet];
	}
}
//IRQL等级太高，所以初始化偏移的事情就在过滤函数外面去做鸟~！
//for xp/2003
VOID FASTCALL NewKiInsertQueueApc(
	IN PKAPC Apc,
	IN KPRIORITY Increment
	)
{
	KIINSERTQUEUEAPC OldKiInsertQueueApc;

	ULONG ulThread;
	ULONG ulKapcState;
	ULONG ulProcess;
	ULONG ulSystemArgument1;
	ULONG ulCallerEprocess;

	//自行保存堆栈
	_asm
	{
		pushfd
		pushad
	}
	OldKiInsertQueueApc = (KIINSERTQUEUEAPC)KiInsertQueueApcHookZone;

	//如果退出了
	if (!bIsInitSuccess)
		goto _FuncTionRet;

	//默认不保护
	if (!bProtectProcess)
		goto _FuncTionRet;

	if (MmIsAddressValidEx(((ULONG)Apc + ulThreadOffset)))
	{
		ulThread = *(ULONG *)((ULONG)Apc + ulThreadOffset);
		if (MmIsAddressValidEx(ulThread))
		{
			//这里不能用IoThreadToProcess来获取eprocess
			//因为KiInsertQueueApc的IRQL太高，用IoThreadToProcess会蓝屏
			if (MmIsAddressValidEx(((ULONG)ulThread + ulApcStateOffset)))
			{
				ulKapcState = *(ULONG *)((ULONG)ulThread + ulApcStateOffset);
				if (MmIsAddressValidEx(((ULONG)ulKapcState + ulProcessOffset)))
				{
					ulProcess = *(ULONG *)((ULONG)ulKapcState + ulProcessOffset);
					if (MmIsAddressValidEx(ulProcess))
					{
						ulSystemArgument1 = *(ULONG *)((ULONG)Apc + ulSystemArgument1Offset);

						if (ulProcess == ProtectEProcess &&
							Increment == 0x2 &&
							(ULONG)Apc == ulSystemArgument1)
						{
							ulCallerEprocess = RPsGetCurrentProcess();

							//KdPrint(("APC:%08x-%08x",(ULONG)Apc,ulSystemArgument1));
							//KdPrint(("[%d]Caller:%08x-%08x",Increment,ulCallerEprocess,ProtectEProcess));

							//过滤csrss.exe
							if (ulCallerEprocess == AttachGuiEProcess)
								goto _FuncTionRet;

							if (LogDefenseInfo->ulCount < 1000)   //记录超过1000条，则不记录。
							{
								LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
								LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
								LogDefenseInfo->LogDefense[ulLogCount].Type = 1;
								ulLogCount++;
							}

							//自行恢复堆栈
							_asm
							{
								popad
								popfd
							}
							return;
						}
					}
				}
			}
		}
	}
_FuncTionRet:
	//自行恢复堆栈
	_asm
	{
		popad
		popfd
	}
	OldKiInsertQueueApc(
		Apc,
		Increment
		);
}
BOOLEAN __stdcall NewKeInsertQueueApc(
	__inout PRKAPC Apc,
	__in_opt PVOID SystemArgument1,
	__in_opt PVOID SystemArgument2,
	__in KPRIORITY Increment
	)
{
	KEINSERTQUEUEAPC OldKeInsertQueueApc;

	ULONG ulThread;
	ULONG ulKapcState;
	ULONG ulProcess;
	ULONG ulCallerEprocess;

	if (!bIsInitSuccess)
		goto _FuncTionRet;

	//默认不保护
	if (!bProtectProcess)
		goto _FuncTionRet;

	if (MmIsAddressValidEx(((ULONG)Apc + ulThreadOffset)))
	{
		ulThread = *(ULONG *)((ULONG)Apc + ulThreadOffset);
		if (MmIsAddressValidEx(ulThread))
		{
			if (MmIsAddressValidEx(((ULONG)ulThread + ulApcStateOffset)))
			{
				ulKapcState = *(ULONG *)((ULONG)ulThread + ulApcStateOffset);

				if (MmIsAddressValidEx(((ULONG)ulKapcState + ulProcessOffset)))
				{
					ulProcess = *(ULONG *)((ULONG)ulKapcState + ulProcessOffset);
					if (MmIsAddressValidEx(ulProcess))
					{
						if (ulProcess == ProtectEProcess &&
							Increment == 2 &&
							(ULONG)Apc == SystemArgument1)
						{
							ulCallerEprocess = RPsGetCurrentProcess();

							//KdPrint(("APC:%08x %x %x\n",(ULONG)Apc,SystemArgument1,SystemArgument2));
							//KdPrint(("[%d]Caller:%08x-%08x\n",Increment,ulCallerEprocess,ProtectEProcess));

							//过滤csrss.exe
							if (ulCallerEprocess == AttachGuiEProcess)
								goto _FuncTionRet;

							if (LogDefenseInfo->ulCount < 1000)   //记录超过1000条，则不记录。
							{
								LogDefenseInfo->LogDefense[ulLogCount].EProcess = RPsGetCurrentProcess();
								LogDefenseInfo->LogDefense[ulLogCount].ulPID = RPsGetCurrentProcessId();
								LogDefenseInfo->LogDefense[ulLogCount].Type = 1;
								ulLogCount++;
							}
							return FALSE;
						}
					}
				}
			}
		}
	}
_FuncTionRet:

	OldKeInsertQueueApc = (KEINSERTQUEUEAPC)KeInsertQueueApcHookZone;
	return OldKeInsertQueueApc(
		Apc,
		SystemArgument1,
		SystemArgument2,
		Increment
		);
}
BOOL ProtectCode()
{
	BOOL bRetOK = FALSE;
	PUCHAR ulKiInsertQueueApc;
	PUCHAR ulKeInsertQueueApc;
	ULONG ulReloadKiInsertQueueApc;
	ULONG ulReloadKeInsertQueueApc;
	
	KiInsertQueueApcHooked = FALSE;
	KeInsertQueueApcHooked = FALSE;

	//RPsGetCurrentProcess = ReLoadNtosCALL(L"PsGetCurrentProcess",SystemKernelModuleBase,ImageModuleBase);
	ReLoadNtosCALL(&RPsGetCurrentProcessId,L"PsGetCurrentProcessId",SystemKernelModuleBase,ImageModuleBase);
	//初始化偏移
	WinVer = GetWindowsVersion();
	switch (WinVer)
	{
	case WINDOWS_VERSION_2K3_SP1_SP2:
		ulThreadOffset = 0x008;
		ulApcStateOffset = 0x028;
		ulProcessOffset = 0x10;
		ulSystemArgument1Offset = 0x24;
		break;
	case WINDOWS_VERSION_XP:
		ulThreadOffset = 0x008;
		ulApcStateOffset = 0x034;
		ulProcessOffset = 0x10;
		ulSystemArgument1Offset = 0x24;
		break;
	case WINDOWS_VERSION_7_7600_UP:
	case WINDOWS_VERSION_7_7000:
		ulThreadOffset = 0x008;
		ulApcStateOffset = 0x040;
		ulProcessOffset = 0x10;
		ulSystemArgument1Offset = 0x24;
		break;
	}
	if (WinVer == WINDOWS_VERSION_2K3_SP1_SP2 ||
		WinVer == WINDOWS_VERSION_XP)
	{
		//得到reload的地址
		ulKiInsertQueueApc = QueryKiInsertQueueApcAddress();
		if (ulKiInsertQueueApc &&
			RPsGetCurrentProcess &&
			RPsGetCurrentProcessId)
		{
			ulReloadKiInsertQueueApc = (ULONG)ulKiInsertQueueApc - SystemKernelModuleBase + ImageModuleBase;
			if (MmIsAddressValidEx(ulReloadKiInsertQueueApc))
			{
				//屌，已经被hook过了
				if (*ulKiInsertQueueApc == 0xE9)
				{
					//获取被hook的跳转地址
					ulKiInsertQueueApc = *(PULONG)((ULONG)ulKiInsertQueueApc+1)+(ULONG)((ULONG)ulKiInsertQueueApc+5);
				}
				//hook KiInsertQueueApc，让所有调用都走reload的KiInsertQueueApc
				bRetOK = HookFunctionByHeaderAddress(
					ulReloadKiInsertQueueApc,
					ulKiInsertQueueApc,
					KiInsertQueueApcHookZone,
					&KiInsertQueueApcPatchCodeLen,
					&KiInsertQueueApcRet
					);
				if (bRetOK)
				{
					//hook ulReloadKiInsertQueueApc，让所有调用都走NewKiInsertQueueApc
					bRetOK = HookFunctionByHeaderAddress(
						(DWORD)NewKiInsertQueueApc,
						ulReloadKiInsertQueueApc,
						KiInsertQueueApcHookZone,
						&KiInsertQueueApcPatchCodeLen,
						&KiInsertQueueApcRet
						);
					if (bRetOK)
					{
						KiInsertQueueApcHooked = TRUE;
						return KiInsertQueueApcHooked;
					}
				}
			}
		}

	}else if (WinVer == WINDOWS_VERSION_7_7000 || WinVer == WINDOWS_VERSION_7_7600_UP)
	{
		//hook KeInsertQueueApc
		ulKeInsertQueueApc = GetSystemRoutineAddress(0,"KeInsertQueueApc");
		if (ulKeInsertQueueApc &&
			RPsGetCurrentProcess)
		{
			ulReloadKeInsertQueueApc = (ULONG)ulKeInsertQueueApc - SystemKernelModuleBase + ImageModuleBase;
			if (MmIsAddressValidEx(ulReloadKeInsertQueueApc))
			{
				//屌，已经被hook过了
				if (*ulKeInsertQueueApc == 0xE9)
				{
					//获取被hook的跳转地址
					ulKeInsertQueueApc = *(PULONG)((ULONG)ulKeInsertQueueApc+1)+(ULONG)((ULONG)ulKeInsertQueueApc+5);
				}
				bRetOK = HookFunctionByHeaderAddress(
					ulReloadKeInsertQueueApc,
					ulKeInsertQueueApc,
					KeInsertQueueApcHookZone,
					&KeInsertQueueApcPatchCodeLen,
					&KeInsertQueueApcRet
					);
				if (bRetOK)
				{
					bRetOK = HookFunctionByHeaderAddress(
						(DWORD)NewKeInsertQueueApc,
						ulReloadKeInsertQueueApc,
						KeInsertQueueApcHookZone,
						&KeInsertQueueApcPatchCodeLen,
						&KeInsertQueueApcRet
						);
					if (bRetOK)
					{
						KeInsertQueueApcHooked = TRUE;
						return KeInsertQueueApcHooked;
					}
				}
			}
		}
	}
	return FALSE;
}
/*
PSYSTEM_HANDLE_INFORMATION_EX GetInfoTable(OUT PULONG nSize)
{
	PVOID Buffer;
	NTSTATUS status;
	Buffer =ExAllocatePool(PagedPool,0x1000);
	status = ZwQuerySystemInformation(SystemHandleInformation, Buffer, 0x1000, nSize);
	ExFreePool(Buffer);
	if(status == STATUS_INFO_LENGTH_MISMATCH)
	{
		Buffer = ExAllocatePool(NonPagedPool, *nSize);
		status = ZwQuerySystemInformation(SystemHandleInformation, Buffer, *nSize, NULL);
		if(NT_SUCCESS(status))
		{
			return (PSYSTEM_HANDLE_INFORMATION_EX)Buffer;
		}
	}
	return (PSYSTEM_HANDLE_INFORMATION_EX)0;
}
HANDLE GetCsrPid(VOID)
{
	HANDLE Process,hObject;
	HANDLE CsrId = (HANDLE)0;
	OBJECT_ATTRIBUTES obj;
	CLIENT_ID cid;
	UCHAR Buff[0x100];
	POBJECT_NAME_INFORMATION ObjName = (PVOID)&Buff;
	PSYSTEM_HANDLE_INFORMATION_EX Handles;
	ULONG i;
	ULONG nSize;

	Handles = GetInfoTable(&nSize);
	if(!Handles)
	{
		return CsrId;
	}
	for(i = 0; i < Handles->NumberOfHandles; i++)
	{

		if(Handles->Information[i].ObjectTypeNumber == 21)
		{
			InitializeObjectAttributes(&obj, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
			cid.UniqueProcess = (HANDLE)Handles->Information[i].ProcessID;
			cid.UniqueThread  = 0;
			if(NT_SUCCESS(NtOpenProcess(&Process, PROCESS_DUP_HANDLE, &obj, &cid)))
			{
				if(NT_SUCCESS(ZwDuplicateObject(Process, (HANDLE)Handles->Information[i].Handle, NtCurrentProcess(), &hObject, 0, 0, DUPLICATE_SAME_ACCESS)))
				{
					if(NT_SUCCESS(ZwQueryObject(hObject, ObjectNameInformation, ObjName, 0x100, NULL)))
					{
						//!!!!!
						//win7下没有"\\Windows\\ApiPort"
						if(ObjName->Name.Buffer && !wcsncmp(L"\\Windows\\ApiPort", ObjName->Name.Buffer, 20))
						{
							CsrId = (HANDLE)Handles->Information[i].ProcessID;
							if (DebugOn)
								KdPrint(("Csrss.exe PID = %d", CsrId));
						}
					}
					ZwClose(hObject);
				}
				ZwClose(Process);
			}
		}
	}
	ExFreePool(Handles);
	return CsrId;
}
*/