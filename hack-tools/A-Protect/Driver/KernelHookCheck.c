#include "KernelHookCheck.h"

unsigned long __fastcall GetFunctionCodeSize(void *Proc)
{
	ULONG  Length;
	PUCHAR pOpcode;
	ULONG  Result = 0;
	ULONG CCINT3Count=0;
	ULONG ulProcCode = NULL;

	ulProcCode = Proc;
	do
	{
		if (MmIsAddressValidEx(ulProcCode) &&
			MmIsAddressValidEx((ulProcCode+1)) &&
			MmIsAddressValidEx((ulProcCode+2)) &&
			MmIsAddressValidEx((ulProcCode+3)))
		{
			Length = SizeOfCode(ulProcCode, &pOpcode);
			Result += Length;
			if ((Length == 1) && (*pOpcode == 0xCC||*pOpcode==0x90)) CCINT3Count++;
			if (CCINT3Count>1 ||
				*pOpcode == 0x00)
			{
				break;
			}
			ulProcCode = (PVOID)((ULONG)ulProcCode + Length);

		}else{
			break;
		}

	} while (Length);

	return Result;
}
//判断函数是否可以访问
BOOL IsFuncInInitSection(ULONG ulFuncBase,ULONG ulSize)
{
	BOOL bRetOK = FALSE;
	ULONG x=0;

	for (x=ulFuncBase;x<ulFuncBase+ulSize;x++)
	{
		if (!MmIsAddressValidEx(x))
		{
			bRetOK = TRUE;
			break;
		}
	}
	return bRetOK;
}
BOOL ReSetEatHook(int x,ULONG ulKernelModuleBase,ULONG ulRealAddress)
{
	ULONG ulModuleBase;
	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;
	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base,functionAddress;
	IMAGE_EXPORT_DIRECTORY *pExportTable;

	//开始恢复
	ulModuleBase = ulKernelModuleBase;

	pDosHeader = (PIMAGE_DOS_HEADER)ulModuleBase;
	if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
	{
		KdPrint(("failed to find NtHeader\r\n"));
		return 0;
	}
	NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
	if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
	{
		KdPrint(("failed to find NtHeader\r\n"));
		return 0;
	}
	opthdr = NtDllHeader->OptionalHeader;
	pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
	arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
	arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
	arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

	Base = pExportTable->Base;

	_asm
	{
		CLI					
			MOV	EAX, CR0		
			AND EAX, NOT 10000H 
			MOV	CR0, EAX		
	}	
	arrayOfFunctionAddresses[arrayOfFunctionOrdinals[x] + Base - 1] = ulRealAddress - ulModuleBase;
	_asm 
	{
		MOV	EAX, CR0		
			OR	EAX, 10000H			
			MOV	CR0, EAX			
			STI					
	}

}
/*
BOOL IatHookCheck(ULONG *ulBase)
{
	PIMAGE_IMPORT_DESCRIPTOR ImageImportDescriptor=NULL;
	PIMAGE_THUNK_DATA ImageThunkData,FirstThunk;
	PIMAGE_IMPORT_BY_NAME ImortByName;
	DWORD ImportSize;
	char ModuleName[260];
	PVOID ModuleBase;
	DWORD FunctionAddress;
	ULONG ImageBase;
	int i=0;

	ImageBase = *ulBase;

	ImportSize = 0;
	ImageImportDescriptor=(PIMAGE_IMPORT_DESCRIPTOR)RtlImageDirectoryEntryToData(ImageBase,TRUE,IMAGE_DIRECTORY_ENTRY_IMPORT,&ImportSize);
	if (!ImageImportDescriptor || 
		!MmIsAddressValidEx(ImageImportDescriptor))
	{
		KdPrint(("ImageImport:%08x ImageBase:%08x\n",ImageImportDescriptor,ImageBase));
		return FALSE;
	}
	while (ImageImportDescriptor->OriginalFirstThunk&&ImageImportDescriptor->Name)
	{
		strcpy(ModuleName,(char*)(ImageBase+ImageImportDescriptor->Name));

		//ntoskrnl.exe(NTKRNLPA.exe、ntkrnlmp.exe、ntkrpamp.exe)：
		if (_stricmp(ModuleName,"ntkrnlpa.exe")==0||
			_stricmp(ModuleName,"ntoskrnl.exe")==0||
			_stricmp(ModuleName,"ntkrnlmp.exe")==0||
			_stricmp(ModuleName,"ntkrpamp.exe")==0)
		{
			ModuleBase=GetKernelModuleBase(PDriverObject,"ntkrnlpa.exe");
			if (ModuleBase==NULL)
			{
				ModuleBase=GetKernelModuleBase(PDriverObject,"ntoskrnl.exe");
				if (ModuleBase==NULL)
				{
					ModuleBase=GetKernelModuleBase(PDriverObject,"ntkrnlmp.exe");
					if (ModuleBase==NULL)
					{
						ModuleBase=GetKernelModuleBase(PDriverObject,"ntkrpamp.exe");

					}

				}
			}

		}
		else
		{
			ModuleBase=GetKernelModuleBase(PDriverObject,ModuleName);

		}
		if (ModuleBase==NULL)
		{
			KdPrint(("can't find module:%s\n",ModuleName));

			ImageImportDescriptor++;
			continue;
		}
		KdPrint(("Module:%s\n",ModuleName));

		ImageThunkData=(PIMAGE_THUNK_DATA)(ImageBase+ImageImportDescriptor->OriginalFirstThunk);
		FirstThunk=(PIMAGE_THUNK_DATA)(ImageBase+ImageImportDescriptor->FirstThunk);

		while(ImageThunkData->u1.Ordinal)
		{
			//序号导入
			if(IMAGE_SNAP_BY_ORDINAL32(ImageThunkData->u1.Ordinal))
			{
				FunctionAddress = FirstThunk->u1.AddressOfData;

				KdPrint(("funcion %08x Index %d \n",FunctionAddress,ImageThunkData->u1.Ordinal & ~IMAGE_ORDINAL_FLAG32));

			}
			//函数名导入
			else
			{
				ImortByName=(PIMAGE_IMPORT_BY_NAME)(ImageBase+ImageThunkData->u1.AddressOfData);
				FunctionAddress = FirstThunk->u1.AddressOfData; // 等同于 *(ULONG *)FirstThunk

				//if (i<=15)
				//{
					KdPrint(("Funcion %08x IMAGE_IMPORT_BY_NAME %s\n",FunctionAddress,ImortByName->Name));
				//}
				//KdPrint(("Funcion %08x i %d\n",FunctionAddress,i));
			}
			i++;
			FirstThunk++;
			ImageThunkData++;
		}
		i=0;
		ImageImportDescriptor++;
	}
	return TRUE;
}
*/
BOOL EatHookCheck(ULONG ulModuleBase,PINLINEHOOKINFO InlineHookInfo,PNTOSFUNCINFO FuncAddressInfo)
{

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;
	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,ulReloadAddress;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	char *functionName;
	char lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	ULONG ulFuncAddr;
	int i=0;
	UNICODE_STRING UnicodeCompareFuncName;
	UNICODE_STRING UnicodeFuncName;
	ANSI_STRING AnsiFuncName;

	if (!MmIsAddressValidEx((PIMAGE_DOS_HEADER)ulModuleBase))
	{
		return FALSE;
	}
	__try
	{
		pDosHeader=(PIMAGE_DOS_HEADER)ulModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return FALSE;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return FALSE;
		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			functionName = (char*)((BYTE*)ulModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
			functionAddress = (DWORD)((BYTE*)ulModuleBase + arrayOfFunctionAddresses[functionOrdinal]);
			if (functionAddress > ulModuleBase && functionAddress < ulModuleBase + NtDllHeader->OptionalHeader.SizeOfImage)
			{
				continue;
			}
			//KdPrint(("Find EAT:%s:0x%08x\n",functionName,functionAddress));
			//填充结构
			memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
			if (!IsAddressInSystem(
				functionAddress,
				&ulHookModuleBase,
				&ulHookModuleSize,
				lpszHookModuleImage))
			{
				memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
				strcat(lpszHookModuleImage,"Unknown");
				ulHookModuleBase = 0;
				ulHookModuleSize = 0;
			}
			//KdPrint(("Module:%s\n",lpszHookModuleImage));
			InlineHookInfo->InlineHook[IntHookCount].ulMemoryHookBase = functionAddress;

			RtlInitAnsiString(&AnsiFuncName,functionName);
			RtlAnsiStringToUnicodeString(&UnicodeCompareFuncName,&AnsiFuncName,TRUE);

			for (i=0;i<FuncAddressInfo->ulCount;i++)
			{
				//通过函数名来判断吧。这里不能通过导出的位置x，因为这里的x有可能是FuncAddressInfo的未导出函数
				RtlInitUnicodeString(&UnicodeFuncName,FuncAddressInfo->NtosFuncInfo[i].FuncName);

				if (RtlCompareUnicodeString(&UnicodeFuncName,&UnicodeCompareFuncName,TRUE) == 0)  //不区分大小写
				{
					InlineHookInfo->InlineHook[IntHookCount].ulRealFunctionBase = FuncAddressInfo->NtosFuncInfo[i].ulAddress;
					break;
				}
			}
			RtlFreeUnicodeString(&UnicodeCompareFuncName);

			memset(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszFunction));
			memset(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage));

			memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,functionName,strlen(functionName));
			memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));

			memcpy(InlineHookInfo->InlineHook[IntHookCount].lpwzRealModuleImage,FuncAddressInfo->ModulePath,wcslen(FuncAddressInfo->ModulePath)*2);
			InlineHookInfo->InlineHook[IntHookCount].ulRealModuleBase = ulModuleBase;

			InlineHookInfo->InlineHook[IntHookCount].ulMemoryFunctionBase = x;
			InlineHookInfo->InlineHook[IntHookCount].ulHookModuleBase = ulHookModuleBase;
			InlineHookInfo->InlineHook[IntHookCount].ulHookModuleSize = ulHookModuleSize;
			InlineHookInfo->InlineHook[IntHookCount].ulHookType = 1;  //eat hook
			IntHookCount++;
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return FALSE;
}
BOOL IsFunctionInExportTable(ULONG ulModuleBase,ULONG ulFunctionAddress)
{

	PIMAGE_DOS_HEADER pDosHeader;
	PIMAGE_NT_HEADERS NtDllHeader;
	IMAGE_OPTIONAL_HEADER opthdr;
	DWORD* arrayOfFunctionAddresses;
	DWORD* arrayOfFunctionNames;
	WORD* arrayOfFunctionOrdinals;
	DWORD functionOrdinal;
	DWORD Base, x, functionAddress,ulOldAddress;
	IMAGE_EXPORT_DIRECTORY *pExportTable;
	char *functionName;


	__try
	{
		pDosHeader=(PIMAGE_DOS_HEADER)ulModuleBase;
		if (pDosHeader->e_magic!=IMAGE_DOS_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return FALSE;
		}
		NtDllHeader=(PIMAGE_NT_HEADERS)(ULONG)((ULONG)pDosHeader+pDosHeader->e_lfanew);
		if (NtDllHeader->Signature!=IMAGE_NT_SIGNATURE)
		{
			KdPrint(("failed to find NtHeader\r\n"));
			return FALSE;
		}
		opthdr = NtDllHeader->OptionalHeader;
		pExportTable =(IMAGE_EXPORT_DIRECTORY*)((BYTE*)ulModuleBase + opthdr.DataDirectory[ IMAGE_DIRECTORY_ENTRY_EXPORT]. VirtualAddress); //得到导出表
		arrayOfFunctionAddresses = (DWORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfFunctions);  //地址表
		arrayOfFunctionNames = (DWORD*)((BYTE*)ulModuleBase + pExportTable->AddressOfNames);         //函数名表
		arrayOfFunctionOrdinals = (WORD*)( (BYTE*)ulModuleBase + pExportTable->AddressOfNameOrdinals);

		Base = pExportTable->Base;

		for(x = 0; x < pExportTable->NumberOfFunctions; x++) //在整个导出表里扫描
		{
			//functionName = (char*)((BYTE*)ulModuleBase + arrayOfFunctionNames[x]);
			functionOrdinal = arrayOfFunctionOrdinals[x] + Base - 1; 
			functionAddress = (DWORD)((BYTE*)ulModuleBase + arrayOfFunctionAddresses[functionOrdinal]);
			//KdPrint(("%08x:%s\r\n",functionAddress,functionName));
			//ulOldAddress = GetSystemRoutineAddress(0,functionName);
			ulOldAddress = functionAddress - ulModuleBase + SystemKernelModuleBase;
			if (ulFunctionAddress == ulOldAddress)
			{
				//是导出函数，退出
				return TRUE;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
	return FALSE;
}
//获取导出函数下一级0xe8 call函数的inlinehookcheck
ULONG GetNextFunctionAddress(ULONG ulNtDllModuleBase,ULONG ulOldAddress,char *functionName,PINLINEHOOKINFO InlineHookInfo)
{
	ULONG ulCodeSize;

	ULONG ulNextFunCodeSize;
	ULONG ulNextFunReloadCodeSize;
	PUCHAR i;

	ULONG ulNextFunctionAddress=0;
	ULONG ulReloadNextFunctionAddress=0;
	BOOL bRetOK = FALSE;
	PUCHAR ulTemp,ulReloadTemp;
	ULONG ulSize;
	ULONG ulHookFunctionAddress;
	PUCHAR p;

	INSTRUCTION	Inst;
	INSTRUCTION	Instb;

	char lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	int Flagss;
	int JmpCount = 0;

	if (!MmIsAddressValidEx(ulOldAddress))
	{
		return bRetOK;
	}
	__try
	{
		ulCodeSize = GetFunctionCodeSize(ulOldAddress);
		for (i=(ULONG)ulOldAddress;i < i+ulCodeSize;i++)
		{
			if (!MmIsAddressValidEx(i)){
				break;
			}
			if (*i == 0xe8)
			{
				ulNextFunctionAddress = *(PULONG)(i+1)+(ULONG)(i+5);
				if (MmIsAddressValidEx(ulNextFunctionAddress))
				{
					//判断一下是否是导出函数
					if (IsFunctionInExportTable(ulNtDllModuleBase,ulNextFunctionAddress))
					{
						return;
					}
					//做hook 扫描
					ulReloadNextFunctionAddress = ulNextFunctionAddress - SystemKernelModuleBase + ImageModuleBase;
					if (MmIsAddressValidEx(ulReloadNextFunctionAddress) &&
						MmIsAddressValidEx(ulNextFunctionAddress))
					{
						ulNextFunCodeSize = GetFunctionCodeSize(ulNextFunctionAddress);
						ulNextFunReloadCodeSize = GetFunctionCodeSize(ulReloadNextFunctionAddress);

						if (ulNextFunCodeSize == ulNextFunReloadCodeSize &&
							memcmp(ulReloadNextFunctionAddress,ulNextFunctionAddress,ulNextFunCodeSize) != NULL)
						{
							//被hook了
							//KdPrint(("found hook---->%s",functionName));

							for (p=ulNextFunctionAddress ;p< ulNextFunctionAddress+ulNextFunCodeSize; p++)
							{
								//折半扫描，如果前面一半一样，则开始扫描下一半
								if (memcmp(ulReloadNextFunctionAddress,ulNextFunctionAddress,ulNextFunCodeSize/2) == NULL)
								{
									ulNextFunCodeSize = ulNextFunCodeSize + ulNextFunCodeSize/2;
									continue;
								}
								//是否结束？
								if (*p == 0xcc ||
									*p == 0xc2)
								{
									break;
								}
								ulHookFunctionAddress = (*(PULONG)(p + 1) + (ULONG)p + 5);  //得到地址
								if (!MmIsAddressValidEx(ulHookFunctionAddress))
								{
									continue;
								}
								ulTemp = NULL;
								get_instruction(&Inst,p,MODE_32);
								switch (Inst.type)
								{
								case INSTRUCTION_TYPE_JMP:
									if(Inst.opcode==0xFF&&Inst.modrm==0x25)
									{
										//DIRECT_JMP
										ulTemp = Inst.op1.displacement;
									}
									else if (Inst.opcode==0xEB)
									{
										ulTemp = (ULONG)(p+Inst.op1.immediate);
									}
									else if(Inst.opcode==0xE9)
									{
										//RELATIVE_JMP;
										ulTemp = (ULONG)(p+Inst.op1.immediate);
									}
									break;
								case INSTRUCTION_TYPE_CALL:
									if(Inst.opcode==0xFF&&Inst.modrm==0x15)
									{
										//DIRECT_CALL
										ulTemp = Inst.op1.displacement;
									}
									else if (Inst.opcode==0x9A)
									{
										ulTemp = (ULONG)(p+Inst.op1.immediate);
									}
									else if(Inst.opcode==0xE8)
									{
										//RELATIVE_CALL;
										ulTemp = (ULONG)(p+Inst.op1.immediate);
									}
									break;
								case INSTRUCTION_TYPE_PUSH:
									if(!MmIsAddressValidEx((PVOID)(p)))
									{
										break;
									}
									get_instruction(&Instb,(BYTE*)(p),MODE_32);
									if(Instb.type == INSTRUCTION_TYPE_RET)
									{
										//StartAddress+len-inst.length-instb.length;
										ulTemp = Instb.op1.displacement;
									}
									break;
								}
								if (ulTemp &&
									MmIsAddressValidEx(ulTemp) &&
									MmIsAddressValidEx(p))   //hook的地址也要有效才可以哦
								{
									//得到长度
									ulSize = p - ulNextFunctionAddress;
									ulReloadTemp = ulReloadNextFunctionAddress + ulSize;
									if (MmIsAddressValidEx(ulReloadTemp))
									{
										if (memcmp(ulReloadTemp,p,0x5) == 0){
											continue;
										}
									}
									for (JmpCount=0;JmpCount<10;JmpCount++)
									{
										if (MmIsAddressValidEx(ulTemp))
										{
											ulTemp = ulTemp+0x5;

											if (*ulTemp == 0xe9 ||
												*ulTemp == 0xe8)
											{
												if (DebugOn)
													KdPrint(("ulTemp == 0xe9"));

												ulTemp = *(PULONG)(ulTemp+1)+(ULONG)(ulTemp+5);

											}else
											{
												break;
											}
										}
									}
									memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
									if (!IsAddressInSystem(
										ulTemp,
										&ulHookModuleBase,
										&ulHookModuleSize,
										lpszHookModuleImage))
									{
										memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
										strcat(lpszHookModuleImage,"Unknown");
										ulHookModuleBase = 0;
										ulHookModuleSize = 0;
									}
									if (!MmIsAddressValidEx(&InlineHookInfo->InlineHook[IntHookCount]))
									{
										return;
									}
									if (DebugOn)
										KdPrint(("found hook---->%s:%08x 0x%x",functionName,*(ULONG *)ulTemp,GetFunctionCodeSize(ulNextFunctionAddress)));

									InlineHookInfo->InlineHook[IntHookCount].ulMemoryHookBase = ulTemp+0x5;
									memset(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszFunction));
									memset(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,0,sizeof(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage));

									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction,functionName,strlen(functionName));
									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszFunction+strlen(functionName),"/NextCallHook",strlen("/NextCallHook"));
									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));
									memcpy(InlineHookInfo->InlineHook[IntHookCount].lpwzRealModuleImage,SystemKernelFilePath,wcslen(SystemKernelFilePath)*2);
									InlineHookInfo->InlineHook[IntHookCount].ulRealModuleBase = SystemKernelModuleBase;
									InlineHookInfo->InlineHook[IntHookCount].ulMemoryFunctionBase = (ULONG)p;
									InlineHookInfo->InlineHook[IntHookCount].ulRealFunctionBase = ulNextFunctionAddress;
									InlineHookInfo->InlineHook[IntHookCount].ulHookModuleBase = ulHookModuleBase;
									InlineHookInfo->InlineHook[IntHookCount].ulHookModuleSize = ulHookModuleSize;
									IntHookCount++;
Next:
									_asm{nop}
								}
							}
						}
					}
				}
			}
			//结束鸟
			if (*i == 0xcc ||
				*i == 0xc2)
			{
				return;
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){

	}
}
//扫描所选模块的inline hook
BOOL KernelHookCheck(PINLINEHOOKINFO SelectModuleInlineHookInfo,int HookType)
{
	PUCHAR p;
	INSTRUCTION	Inst;
	INSTRUCTION	Instb;
	ULONG ulFuncAddr;
	ULONG ulReloadFuncAddr;
	ULONG ulCodeSize,ulReloadCodeSize;
	int i=0;
	PUCHAR x=0;
	char lpszHookModuleImage[256];
	ULONG ulHookModuleBase;
	ULONG ulHookModuleSize;
	ULONG ulIsRealFunction;
	PUCHAR ulHookCallFunction;
	ULONG ulReloadRealFunction;
	ULONG ulSize;
	char FuncName[260] = {0};
	BOOL bInit = FALSE;
	BOOL bIsRealHook = FALSE;
	BOOL bIsSSDTFunc = FALSE;
	int JmpCount = 0;

	if (DebugOn)
		KdPrint(("Module Hook Scan\r\n"));

	switch (HookType)
	{
	case NtosModule:
		//因为pdb下载可能读取网络，所以如果初始化pdb成功了，就用PDB
		if (bKrnlPDBSuccess){
			KernelFuncInfo = PDBNtosFuncAddressInfo;
		}else{
			KernelFuncInfo = NtosFuncAddressInfo;
		}
		KernelFuncInfo->ulModuleBase = SystemKernelModuleBase;
		break;
	case SelectModule:
		KernelFuncInfo = SelectModuleFuncInfo;
		break;
	default:
		return;
	}
	//效验ModuleBase
	if (!MmIsAddressValidEx(KernelFuncInfo->ulModuleBase)){
		return;
	}
	//初始化为0
	IntHookCount = 0;

	//如果是win32K，挂载
	if (KernelFuncInfo->ulModuleBase == ulWin32kBase){

		ReLoadNtosCALL(&RKeAttachProcess,L"KeAttachProcess",SystemKernelModuleBase,ImageModuleBase);
		ReLoadNtosCALL(&RKeDetachProcess,L"KeDetachProcess",SystemKernelModuleBase,ImageModuleBase);
		if (RKeAttachProcess &&
			RKeDetachProcess)
		{
			bInit = TRUE;
		}
		if (!bInit)
			return STATUS_UNSUCCESSFUL;

		RKeAttachProcess(AttachGuiEProcess);
	}
	__try{
		EatHookCheck(KernelFuncInfo->ulModuleBase,SelectModuleInlineHookInfo,KernelFuncInfo);
		//*********************************************************************
		//inline hook 检查
		//*********************************************************************
		for (i=0;i<KernelFuncInfo->ulCount;i++)
		{
			ulFuncAddr = KernelFuncInfo->NtosFuncInfo[i].ulAddress;
			ulReloadFuncAddr = KernelFuncInfo->NtosFuncInfo[i].ulReloadAddress;

			//两地址只要有一个出现问题，就下一位
			if (!MmIsAddressValidEx(ulFuncAddr) ||
				!MmIsAddressValidEx(ulReloadFuncAddr)){
					continue;
			}
			if (*(PULONG)ulFuncAddr == 0 ||
				*(PULONG)ulReloadFuncAddr == 0){
					continue;
			}
			memset(FuncName,0,sizeof(FuncName));
			WcharToChar(KernelFuncInfo->NtosFuncInfo[i].FuncName,FuncName);

			//如果
			if (!bKrnlPDBSuccess){
				if (*KernelFuncInfo->NtosFuncInfo[i].FuncName == 'Z' &&
					*(KernelFuncInfo->NtosFuncInfo[i].FuncName+1) == 'w')
				{
					bIsSSDTFunc = TRUE;
				}
				//下一层函数只扫描非Zw开头的，并且只扫描未导出函数
				if (!bIsSSDTFunc)
				{
					//PDB没有初始化成功，才扫描下级函数
					if (!bKrnlPDBSuccess)
						GetNextFunctionAddress(ImageModuleBase,ulFuncAddr,FuncName,InlineHookInfo);
				}
				bIsSSDTFunc = FALSE;
			}

			ulReloadCodeSize = GetFunctionCodeSize(ulReloadFuncAddr);

			//判断下函数是否在INIT节，在INIT节的函数调用完就去掉了，不蓝屏会BSOD
			if (IsFuncInInitSection(ulFuncAddr,ulReloadCodeSize) == TRUE){
				continue;
			}
			//可以安全读取原始函数大小了
			//大小不一样，下一位
			ulCodeSize = GetFunctionCodeSize(ulFuncAddr);
			if (ulCodeSize != ulReloadCodeSize){
				//排除特殊情况，比如绕过hook，会产生两个nop，这样GetFunctionCodeSize就会认为该函数已经结束，其实没有,比如下面：
				/*
				lkd> u NtCreateSection
				nt!NtCreateSection:
				805a176c e9fbdf507f      jmp     ffaaf76c      <----A盾的绕过功能
				805a1771 90              nop
				805a1772 90              nop                   <---反汇编引擎会认为函数到这里就结束了，刚好7个字节
				805a1773 e8289b4c78      call    f8a6b2a0      <---------------被hook了
				805a1778 8b551c          mov     edx,dword ptr [ebp+1Ch]
				805a177b f7c2ffff9fe2    test    edx,0E29FFFFFh
				805a1781 7528            jne     nt!NtCreateSection+0x3f (805a17ab)
				805a1783 f7c20000000d    test    edx,0D000000h
				*/
				if (ulCodeSize == 0x7 && ulReloadCodeSize > ulCodeSize){
						goto Check;
				}
				if (DebugOn)
					KdPrint(("size---->%ws:%08x %08x %x %x\r\n",KernelFuncInfo->NtosFuncInfo[i].FuncName,ulFuncAddr,ulReloadFuncAddr,ulCodeSize,ulReloadCodeSize));
				continue;
			}
Check:

			//开始扫描hook
			if (memcmp(ulReloadFuncAddr,ulFuncAddr,ulReloadCodeSize) != NULL)
			{
				if (DebugOn)
					KdPrint(("%ws:%08x---->%08x %x\r\n",KernelFuncInfo->NtosFuncInfo[i].FuncName,ulFuncAddr,ulReloadFuncAddr,ulCodeSize));

				for (p=ulFuncAddr ;p< ulFuncAddr+ulCodeSize; p++)
				{
					//折半扫描，如果前面一半一样，则开始扫描下一半
					if (memcmp(ulReloadFuncAddr,ulFuncAddr,ulCodeSize/2) == NULL)
					{
						ulCodeSize = ulCodeSize + ulCodeSize/2;
						continue;
					}
					if (*p == 0xcc ||
						*p == 0xc2)
					{
						break;
					}
					ulHookCallFunction = NULL;
					get_instruction(&Inst,p,MODE_32);
					switch (Inst.type)
					{
					case INSTRUCTION_TYPE_JMP:
						if(Inst.opcode==0xFF&&Inst.modrm==0x25)
						{
							//DIRECT_JMP
							ulHookCallFunction = Inst.op1.displacement;
						}
						else if (Inst.opcode==0xEB)
						{
							ulHookCallFunction = (ULONG)(p+Inst.op1.immediate);
						}
						else if(Inst.opcode==0xE9)
						{
							//RELATIVE_JMP;
							ulHookCallFunction = (ULONG)(p+Inst.op1.immediate);
						}
						break;
					case INSTRUCTION_TYPE_CALL:
						if(Inst.opcode==0xFF&&Inst.modrm==0x15)
						{
							//DIRECT_CALL
							ulHookCallFunction = Inst.op1.displacement;
						}
						else if (Inst.opcode==0x9A)
						{
							ulHookCallFunction = (ULONG)(p+Inst.op1.immediate);
						}
						else if(Inst.opcode==0xE8)
						{
							//RELATIVE_CALL;
							ulHookCallFunction = (ULONG)(p+Inst.op1.immediate);
						}
						break;
					case INSTRUCTION_TYPE_PUSH:
						if(!MmIsAddressValidEx((PVOID)(p)))
						{
							break;
						}
						get_instruction(&Instb,(BYTE*)(p),MODE_32);
						if(Instb.type == INSTRUCTION_TYPE_RET)
						{
							//StartAddress+len-inst.length-instb.length;
							ulHookCallFunction = Instb.op1.displacement;
						}
						break;
					}

					if (MmIsAddressValidEx(ulHookCallFunction) &&
						MmIsAddressValidEx(p))   //hook的地址也要有效才可以哦
					{
						//得到长度
						ulSize = p - ulFuncAddr;
						ulReloadRealFunction = ulReloadFuncAddr + ulSize;
						//Hook检查原理：
						//
						//real  ：0xFFFFFFFF call xxxxxx
						//reload: 0xFFFFFFFF call yyyyyy
						//对比0xFFFFFFFF处的指令是否相等，不等说明hook该函数被hook鸟~~！
						if (MmIsAddressValidEx(ulReloadRealFunction))
						{
							if (*(ULONG *)p == *(ULONG *)ulReloadRealFunction ||
								*(ULONG *)ulReloadRealFunction == 0x23b9 ||  //过滤那些中断函数
								*(PUCHAR)ulReloadRealFunction == 0 ||
								*(ULONG *)ulFuncAddr == 0x4f780a2c ||     //过滤RtlpRandomConstantVector
								*(PUCHAR)ulFuncAddr == 0)       // *(PUCHAR)-> 0x00  *(ULONG *)->0x00000000
							{
								continue;
							}
						}else
						{
							continue;
						}
						//hook跳转函数不能在原始函数的范围内
						if (ulHookCallFunction > ulFuncAddr && ulHookCallFunction < ulFuncAddr+ulCodeSize){
							continue;
						}
						//hook跳转函数不能为0 和 
						if (*(ULONG *)ulHookCallFunction == 0 ||
							*(ULONG *)ulHookCallFunction == 0x36f0a015){
								continue;
						}
						if (DebugOn)
							KdPrint(("found hook!!!---->%ws:%08x %x %x\r\n",KernelFuncInfo->NtosFuncInfo[i].FuncName,*(ULONG *)p,*(ULONG *)ulReloadRealFunction,*(ULONG *)ulHookCallFunction));

						for (JmpCount=0;JmpCount<10;JmpCount++)
						{
							if (MmIsAddressValidEx(ulHookCallFunction))
							{
								ulHookCallFunction = ulHookCallFunction+0x5;

								if (*ulHookCallFunction == 0xe9 ||
									*ulHookCallFunction == 0xe8)
								{
									if (DebugOn)
										KdPrint(("ulHookCallFunction == 0xe9"));

									ulHookCallFunction = *(PULONG)(ulHookCallFunction+1)+(ULONG)(ulHookCallFunction+5);

								}else
								{
									break;
								}
							}
						}
						memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
						if (!IsAddressInSystem(
							ulHookCallFunction,
							&ulHookModuleBase,
							&ulHookModuleSize,
							lpszHookModuleImage))
						{
							memset(lpszHookModuleImage,0,sizeof(lpszHookModuleImage));
							strcat(lpszHookModuleImage,"Unknown");
							ulHookModuleBase = 0;
							ulHookModuleSize = 0;
						}
						if (!MmIsAddressValidEx(&SelectModuleInlineHookInfo->InlineHook[IntHookCount]))
						{
							return;
						}
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulMemoryHookBase = ulHookCallFunction;
						memset(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpszFunction,0,sizeof(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpszFunction));
						memset(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,0,sizeof(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage));

						memcpy(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpszFunction,FuncName,strlen(FuncName));
						memcpy(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpszHookModuleImage,lpszHookModuleImage,strlen(lpszHookModuleImage));

						memcpy(SelectModuleInlineHookInfo->InlineHook[IntHookCount].lpwzRealModuleImage,KernelFuncInfo->ModulePath,wcslen(KernelFuncInfo->ModulePath)*2);
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulRealModuleBase = KernelFuncInfo->ulModuleBase;
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulMemoryFunctionBase = (ULONG)p;
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulRealFunctionBase = ulFuncAddr;
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulHookModuleBase = ulHookModuleBase;
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulHookModuleSize = ulHookModuleSize;
						SelectModuleInlineHookInfo->InlineHook[IntHookCount].ulHookType = 0;  //inline hook
						IntHookCount++;
Next:
						_asm{nop}
					}
				}
			}
		}

	}__except(EXCEPTION_EXECUTE_HANDLER){
		goto _FunctionRet;
	}
_FunctionRet:
	if (DebugOn)
		KdPrint(("IntHookCount:%d\r\n",IntHookCount));

	if (KernelFuncInfo->ulModuleBase == ulWin32kBase && bInit == TRUE){
		RKeDetachProcess();
	}
	SelectModuleInlineHookInfo->ulCount = IntHookCount;
	return TRUE;
}