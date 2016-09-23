#include "stdafx.h"
#include "EnumSymbols.h"

int FuncCount=0;

BOOLEAN InitSymHandler()
{
	HANDLE hfile;
	char Path[MAX_PATH]={0};
	char FileName[MAX_PATH]={0};
	char SymPath[MAX_PATH*2]={0};
	
	FuncCount = 0;

	if (!GetSystemDirectoryA(Path,MAX_PATH))
	{
		printf ("cannot get current directory \n");
		return FALSE;
	}
	strcat(Path,"\\symbols");
	CreateDirectoryA(Path,NULL);

	strcpy( FileName , Path);
	strcat(FileName ,"\\symsrv.yes");
	printf ("%s \n",FileName);
	hfile = CreateFileA ( FileName,
		FILE_ALL_ACCESS,
		FILE_SHARE_READ,
		NULL,
		OPEN_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (hfile == INVALID_HANDLE_VALUE)
	{
		printf ("create or open file error: 0x%X \n",GetLastError());
		return FALSE;
	}
	CloseHandle (hfile);
	
	Sleep(3000);

	hProcess = GetCurrentProcess();

	SymSetOptions ( SYMOPT_CASE_INSENSITIVE | SYMOPT_DEFERRED_LOADS | SYMOPT_UNDNAME);
	
	SymSetSearchPath(hProcess,Path);

	wsprintfA(SymPath,"SRV*%s*%s",Path,SymbolsUrl);

	if (!SymInitialize( hProcess,
		SymPath,
		TRUE))
	{
		printf ("SymInitialize failed %d \n",GetLastError());
		return FALSE;
	}

	return TRUE;
}

BOOLEAN LoadSymModule(
					  char* ImageName,
					  DWORD ModuleBase)
{
	DWORD64 tmp;
	char    SymFileName[MAX_PATH]={0};
	BOOL bRetOK = FALSE;

	HINSTANCE hmod = LoadLibraryA("Imagehlp.dll");
	if (!hmod)
		return FALSE;

	pImageLoad= (IMAGELOAD)GetProcAddress(hmod,"ImageLoad"); 
	pImageUnload= (IMAGEUNLOAD)GetProcAddress(hmod,"ImageUnload"); 
	if (!pImageLoad ||
		!pImageUnload)
		return FALSE;

	pli = pImageLoad( ImageName,NULL);
	if (pli == NULL)
	{
		printf ("cannot get loaded module of %s \n",ImageName);
		return FALSE;
	}
	printf ("ModuleName:%s:%08x\n",pli->ModuleName,pli->SizeOfImage);

	HINSTANCE hDbgHelp = LoadLibraryA("dbghelp.dll");
	if (!hDbgHelp)
		return FALSE;

	pSymGetSymbolFile = (SYMGETSYMBOLFILE)GetProcAddress(hDbgHelp,"SymGetSymbolFile"); 
	if (!pSymGetSymbolFile)
		return FALSE;

	if (pSymGetSymbolFile( hProcess,
		NULL,
		pli->ModuleName,
		sfPdb,
		SymFileName,
		MAX_PATH,
		SymFileName,
		MAX_PATH))
	{
		tmp =  SymLoadModule64( hProcess,
			pli->hFile,
			pli->ModuleName,
			NULL,
			(DWORD64)ModuleBase,
			pli->SizeOfImage);
		if (tmp)
		{
			bRetOK =  TRUE;
		}
	}
	pImageUnload(pli);

	return bRetOK;
}


BOOLEAN EnumSyms(
				 char* ImageName,
				 DWORD ModuleBase,
				 PSYM_ENUMERATESYMBOLS_CALLBACK EnumRoutine,
				 PVOID Context)
{
	BOOLEAN bEnum;
	
	if ( !LoadSymModule( ImageName , ModuleBase) )
	{
		return FALSE;
	}
	
	bEnum = SymEnumSymbols( hProcess,
		ModuleBase,
		NULL,
		EnumRoutine,
		Context);
	if (!bEnum)
	{
		printf ("cannot enum symbols ,error: %x \n",GetLastError());
	}
	return bEnum;
}

BOOLEAN CALLBACK EnumSymRoutine(
								PSYMBOL_INFO psi,
								ULONG     SymSize,
								PVOID     Context)
{
	if (strstr(psi->Name,"_imp") != 0)
	{
		return TRUE;
	}
	MultiByteToWideChar(
		CP_ACP,
		0, 
		psi->Name,
		-1, 
		NtosFuncAddressInfo->NtosFuncInfo[FuncCount].FuncName, 
		strlen(psi->Name)
		);
	NtosFuncAddressInfo->NtosFuncInfo[FuncCount].NumberOfFunctions = FuncCount;
	NtosFuncAddressInfo->NtosFuncInfo[FuncCount].ulAddress = psi->Address;
	NtosFuncAddressInfo->NtosFuncInfo[FuncCount].ulReloadAddress = 0;  //先给0，等到了内核，再多给
	FuncCount++;
	NtosFuncAddressInfo->ulCount = FuncCount;

	return TRUE;
}
BOOLEAN CALLBACK EnumSelectSymRoutine(
	PSYMBOL_INFO psi,
	ULONG     SymSize,
	PVOID     Context)
{
	if (strstr(psi->Name,"_imp") != 0)
	{
		return TRUE;
	}
	MultiByteToWideChar(
		CP_ACP,
		0, 
		psi->Name,
		-1, 
		FuncAddressInfo->NtosFuncInfo[FuncCount].FuncName, 
		strlen(psi->Name)
		);
	FuncAddressInfo->NtosFuncInfo[FuncCount].NumberOfFunctions = FuncCount;
	FuncAddressInfo->NtosFuncInfo[FuncCount].ulAddress = psi->Address;
	FuncAddressInfo->NtosFuncInfo[FuncCount].ulReloadAddress = 0;  //先给0，等到了内核，再多给
	FuncCount++;
	FuncAddressInfo->ulCount = FuncCount;

// 	CHAR lpszOut[260] = {0};
// 	memset(lpszOut,0,sizeof(lpszOut));
// 	wsprintfA(lpszOut,"%s %08X\n",psi->Name,psi->Address);
// 	OutputDebugStringA(lpszOut);

	return TRUE;
}
//////////////////////////////////////////////////////////////////////////
ULONG GetKernelInfo(char *lpKernelName,ULONG *ulBase,ULONG *ulSize)
{
	DWORD	dwsize;
	DWORD	dwSizeReturn;
	PUCHAR	pBuffer	=	NULL;

	PMODULES	pSmi=NULL;
	NTSTATUS	ntStatus=STATUS_UNSUCCESSFUL;

	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation,
		pSmi, 
		0, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_INFO_LENGTH_MISMATCH)
	{
		return 0;
	}
	dwsize	=	dwSizeReturn*2;
	pSmi	=	(PMODULES)new char[dwsize];
	if (pSmi==NULL)
	{
		return 0;
	}
	
	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation, 
		pSmi,
		dwsize, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_SUCCESS)
	{
		return 0;
	}
	*ulBase	= pSmi->smi[0].Base;
	*ulSize = pSmi->smi[0].Size;

	strcpy(lpKernelName, pSmi->smi[0].ImageName);

	delete pSmi;
	
	return TRUE;
}
void LoadNtkrnlSym(void)
{
	ULONG ulBase;
	ULONG ulSize;
	char lpszKernelPath[260] = {0};
	char lpszKernelName[50] = {0};
	int i=0;
	DWORD dwReadByte;

	if (NtosFuncAddressInfo)
	{
		if (NtosFuncAddressInfo->ulCount > 100)
		{
			return;
		}
	}
	if (InitSymHandler())
	{
		if (GetKernelInfo(lpszKernelPath,&ulBase,&ulSize))
		{
// 			CHAR lpszOut[260] = {0};
// 
// 			memset(lpszOut,0,sizeof(lpszOut));
// 			wsprintfA(lpszOut,"kernel:%s : 0x%X \n",lpszKernelPath,ulBase);
// 			OutputDebugStringA(lpszOut);

			wsprintfA(lpszKernelName,"%s",ExtractFileName(lpszKernelPath));

			NtosFuncAddressInfo = (PNTOSFUNCINFO)VirtualAlloc(0,ulSize+260,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (NtosFuncAddressInfo)
			{
				memset(NtosFuncAddressInfo,0,ulSize+260);
				EnumSyms(lpszKernelName,ulBase,(PSYM_ENUMERATESYMBOLS_CALLBACK)EnumSymRoutine,NULL);
				SymUnloadModule64(GetCurrentProcess(),ulBase);
				SymCleanup(GetCurrentProcess());

				MultiByteToWideChar(
					CP_ACP,
					0, 
					lpszKernelPath,
					-1, 
					NtosFuncAddressInfo->ModulePath, 
					strlen(lpszKernelPath)
					);
				NtosFuncAddressInfo->ulModuleBase = ulBase;

				//把数据传到内核~~！
				ReadFile((HANDLE)INIT_PDB_KERNEL_INFO,NtosFuncAddressInfo,ulSize+260,&dwReadByte,0);

// 				for (i=0;i<NtosFuncAddressInfo->ulCount;i++)
// 				{
// 
// 					WCHAR lpszOut[260] = {0};
// 					memset(lpszOut,0,sizeof(lpszOut));
// 					wsprintfW(lpszOut,L"%ws : 0x%X \n",NtosFuncAddressInfo->NtosFuncInfo[i].FuncName,NtosFuncAddressInfo->NtosFuncInfo[i].ulAddress);
// 					OutputDebugStringW(lpszOut);
// 				}
//				VirtualFree(NtosFuncAddressInfo,ulSize+260,MEM_RESERVE | MEM_COMMIT);
			}
		}
	}
}
VOID FixSelectModuleToKernel(ULONG ulModuleBase,WCHAR *ModulePath,char *lpszModulePath)
{
	CHAR lpszKernelPath[260] = {0};

	ULONG ulKernelBase;
	ULONG ulKernelSize;
	int i=0;
	DWORD dwReadByte;

	if (InitSymHandler())
	{
		if (GetKernelInfo(lpszKernelPath,&ulKernelBase,&ulKernelSize))
		{
// 			WCHAR lpszOut[260] = {0};
// 			memset(lpszOut,0,sizeof(lpszOut));
// 			wsprintfW(lpszOut,L"kernel:0x%X : 0x%X \n",ulKernelBase,ulKernelSize);
// 			OutputDebugStringW(lpszOut);

			FuncAddressInfo = (PNTOSFUNCINFO)VirtualAlloc(0,ulKernelSize+260,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (FuncAddressInfo)
			{
				memset(FuncAddressInfo,0,ulKernelSize+260);
				//下载符号
				EnumSyms(lpszModulePath,ulModuleBase,(PSYM_ENUMERATESYMBOLS_CALLBACK)EnumSelectSymRoutine,NULL);
				SymUnloadModule64(GetCurrentProcess(),ulModuleBase);
				SymCleanup(GetCurrentProcess());

				wcscat(FuncAddressInfo->ModulePath,ModulePath);

				FuncAddressInfo->ulModuleBase = ulModuleBase;

				//把数据传到内核初始化一下~~！
				ReadFile((HANDLE)INIT_SELECT_MODULE_INLINE_HOOK,FuncAddressInfo,ulKernelSize+260,&dwReadByte,0);
// 
// 				for (i=0;i<FuncAddressInfo->ulCount;i++)
// 				{
// 
// 					WCHAR lpszOut[260] = {0};
// 					memset(lpszOut,0,sizeof(lpszOut));
// 					wsprintfW(lpszOut,L"fuck:%ws : 0x%X \n",FuncAddressInfo->NtosFuncInfo[i].FuncName,FuncAddressInfo->NtosFuncInfo[i].ulAddress);
// 					OutputDebugStringW(lpszOut);
// 				}
				VirtualFree(FuncAddressInfo,ulKernelSize+260,MEM_RESERVE | MEM_COMMIT);
			}else
				OutputDebugStringW(L"申请内存失败\n");
		}
	}
}
//填充结构
VOID FixPDBModule()
{
	CHAR lpszKernelPath[260] = {0};
	CHAR lpszKernelName[100] = {0};
	ULONG ulKernelBase;
	ULONG ulKernelSize;
	int i=0;
	DWORD dwReadByte;

	if (InitSymHandler())
	{
		if (GetKernelInfo(lpszKernelPath,&ulKernelBase,&ulKernelSize))
		{
			ulSize = ulKernelSize;

// 			WCHAR lpszOut[260] = {0};
// 			memset(lpszOut,0,sizeof(lpszOut));
// 			wsprintfW(lpszOut,L"kernel:0x%X : 0x%X \n",ulKernelBase,ulKernelSize);
// 			OutputDebugStringW(lpszOut);

			wsprintfA(lpszKernelName,"%s",ExtractFileName(lpszKernelPath));

			if (FuncAddressInfo)
				VirtualFree(FuncAddressInfo,ulKernelSize+260,MEM_RESERVE | MEM_COMMIT);

			FuncAddressInfo = (PNTOSFUNCINFO)VirtualAlloc(0,ulKernelSize+260,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (FuncAddressInfo)
			{
				memset(FuncAddressInfo,0,ulKernelSize+260);
				//下载符号
				EnumSyms(lpszKernelName,ulKernelBase,(PSYM_ENUMERATESYMBOLS_CALLBACK)EnumSelectSymRoutine,NULL);
				SymUnloadModule64(GetCurrentProcess(),ulKernelBase);
				SymCleanup(GetCurrentProcess());

				FuncAddressInfo->ulModuleBase = ulKernelBase;

				wcscat(FuncAddressInfo->ModulePath,L"ntkrnlpa.exe");

// 				for (i=0;i<FuncAddressInfo->ulCount;i++)
// 				{
// 					WCHAR lpszOut[260] = {0};
// 					memset(lpszOut,0,sizeof(lpszOut));
// 					wsprintfW(lpszOut,L"1111:%ws : 0x%X \n",FuncAddressInfo->NtosFuncInfo[i].FuncName,FuncAddressInfo->NtosFuncInfo[i].ulAddress);
// 					OutputDebugStringW(lpszOut);
// 				}
			}else
				OutputDebugStringW(L"申请内存失败\n");
		}
	}
}
BOOL IsStackAddrInModule(ULONG ulStack,CHAR *lpszModule,ULONG *ulModule)
{
	DWORD	dwsize;
	DWORD	dwSizeReturn;
	PUCHAR	pBuffer	=	NULL;
	BOOL bRetOK = FALSE;
	PMODULES	pSmi=NULL;
	NTSTATUS	ntStatus=STATUS_UNSUCCESSFUL;

	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation,
		pSmi, 
		0, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_INFO_LENGTH_MISMATCH)
	{
		return bRetOK;
	}
	dwsize	=	dwSizeReturn*2;
	pSmi	=	(PMODULES)new char[dwsize];
	if (pSmi)
	{
		ntStatus = ZwQuerySystemInformation(
			SystemModuleInformation, 
			pSmi,
			dwsize, 
			&dwSizeReturn
			);
		if (ntStatus==STATUS_SUCCESS)
		{
			for (int i=0;i<pSmi->ulCount;i++)
			{
				if (ulStack > pSmi->smi[i].Base && ulStack < pSmi->smi[i].Base + pSmi->smi[i].Size)
				{
					memcpy(lpszModule,pSmi->smi[i].ImageName,strlen(pSmi->smi[i].ImageName)*2);
					*ulModule = pSmi->smi[i].Base;

					bRetOK = TRUE;
					break;
				}
			}
		}
		delete pSmi;
	}
	return bRetOK;
}
//开始解析
VOID FormatStack(ULONG ulStack,WCHAR *lpwzFormatOut)
{
	BOOL bIsKernelFunc = FALSE;
	BOOL bIsOtherFunc = FALSE;
	ULONG ulTemp=0,ulTemp1=0;
	CHAR lpszModule[260] = {0};
	CHAR lpszModule1[260] = {0};
	WCHAR lpwzModule[260] = {0};
	ULONG ulModule = 0;

	if (ulStack > FuncAddressInfo->ulModuleBase &&
		ulStack < FuncAddressInfo->ulModuleBase + ulSize)
	{
		bIsKernelFunc = TRUE;
	}
	//不是系统模块的函数
	if (!bIsKernelFunc){
		memset(lpszModule,0,sizeof(lpszModule));
		if (IsStackAddrInModule(ulStack,lpszModule,&ulModule))
		{
			bIsOtherFunc = TRUE;

			if (strstr(lpszModule,"\\") != 0)
			{
				wsprintfA(lpszModule1,"%s",ExtractFileName(lpszModule));
				MultiByteToWideChar(
					CP_ACP,
					0, 
					lpszModule1,
					-1, 
					lpwzModule, 
					strlen(lpszModule1)
					);
				//wsprintfW(lpwzFormatOut,L"%ws!+0x00 --> 0x%08x",lpwzModule,ulStack);
			}else{
				MultiByteToWideChar(
					CP_ACP,
					0, 
					lpszModule,
					-1, 
					lpwzModule, 
					strlen(lpszModule)
					);
				//wsprintfW(lpwzFormatOut,L"%ws!+0x00 --> 0x%08x",lpwzModule,ulStack);
			}
		}else
		{
			wsprintfW(lpwzFormatOut,L"unknown!+0x00 --> 0x%08x",ulStack);
			return;
		}
	}
	//其他模块的解析
	if (bIsOtherFunc)
	{
		//***************************************************************
		WCHAR lpwzDosFullPath[256];
		WCHAR lpwzWinDir[256];
		WCHAR lpwzSysDisk[256];
		WCHAR lpwzFullSysName[1024] = {0};
		CHAR lpszPath[1024] = {0};

		memset(lpwzWinDir,0,sizeof(lpwzWinDir));
		memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
		memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
		memset(lpwzFullSysName,0,sizeof(lpwzFullSysName));
		memset(lpszPath,0,sizeof(lpszPath));

		MultiByteToWideChar(
			CP_ACP,
			0, 
			lpszModule,
			-1, 
			lpwzFullSysName, 
			strlen(lpszModule)
			);

		GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
		memcpy(lpwzSysDisk,lpwzWinDir,4);

		if (wcsstr(lpwzFullSysName,L"\\??\\"))
		{
			//开始这种路径的处理
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcsncpy(lpwzDosFullPath,lpwzFullSysName+wcslen(L"\\??\\"),wcslen(lpwzFullSysName)-wcslen(L"\\??\\"));
			goto Next;
		}
		if (wcsstr(lpwzFullSysName,L"\\WINDOWS\\system32\\"))
		{
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzDosFullPath,lpwzSysDisk);
			wcscat(lpwzDosFullPath,lpwzFullSysName);
			goto Next;
		}
		if (wcsstr(lpwzFullSysName,L"\\SystemRoot\\"))
		{
			WCHAR lpwzTemp[256];
			memset(lpwzTemp,0,sizeof(lpwzTemp));
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzTemp,lpwzSysDisk);
			wcscat(lpwzTemp,L"\\WINDOWS\\");
			wcscat(lpwzDosFullPath,lpwzTemp);
			wcsncpy(lpwzDosFullPath+wcslen(lpwzTemp),lpwzFullSysName+wcslen(L"\\SystemRoot\\"),wcslen(lpwzFullSysName) - wcslen(L"\\SystemRoot\\"));
			goto Next;
		}
		if (wcsstr(lpwzFullSysName,L"\\") == 0)
		{
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzDosFullPath,lpwzSysDisk);
			wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
			wcscat(lpwzDosFullPath,lpwzFullSysName);
		}
Next:
		if (InitSymHandler())
		{
			if (FuncAddressInfo)
				VirtualFree(FuncAddressInfo,ulSize+260,MEM_RESERVE | MEM_COMMIT);

			FuncAddressInfo = (PNTOSFUNCINFO)VirtualAlloc(0,ulSize+260,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
			if (FuncAddressInfo)
			{
				memset(FuncAddressInfo,0,ulSize+260);

				wcscat(FuncAddressInfo->ModulePath,lpwzModule);

				char lpszPDBModule[260] ={0};
				//下载符号
				WideCharToMultiByte(CP_ACP,
					0,
					lpwzDosFullPath,
					-1,
					lpszPDBModule,
					wcslen(lpwzDosFullPath)*2,
					NULL,
					NULL
					);
				//OutputDebugStringA(lpszPDBModule);

				EnumSyms(lpszPDBModule,ulModule,(PSYM_ENUMERATESYMBOLS_CALLBACK)EnumSelectSymRoutine,NULL);
				SymUnloadModule64(GetCurrentProcess(),ulModule);
				SymCleanup(GetCurrentProcess());
			}
		}
		//***************************************************************
	}
	for (int i=0;i<FuncAddressInfo->ulCount;i++)
	{
		ulTemp = ulStack - FuncAddressInfo->NtosFuncInfo[i].ulAddress;
		//得到正数
		if (ulTemp > 0){
			//这里取相减最小的那个值
			//如果当前的大于之前得，就继续，
			if (ulTemp > ulTemp1 && ulTemp1 != 0){
				continue;
			}
			//取最小的值
			ulTemp1 = ulTemp;
			memset(lpwzFormatOut,0,sizeof(lpwzFormatOut));
			wsprintfW(lpwzFormatOut,L"%ws!%ws+0x%x --> 0x%08x",FuncAddressInfo->ModulePath,FuncAddressInfo->NtosFuncInfo[i].FuncName,ulTemp1,ulStack);
		}else{
			ulTemp = 0;
		}
	}
	if (!ulTemp1){
		wsprintfW(lpwzFormatOut,L"unknown!+0x00 --> 0x%08x",ulStack);
		return;
	}
}