#include "ntos.h"

BOOL GetOriginalKiServiceTable(BYTE *NewImageBase,DWORD ExistImageBase,DWORD *NewKiServiceTable)
{
	PIMAGE_DOS_HEADER ImageDosHeader;
	PIMAGE_NT_HEADERS ImageNtHeaders;
	DWORD KeServiceDescriptorTableRva;
	PIMAGE_BASE_RELOCATION ImageBaseReloc=NULL;
	DWORD RelocSize;
	int ItemCount,Index;
	int Type;
	PDWORD RelocAddress;
	DWORD RvaData;
	DWORD count=0;
	WORD *TypeOffset;


	ImageDosHeader=(PIMAGE_DOS_HEADER)NewImageBase;
	if (ImageDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
	{
		return FALSE;
	}
	ImageNtHeaders=(PIMAGE_NT_HEADERS)(NewImageBase+ImageDosHeader->e_lfanew);
	if (ImageNtHeaders->Signature!=IMAGE_NT_SIGNATURE)
	{
		return FALSE;
	}
	KeServiceDescriptorTableRva=(DWORD)MiFindExportedRoutine(NewImageBase,TRUE,"KeServiceDescriptorTable",0);
	if (KeServiceDescriptorTableRva==0)
	{
		return FALSE;
	}

	KeServiceDescriptorTableRva=KeServiceDescriptorTableRva-(DWORD)NewImageBase;
	if (DebugOn)
		KdPrint(("KeServiceDescriptorTable:%X\n",KeServiceDescriptorTableRva));
	ImageBaseReloc=RtlImageDirectoryEntryToData(NewImageBase,TRUE,IMAGE_DIRECTORY_ENTRY_BASERELOC,&RelocSize);
	if (ImageBaseReloc==NULL)
	{
		return FALSE;
	}
	if (DebugOn)
		KdPrint(("get x IMAGE_DIRECTORY_ENTRY_BASERELOC ok\n"));
	while (ImageBaseReloc->SizeOfBlock)
	{  
		count++;
		ItemCount=(ImageBaseReloc->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION))/2;
		TypeOffset=(WORD*)((DWORD)ImageBaseReloc+sizeof(IMAGE_BASE_RELOCATION));
		for (Index=0;Index<ItemCount;Index++)
		{
			Type=TypeOffset[Index]>>12;
			if (Type==3)
			{
				RelocAddress=(PDWORD)((DWORD)(TypeOffset[Index]&0x0fff)+ImageBaseReloc->VirtualAddress+(DWORD)NewImageBase);
				RvaData=*RelocAddress-ExistImageBase;

				if (RvaData==KeServiceDescriptorTableRva)
				{
					if(*(USHORT*)((DWORD)RelocAddress-2)==0x05c7)
					{

						*NewKiServiceTable=*(DWORD*)((DWORD)RelocAddress+4)-ExistImageBase+(DWORD)NewImageBase;
						if (DebugOn)
							KdPrint(("Find OriginalImage ssdt :%X\n",*NewKiServiceTable));
						return TRUE;
					}
				}

			}

		}
		ImageBaseReloc=(PIMAGE_BASE_RELOCATION)((DWORD)ImageBaseReloc+ImageBaseReloc->SizeOfBlock);
	}
	if (DebugOn)
		KdPrint(("count:%d\n",count));

	return FALSE;
}
VOID FixOriginalKiServiceTable(PDWORD OriginalKiServiceTable,DWORD ModuleBase,DWORD ExistImageBase)
{
	DWORD FuctionCount;
	DWORD Index;
	FuctionCount=KeServiceDescriptorTable->TableSize;
	if (DebugOn)
		KdPrint(("ssdt funcion count:%X---KiServiceTable:%X\n",FuctionCount,KeServiceDescriptorTable->ServiceTable));	
	for (Index=0;Index<FuctionCount;Index++)
	{
		OriginalKiServiceTable[Index]=OriginalKiServiceTable[Index]-ExistImageBase+ModuleBase;
	}
}
BOOL InitSafeOperationModule(PDRIVER_OBJECT pDriverObject,WCHAR *SystemModulePath,ULONG KernelModuleBase)
{
	UNICODE_STRING FileName;
	HANDLE hSection;
	PDWORD FixdOriginalKiServiceTable;
	PDWORD CsRootkitOriginalKiServiceTable;
	int i=0;

	if (DebugOn)
		KdPrint(("Safe->Get System Kernel Module Info %ws:%08x\r\n",SystemModulePath,KernelModuleBase));

	if (DebugOn)
		KdPrint(("Safe->DriverObject:%08x\r\n",pDriverObject));

	//自己peload 一个ntos*，这样就解决了跟其他安全软件的冲突啦~

	if (!PeLoad(SystemModulePath,&ImageModuleBase,pDriverObject,KernelModuleBase))
	{
		if (DebugOn)
			KdPrint(("Safe->PeLoad failed\n"));
		return FALSE;
	}

	if (DebugOn)
		KdPrint(("Safe->ModuleBase:%08x\r\n",ImageModuleBase));

	OriginalKiServiceTable = ExAllocatePool(NonPagedPool,KeServiceDescriptorTable->TableSize*sizeof(DWORD));
	if (!OriginalKiServiceTable)
	{
		if (DebugOn)
			KdPrint(("OriginalKiServiceTable Failed\n"));
		return FALSE;
	}
	if(!GetOriginalKiServiceTable(ImageModuleBase,KernelModuleBase,&OriginalKiServiceTable))
	{
		if (DebugOn)
			KdPrint(("Safe->Get Original KiServiceTable Failed\n"));

		ExFreePool(OriginalKiServiceTable);

		return FALSE;
	}
	if (DebugOn)
		KdPrint(("Safe->OriginalKiServiceTable %X\n",OriginalKiServiceTable));

	//填充每一个ssdt对应函数地址~这里的地址是reload的
    FixOriginalKiServiceTable((PDWORD)OriginalKiServiceTable,(DWORD)ImageModuleBase,KernelModuleBase);

	OriginalServiceDescriptorTable=ExAllocatePool(NonPagedPool,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	if (OriginalServiceDescriptorTable == NULL)
	{
		ExFreePool(OriginalKiServiceTable);
		return FALSE;
	}
	RtlZeroMemory(OriginalServiceDescriptorTable,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	//这是一个干净的原始表，每个表里所对应的SSDT函数的地址都是有效的~
	OriginalServiceDescriptorTable->ServiceTable = (PDWORD)OriginalKiServiceTable;
	OriginalServiceDescriptorTable->CounterTable = KeServiceDescriptorTable->CounterTable;
	OriginalServiceDescriptorTable->TableSize    = KeServiceDescriptorTable->TableSize;
	OriginalServiceDescriptorTable->ArgumentTable = KeServiceDescriptorTable->ArgumentTable;

	CsRootkitOriginalKiServiceTable = ExAllocatePool(NonPagedPool,KeServiceDescriptorTable->TableSize*sizeof(DWORD));
	if (CsRootkitOriginalKiServiceTable==NULL)
	{
		ExFreePool(OriginalServiceDescriptorTable);
		ExFreePool(OriginalKiServiceTable);
		return FALSE;

	}
	RtlZeroMemory(CsRootkitOriginalKiServiceTable,KeServiceDescriptorTable->TableSize*sizeof(DWORD));

	Safe_ServiceDescriptorTable = ExAllocatePool(NonPagedPool,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	if (Safe_ServiceDescriptorTable == NULL)
	{
		ExFreePool(OriginalServiceDescriptorTable);
		ExFreePool(CsRootkitOriginalKiServiceTable);
		ExFreePool(OriginalKiServiceTable);
		return FALSE;
	}
	//这是一个干净的原始表，每个表里所对应的SSDT函数的地址都是原始函数
	RtlZeroMemory(Safe_ServiceDescriptorTable,sizeof(SERVICE_DESCRIPTOR_TABLE)*4);
	
	//填充原始函数地址
// 	for (i=0;i<KeServiceDescriptorTable->TableSize;i++)
// 	{
// 		CsRootkitOriginalKiServiceTable[i] = OriginalServiceDescriptorTable->ServiceTable[i];
// 	}
	Safe_ServiceDescriptorTable->ServiceTable = (PDWORD)CsRootkitOriginalKiServiceTable;
	Safe_ServiceDescriptorTable->CounterTable = KeServiceDescriptorTable->CounterTable;
	Safe_ServiceDescriptorTable->TableSize = KeServiceDescriptorTable->TableSize;
	Safe_ServiceDescriptorTable->ArgumentTable = KeServiceDescriptorTable->ArgumentTable;

	//释放就会bsod
	//ExFreePool(OriginalKiServiceTable);
	return TRUE;
}
///////////////////////////////////////////////////
__declspec(naked) VOID KiFastCallEntryHookZone()
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
		jmp [KiFastCallEntryRet];

	}
}
///////////////////////////////////////////////////
__declspec(naked) VOID KiFastCallEntryTempHookZone()
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
		jmp [KiFastCallEntryTempRet];

	}
}

PSERVICE_DESCRIPTOR_TABLE __stdcall FakeServiceDescriptorTable(PSERVICE_DESCRIPTOR_TABLE ServiceDescriptorTable,DWORD Index)
{
	ULONG ulEProcess;
	ULONG ulRetOK;
	int i=0;

	if (!RMmIsAddressValid ||
		!RPsGetCurrentProcess)
	{
		goto _FunRet;
	}
	//---------------------------------------------------
	//sreach hide process demo
	//---------------------------------------------------
	if (bIsInitSuccess)
	{
		ulEProcess = NULL;
		ulEProcess = RPsGetCurrentProcess();
		if (RMmIsAddressValid(ulEProcess))
		{
			ulRetOK = IsHideProcess(ulEProcess,HideProcessInfo);
			if (ulRetOK == 8)
			{
				//发现隐藏进程！！！
			}
		}
	}
	//--------------------------------------------------------
	//SSDT
	//-------------------------------------------------------
	if (ServiceDescriptorTable->ServiceTable == KeServiceDescriptorTable->ServiceTable)
	{
		if (Safe_ServiceDescriptorTable->TableSize > Index &&
			Safe_ServiceDescriptorTable->TableSize == ServiceDescriptorTable->TableSize)
		{
			if ((ULONG)Safe_ServiceDescriptorTable->ServiceTable[Index] >= ulMyDriverBase &&
				(ULONG)Safe_ServiceDescriptorTable->ServiceTable[Index] <= ulMyDriverBase + ulMyDriverSize)
			{
				//当前服务号的函数被hook，返回有过滤函数的表~
				return Safe_ServiceDescriptorTable;
			}
		}
		//如果该函数没有被hook，就返回干净的表，表里的地址是有效并且没有被其他软件hook过的哦，所以其他软件的hook就不存在鸟~体现为用任务管理器可以结束xuetr
		//这里也可以返回ServiceDescriptorTable，这样其他软件的hook就存在了
		//所以两种返回值，按照具体情况来吧 -_-!!
		//开启了内核安全模式，这样一来返回一个原始干净的SSDT表~~无视任何hook鸟

		//当A盾运行的时候，才考虑内核安全模式
		if (bIsInitSuccess &&
			bKernelSafeModule &&
			OriginalServiceDescriptorTable->TableSize == ServiceDescriptorTable->TableSize)
		{
			return OriginalServiceDescriptorTable;
		}

	}
	if (bInitWin32K &&
		RMmIsAddressValid(ShadowTable))
	{
		if (ServiceDescriptorTable->ServiceTable == ShadowTable)    //ShadowSSDT
		{
			//KdPrint(("ShadowSSDT:%08x",ServiceDescriptorTable->ServiceTable[Index]));
			if (Safe_ServiceDescriptorShadowSSDTTable->TableSize > Index &&
				Safe_ServiceDescriptorShadowSSDTTable->TableSize == ServiceDescriptorTable->TableSize)
			{
				if (Safe_ServiceDescriptorShadowSSDTTable->ServiceTable[Index] >= ulMyDriverBase &&
					Safe_ServiceDescriptorShadowSSDTTable->ServiceTable[Index] <= ulMyDriverBase + ulMyDriverSize)
				{
					//当前服务号的函数被hook，返回有过滤函数的表~
					return Safe_ServiceDescriptorShadowSSDTTable;
				}
			}
			if (bIsInitSuccess &&
				bKernelSafeModule &&
				OriginalShadowServiceDescriptorTable->TableSize == ServiceDescriptorTable->TableSize)
			{
				return OriginalShadowServiceDescriptorTable;
			}
		}
	}

_FunRet:
	//不是ssdt，返回原始~
	return ServiceDescriptorTable;
}
__declspec(naked) VOID KiFastCallEntryHookProc()
{
	_asm
	{
		push eax;
		push ecx;
		push edx;

		push eax;
		push edi;
		call FakeServiceDescriptorTable;
		mov edi,eax;

		pop edx;
		pop ecx;
		pop eax;
		jmp [KiFastCallEntryHookZone];
	}
}
/*
    win xp:
	8053e632 8bf2            mov     esi,edx
	8053e634 8b5f0c          mov     ebx,dword ptr [edi+0Ch]
	8053e637 33c9            xor     ecx,ecx
	8053e639 8a0c18          mov     cl,byte ptr [eax+ebx]
	8053e63c 8b3f            mov     edi,dword ptr [edi]
	8053e63e 8b1c87          mov     ebx,dword ptr [edi+eax*4]

	CodeInfo->LineCount=2;
	CodeInfo->CodeLine[0].CodeLength=2;
	CodeInfo->CodeLine[0].Code[0]=0x33;
	CodeInfo->CodeLine[0].Code[1]=0xC9;

	CodeInfo->CodeLine[1].CodeLength=3;
	CodeInfo->CodeLine[1].Code[0]=0x8A;
	CodeInfo->CodeLine[1].Code[1]=0x0C;
	CodeInfo->CodeLine[1].Code[2]=0x18;


	win 7:
	83c593ce 64ff05b0060000  inc     dword ptr fs:[6B0h]
	83c593d5 8bf2            mov     esi,edx
	83c593d7 33c9            xor     ecx,ecx
	83c593d9 8b570c          mov     edx,dword ptr [edi+0Ch]
	83c593dc 8b3f            mov     edi,dword ptr [edi]
	83c593de 8a0c10          mov     cl,byte ptr [eax+edx]
	83c593e1 8b1487          mov     edx,dword ptr [edi+eax*4]
	83c593e4 2be1            sub     esp,ecx
	83c593e6 c1e902          shr     ecx,2

	CodeInfo->LineCount=2;
	CodeInfo->CodeLine[0].CodeLength=2;
	CodeInfo->CodeLine[0].Code[0]=0x33;
	CodeInfo->CodeLine[0].Code[1]=0xC9;

	CodeInfo->CodeLine[1].CodeLength=3;
	CodeInfo->CodeLine[1].Code[0]=0x8B;
	CodeInfo->CodeLine[1].Code[1]=0x57;
	CodeInfo->CodeLine[1].Code[2]=0x0C;
*/
BOOL HookKiFastCallEntry()
{
	DWORD KiFastCallEntry;
	DWORD dwReloadKiFastCallEntry;
	int CodeInfoLength;
	PCODE_INFO CodeInfo;
	int PatchCodeLength;
	WIN_VER_DETAIL WinVer;
	BOOL bRetOK = FALSE;

	_asm
	{
		pushad;
		mov ecx, 0x176;
		rdmsr;
		mov KiFastCallEntry, eax;
		popad;
	}
	if (DebugOn)
		KdPrint(("Safe->KiFastCallEntry:0x%08X\n",KiFastCallEntry));

	CodeInfoLength=sizeof(CODE_INFO)+sizeof(CODE_LINE);
	CodeInfo=ExAllocatePool(NonPagedPool,CodeInfoLength);
	if (CodeInfo==NULL)
	{
		return bRetOK;
	}
	RtlZeroMemory(CodeInfo,CodeInfoLength);

	WinVer = GetWindowsVersion();
	switch(WinVer)
	{
	case WINDOWS_VERSION_XP:
	case WINDOWS_VERSION_2K3_SP1_SP2:
		CodeInfo->LineCount=1;
		CodeInfo->CodeLine[0].CodeLength=2;
		CodeInfo->CodeLine[0].Code[0]=0x33;
		CodeInfo->CodeLine[0].Code[1]=0xC9;

		CodeInfo->CodeLine[1].CodeLength=3;
		CodeInfo->CodeLine[1].Code[0]=0x8A;
		CodeInfo->CodeLine[1].Code[1]=0x0C;
		CodeInfo->CodeLine[1].Code[2]=0x18;
		break;
	case WINDOWS_VERSION_7_7000:
	case WINDOWS_VERSION_7_7600_UP:
		CodeInfo->LineCount=1;
		CodeInfo->CodeLine[0].CodeLength=2;
		CodeInfo->CodeLine[0].Code[0]=0x33;
		CodeInfo->CodeLine[0].Code[1]=0xC9;

		CodeInfo->CodeLine[1].CodeLength=3;
		CodeInfo->CodeLine[1].Code[0]=0x8B;
		CodeInfo->CodeLine[1].Code[1]=0x57;
		CodeInfo->CodeLine[1].Code[2]=0x0C;
		break;
	}
	//先hook KiFastCallEntry头部，跳转到 dwReloadKiFastCallEntry
	dwReloadKiFastCallEntry = KiFastCallEntry - SystemKernelModuleBase + ImageModuleBase;
	if (!MmIsAddressValidEx(dwReloadKiFastCallEntry))
	{
		return FALSE;
	}
	bRetOK = HookFunctionByHeaderAddress(
		dwReloadKiFastCallEntry,
		KiFastCallEntry,
		KiFastCallEntryTempHookZone,
		&KiFastCallEntryTempPatchCodeLength,
		&KiFastCallEntryTempRet
		);
	if (bRetOK)
	{
		bRetOK = FALSE;

		//再hook dwReloadKiFastCallEntry，免得被hook烂了

		if(HookFunctionMiddle((BYTE*)dwReloadKiFastCallEntry,4096,(DWORD)KiFastCallEntryHookProc,CodeInfo,KiFastCallEntryHookZone,&PatchCodeLength,&KiFastCallEntryRet))
		{
			memcpy(ByteKiFastCallEntryBak,(PVOID)KiFastCallEntry,5);  //备份跳转地址
			memcpy(ByteReloadKiFastCallEntryBak,(PVOID)dwReloadKiFastCallEntry,5);  //备份跳转地址
			bRetOK = TRUE;
		}
	}
/*
	if(HookFunctionMiddle((BYTE*)KiFastCallEntry,4096,(DWORD)KiFastCallEntryHookProc,CodeInfo,KiFastCallEntryHookZone,&PatchCodeLength,&KiFastCallEntryRet))
	{
		//保存hook的地址
		//ulKiFastCallEntryHookCheck = (ULONG)KiFastCallEntryRet-PatchCodeLength;
		//memcpy(ByteHookCheck,(PVOID)ulKiFastCallEntryHookCheck,5);  //备份跳转地址

		//KdPrint(("hook ok：%08x\n",ulKiFastCallEntryHookCheck));
		bRetOK = TRUE;
	}
*/
	ExFreePool(CodeInfo);
	return bRetOK;
}
ULONG ReLoadNtosCALL(ULONG *FuncSyntax,WCHAR *lpwzFuncTion,ULONG ulOldNtosBase,ULONG ulReloadNtosBase)
{
	int i=0;
	ULONG ulRet = FALSE;

	if (RMmIsAddressValid){
		if (RMmIsAddressValid(*FuncSyntax)){
			return *FuncSyntax;
		}
	}else{
		if (MmIsAddressValid(*FuncSyntax)){
			return *FuncSyntax;
		}
	}
	__try
	{
		for (i=0;i<NtosFuncAddressInfo->ulCount;i++)
		{
			if (_wcsnicmp(NtosFuncAddressInfo->NtosFuncInfo[i].FuncName,lpwzFuncTion,wcslen(lpwzFuncTion)) == 0)
			{
				*FuncSyntax = NtosFuncAddressInfo->NtosFuncInfo[i].ulReloadAddress;

				if (DebugOn)
					KdPrint(("[%ws]%08x -- %08x\n",lpwzFuncTion,*FuncSyntax,NtosFuncAddressInfo->NtosFuncInfo[i].ulReloadAddress));

				ulRet = TRUE;
				break;
			}
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return ulRet;
}
//从导出表里面去所有函数保存到结构里
ULONG GetKernelFunction(PNTOSFUNCINFO NtosFunc)
{
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;

	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,position;
	char* functionName;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	ULONG ulNtDllModuleBase;

	UNICODE_STRING UnicodeFunction;
	UNICODE_STRING UnicodeExportTableFunction;
	ANSI_STRING ExportTableFunction;


	memcpy(NtosFunc->ModulePath,SystemKernelFilePath,wcslen(SystemKernelFilePath)*2);

	__try
	{
		//从导出表里获取
		ulNtDllModuleBase = ImageModuleBase;
		pDosHeader = (PIMAGE_DOS_HEADER)ImageModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE){
			KdPrint(("failed to find NtHeader\r\n"));
			return NULL;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE){
			KdPrint(("failed to find NtHeader\r\n"));
			return NULL;
		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulNtDllModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulNtDllModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)((BYTE*)ulNtDllModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			functionName = (char*)( (BYTE*)ulNtDllModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
			functionAddress = (DWORD)((BYTE*)ulNtDllModuleBase + arrayOfFunctionAddresses[functionOrdinal]);

			if (strlen(functionName) < 2){
				continue;
			}
			RtlInitAnsiString(&ExportTableFunction,functionName);
			RtlAnsiStringToUnicodeString(&UnicodeExportTableFunction,&ExportTableFunction,TRUE);

			NtosFunc->ulCount = x;
			memcpy(NtosFunc->NtosFuncInfo[x].FuncName,UnicodeExportTableFunction.Buffer,UnicodeExportTableFunction.Length);
			NtosFunc->NtosFuncInfo[x].ulAddress = functionAddress - (ULONG)ImageModuleBase + SystemKernelModuleBase;
			NtosFunc->NtosFuncInfo[x].ulReloadAddress = functionAddress;
			NtosFunc->NtosFuncInfo[x].NumberOfFunctions = x;

			RtlFreeUnicodeString(&UnicodeExportTableFunction);
		}
	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return x;
}
NTSTATUS ReLoadNtos(PDRIVER_OBJECT   DriverObject,DWORD RetAddress)
{
	NTSTATUS status = STATUS_UNSUCCESSFUL;
	ULONG ulKeAddSystemServiceTable;
	int p;
	HANDLE ThreadHandle;
	PVOID ThreadObject;

	if (DebugOn)
		KdPrint(("ret address:%X\n",RetAddress));

	//为记录隐藏进程建立表
	HideProcessInfo = (PPROCESSINFO)ExAllocatePool(NonPagedPool,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*120);
	if (!HideProcessInfo)
	{
		KdPrint(("Init ProcessInfo failed"));
		return status;
	}
	memset(HideProcessInfo,0,(sizeof(PROCESSINFO)+sizeof(SAFESYSTEM_PROCESS_INFORMATION))*120);

	if (!GetSystemKernelModuleInfo(
		&SystemKernelFilePath,
		&SystemKernelModuleBase,
		&SystemKernelModuleSize
		))
	{
		KdPrint(("Get System Kernel Module failed"));
		ExFreePool(HideProcessInfo);
		return status;
	}
	if (DebugOn)
		KdPrint(("%S,%X\n",SystemKernelFilePath,SystemKernelModuleBase));

	//为记录日志建立一个表
	LogDefenseInfo = (PLOGDEFENSE)ExAllocatePool(NonPagedPool,sizeof(LOGDEFENSE)*1024);
	if (!LogDefenseInfo)
	{
		KdPrint(("Init Log Defense Info failed\n"));
		ExFreePool(HideProcessInfo);
		return status;
	}
	memset(LogDefenseInfo,0,sizeof(LOGDEFENSE)*1024);

	if (InitSafeOperationModule(
		DriverObject,
		SystemKernelFilePath,
		SystemKernelModuleBase
		))
	{
		KdPrint(("Init Ntos Module Success\r\n"));

		//申请一块内存来保存函数的一些信息
		NtosFuncAddressInfo = (PNTOSFUNCINFO)ExAllocatePool(NonPagedPool,(SystemKernelModuleSize+1024));
		if (!NtosFuncAddressInfo)
		{
			KdPrint(("Init Kernel Function Pool failed\n"));
			ExFreePool(HideProcessInfo);
			ExFreePool(LogDefenseInfo);
			return status;
		}
		memset(NtosFuncAddressInfo,0,(SystemKernelModuleSize+1024));
		if (!GetKernelFunction(NtosFuncAddressInfo)){
			KdPrint(("Init Kernel Function Info failed\n"));
			ExFreePool(HideProcessInfo);
			ExFreePool(LogDefenseInfo);
			ExFreePool(NtosFuncAddressInfo);
			return status;
		}
		if (DebugOn){
			for (p=0;p<NtosFuncAddressInfo->ulCount;p++)
			{
				KdPrint(("add:%08x\r\nreload:%80x\r\n%ws\r\n\r\n",
					NtosFuncAddressInfo->NtosFuncInfo[p].ulAddress,
					NtosFuncAddressInfo->NtosFuncInfo[p].ulReloadAddress,
					NtosFuncAddressInfo->NtosFuncInfo[p].FuncName));
			}
		}

		//初始化下这两个api
		ReLoadNtosCALL(&RPsGetCurrentProcess,L"PsGetCurrentProcess",SystemKernelModuleBase,ImageModuleBase);
		ReLoadNtosCALL(&RMmIsAddressValid,L"MmIsAddressValid",SystemKernelModuleBase,ImageModuleBase);
		if (!RMmIsAddressValid ||
			!RPsGetCurrentProcess)
		{
			KdPrint(("Init NtosCALL failed\n"));
			ExFreePool(HideProcessInfo);
			ExFreePool(LogDefenseInfo);
			ExFreePool(NtosFuncAddressInfo);
			return status;
		}
		KdPrint(("Init Kernel Function Info Success\n"));
		
		if (HookKiFastCallEntry()){

			//通信控制
			InitControl();
			status = STATUS_SUCCESS;

			KdPrint(("Init A-Protect Kernel Module Success\r\n"));
		}else
			KdPrint(("Init A-Protect Kernel Module Failed"));
	}
	return status;
}

