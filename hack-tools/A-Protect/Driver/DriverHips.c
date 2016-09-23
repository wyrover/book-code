#include "DriverHips.h"

__declspec(naked) BOOLEAN SeSinglePrivilegeCheckHookZone(,...)
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
		jmp [SeSinglePrivilegeCheckRet];
	}
}
//权限检查的时候返回失败来达到禁止加载驱动
BOOLEAN __stdcall NewSeSinglePrivilegeCheck(
	__in  LUID PrivilegeValue,
	__in  KPROCESSOR_MODE PreviousMode
	)
{
	ULONG ulPage;

	if (!bIsInitSuccess)
		goto _FunctionRet;

	//取返回地址
	_asm
	{
		mov eax,dword ptr[ebp+4]
		mov ulPage,eax
	}
	if (RPsGetCurrentProcess() == ProtectEProcess){
		goto _FunctionRet;
	}
	//开启内核安全模式，则判断的是Reload后的地址
	if (bKernelSafeModule){
		if (ulPage >= ulReloadNtLoadDriverBase && ulPage <= ulReloadNtLoadDriverBase+ulNtLoadDriverSize)
			return FALSE;

		if (ulPage >= ulReloadZwSetSystemInformationBase && ulPage <= ulReloadZwSetSystemInformationBase+ulZwSetSystemInformationSize)
			return FALSE;
	}else{

		//没有开启内核安全模式，则判断的是原始地址
		if (ulPage >= ulNtLoadDriverBase && ulPage <= ulNtLoadDriverBase+ulNtLoadDriverSize)
			return FALSE;

		if (ulPage >= ulZwSetSystemInformationBase && ulPage <= ulZwSetSystemInformationBase+ulZwSetSystemInformationSize)
			return FALSE;
	}

_FunctionRet:
	OldSeSinglePrivilegeCheck = (SeSinglePrivilegeCheck_1)SeSinglePrivilegeCheckHookZone;
	return OldSeSinglePrivilegeCheck(
		PrivilegeValue,
		PreviousMode
		);
}
//禁止驱动加载
NTSTATUS DisEnableDriverLoading()
{
	int bRet;

	ulZwSetSystemInformationBase = GetSystemRoutineAddress(1,L"ZwSetSystemInformation");
	ulNtLoadDriverBase = GetSystemRoutineAddress(1,L"ZwLoadDriver");
	if (ulNtLoadDriverBase &&
		ulZwSetSystemInformationBase)
	{
		ulNtLoadDriverSize = SizeOfProc(ulNtLoadDriverBase);
		ulZwSetSystemInformationSize = SizeOfProc(ulZwSetSystemInformationBase);
	}

	ulSeSinglePrivilegeCheck = GetSystemRoutineAddress(1,L"SeSinglePrivilegeCheck");
	if (!ulSeSinglePrivilegeCheck ||
		!ulNtLoadDriverBase ||
		!ulZwSetSystemInformationBase)
	{
		return STATUS_UNSUCCESSFUL;
	}
	//如果开启了内核安全模式，则要计算reload后的地址，不然判断不对
	ulReloadNtLoadDriverBase = ulNtLoadDriverBase - SystemKernelModuleBase+ImageModuleBase;
	ulReloadZwSetSystemInformationBase = ulZwSetSystemInformationBase - SystemKernelModuleBase+ImageModuleBase;

	ulReloadSeSinglePrivilegeCheck = ulSeSinglePrivilegeCheck - SystemKernelModuleBase+ImageModuleBase;

	//hook reload SeSinglePrivilegeCheck

	bRet = HookFunctionByHeaderAddress(ulReloadSeSinglePrivilegeCheck,ulSeSinglePrivilegeCheck,SeSinglePrivilegeCheckHookZone,&SeSinglePrivilegeCheckPatchCodeLen,&SeSinglePrivilegeCheckRet);
	if(bRet)
	{
		bRet = FALSE;
		bRet = HookFunctionByHeaderAddress(
			NewSeSinglePrivilegeCheck,
			ulReloadSeSinglePrivilegeCheck,
			SeSinglePrivilegeCheckHookZone,
			&SeSinglePrivilegeCheckPatchCodeLen,
			&SeSinglePrivilegeCheckRet
			);
		if (bRet)
		{
			SeSinglePrivilegeCheckHooked = TRUE;
			//DbgPrint("hook SeSinglePrivilegeCheck success\n");
		}
	}
	return STATUS_SUCCESS;
}
//允许驱动加载
NTSTATUS EnableDriverLoading()
{
	if (SeSinglePrivilegeCheckHooked == TRUE)
	{
		SeSinglePrivilegeCheckHooked = FALSE;
		UnHookFunctionByHeaderAddress((DWORD)ulReloadSeSinglePrivilegeCheck,SeSinglePrivilegeCheckHookZone,SeSinglePrivilegeCheckPatchCodeLen);
		UnHookFunctionByHeaderAddress((DWORD)ulSeSinglePrivilegeCheck,SeSinglePrivilegeCheckHookZone,SeSinglePrivilegeCheckPatchCodeLen);
	}
}
//***************************************
//创建一个ImageNotify保存加载的驱动
//***************************************
VOID ImageNotify(
	PUNICODE_STRING  FullImageName,
	HANDLE  ProcessId,
	PIMAGE_INFO  ImageInfo
	)
{
	//排除非驱动模块的加载
	if (ProcessId != 0 || PsGetCurrentProcessId() != 4)
	{
		return;
	}
	//检查是否是加载驱动
	if (ImageInfo->ImageBase < MmUserProbeAddress)
	{
		return;
	}
	//检查UNICODE_STRING是否可以访问
	if (!ValidateUnicodeString(FullImageName))
	{
		return;
	}
	//KdPrint(("%d:%08x --> %ws\r\n",PsGetCurrentProcessId(),ImageInfo->ImageBase,FullImageName->Buffer));

	if (LogDefenseInfo->ulCount < 1000 &&
		ulLogCount < 1000)   //记录超过1000条，则不记录。
	{
		LogDefenseInfo->LogDefense[ulLogCount].ulPID = ImageInfo->ImageSize;
		LogDefenseInfo->LogDefense[ulLogCount].Type = 6;  //驱动加载
		LogDefenseInfo->LogDefense[ulLogCount].EProcess = ImageInfo->ImageBase;  //驱动基址

		memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess));
		memset(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents,0,sizeof(LogDefenseInfo->LogDefense[ulLogCount].lpwzMoreEvents));

		SafeCopyMemory(FullImageName->Buffer,LogDefenseInfo->LogDefense[ulLogCount].lpwzCreateProcess,FullImageName->Length);
		ulLogCount++;
	}
	return;
}