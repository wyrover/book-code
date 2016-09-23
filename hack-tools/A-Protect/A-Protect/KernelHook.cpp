#include "stdafx.h"
#include "kernelhook.h"
#include "A-ProtectView.h"
#include "A-Protect.h"

extern BOOL bIsNtosOrSelect;  //是单独扫描ntkrnlpa还是扫描其他

VOID QueryKernelHook(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;
	CHAR lpszKernelPath[260] = {0};

	ULONG ulKernelBase;
	ULONG ulKernelSize;

	int ItemNum = m_list->GetItemCount();

	SHFILEINFO shfileinfo;
	KernelHookImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;
	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();

	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---内核钩子---]\r\n",PhysicalFile);
	}
	if (!bIsNtosOrSelect)
		SetDlgItemTextW(m_hWnd,ID,L"正在扫描内核钩子，请稍后...");

	if (!bIsNtosOrSelect)
		InlineHookInfo = (PINLINEHOOKINFO)VirtualAlloc(0, sizeof(INLINEHOOKINFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	else{
		if (GetKernelInfo(lpszKernelPath,&ulKernelBase,&ulKernelSize))
		{
			InlineHookInfo = (PINLINEHOOKINFO)VirtualAlloc(0, ulKernelSize+1024,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		}
	}
	if (InlineHookInfo)
	{
		if (!bIsNtosOrSelect)
			ReadFile((HANDLE)LIST_INLINEHOOK,InlineHookInfo,sizeof(INLINEHOOKINFO)*1025,&dwReadByte,0);
		else
			ReadFile((HANDLE)LIST_SELECT_MODULE_INLINE_HOOK,InlineHookInfo,ulKernelSize+1024,&dwReadByte,0);
		
		for (i=0;i< (int)InlineHookInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",InlineHookInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzHookType[50] = {0};
			WCHAR lpwzMemoryFunctionBase[256] = {0};
			WCHAR lpwzRealFunctionBase[256] = {0};
			WCHAR lpwzMemoryHookBase[256] = {0};
			WCHAR lpwzFunction[256] = {0};

			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			WCHAR lpwzRealModuleBase[256] = {0};

			memset(lpwzMemoryFunctionBase,0,sizeof(lpwzMemoryFunctionBase));
			memset(lpwzRealFunctionBase,0,sizeof(lpwzRealFunctionBase));
			memset(lpwzMemoryHookBase,0,sizeof(lpwzMemoryHookBase));
			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			memset(lpwzHookType,0,sizeof(lpwzHookType));

			if (!InlineHookInfo->InlineHook[i].ulRealFunctionBase ||
				!InlineHookInfo->InlineHook[i].ulMemoryHookBase ||
				!InlineHookInfo->InlineHook[i].ulRealModuleBase)
			{
				continue;
			}
			wsprintfW(lpwzRealFunctionBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulRealFunctionBase);

			wsprintfW(lpwzMemoryHookBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulMemoryHookBase);

			wsprintfW(lpwzRealModuleBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulRealModuleBase);

			MultiByteToWideChar(
				CP_ACP,
				0, 
				InlineHookInfo->InlineHook[i].lpszFunction,
				-1, 
				lpwzFunction, 
				strlen(InlineHookInfo->InlineHook[i].lpszFunction)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				InlineHookInfo->InlineHook[i].lpszHookModuleImage,
				-1, 
				lpwzHookModuleImage, 
				strlen(InlineHookInfo->InlineHook[i].lpszHookModuleImage)
				);
			//wsprintfW(lpwzMemoryHookBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulMemoryHookBase);
			wsprintfW(lpwzHookModuleBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulHookModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%X",InlineHookInfo->InlineHook[i].ulHookModuleSize);

			switch (InlineHookInfo->InlineHook[i].ulHookType)
			{
			case 0:
				wsprintfW(lpwzMemoryFunctionBase,L"0x%08X",InlineHookInfo->InlineHook[i].ulMemoryFunctionBase);

				if (!bIsNtosOrSelect)
					wcscat_s(lpwzHookType,L"Inline Hook");
				else
					wcscat_s(lpwzHookType,L"Select Inline");

				break;
			case 1:
				wsprintfW(lpwzMemoryFunctionBase,L"%d",InlineHookInfo->InlineHook[i].ulMemoryFunctionBase);

				wcscat_s(lpwzHookType,L"Eat Hook");

				break;
			}
			WCHAR lpwzDosFullPath[256];
			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[256];

			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));

			GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
			memcpy(lpwzSysDisk,lpwzWinDir,4);

			if (wcsstr(lpwzHookModuleImage,L"\\??\\"))
			{
				//开始这种路径的处理
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcsncpy(lpwzDosFullPath,lpwzHookModuleImage+wcslen(L"\\??\\"),wcslen(lpwzHookModuleImage)-wcslen(L"\\??\\"));
				goto Next;
			}
			if (wcsstr(lpwzHookModuleImage,L"\\WINDOWS\\system32\\"))
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,lpwzHookModuleImage);
				//MessageBoxW(lpwzDosFullPath,lpwzFullSysName,0);
				goto Next;
			}
			if (wcsstr(lpwzHookModuleImage,L"\\SystemRoot\\"))
			{
				WCHAR lpwzTemp[256];
				memset(lpwzTemp,0,sizeof(lpwzTemp));
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzTemp,lpwzSysDisk);
				wcscat(lpwzTemp,L"\\WINDOWS\\");
				wcscat(lpwzDosFullPath,lpwzTemp);
				wcsncpy(lpwzDosFullPath+wcslen(lpwzTemp),lpwzHookModuleImage+wcslen(L"\\SystemRoot\\"),wcslen(lpwzHookModuleImage) - wcslen(L"\\SystemRoot\\"));
				goto Next;
			}
Next:
			//这里是一键体检的数据，不需要插入界面了
			if (bIsPhysicalCheck){
				WCHAR lpwzSaveBuffer[1024] ={0};
				CHAR lpszSaveBuffer[2024] ={0};
				memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
				memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

				wsprintfW(lpwzSaveBuffer,L"          --> 发现Hook--->当前地址:%ws | 原始地址:%ws | 函数名:%ws | 内核模块:%ws | Hook类型:%ws\r\n",
					lpwzMemoryFunctionBase,lpwzRealFunctionBase,lpwzFunction,lpwzDosFullPath,lpwzHookType);

				m_list->InsertItem(0,L"内核钩子",RGB(77,77,77));
				m_list->SetItemText(0,1,lpwzSaveBuffer);

				WideCharToMultiByte( CP_ACP,
					0,
					lpwzSaveBuffer,
					-1,
					lpszSaveBuffer,
					wcslen(lpwzSaveBuffer)*2,
					NULL,
					NULL
					);
				SaveToFile(lpszSaveBuffer,PhysicalFile);
				continue;
			}
			m_list->InsertItem(i,lpwzMemoryFunctionBase,RGB(255,20,147));
			m_list->SetItemText(i,1,lpwzRealFunctionBase);
			m_list->SetItemText(i,2,lpwzFunction);
			m_list->SetItemText(i,3,lpwzMemoryHookBase);
			m_list->SetItemText(i,4,lpwzDosFullPath);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
		    m_list->SetItemText(i,7,InlineHookInfo->InlineHook[i].lpwzRealModuleImage);
			m_list->SetItemText(i,8,lpwzRealModuleBase);
			m_list->SetItemText(i,9,lpwzHookType);

			if(GetFileAttributes(lpwzDosFullPath)!=INVALID_FILE_ATTRIBUTES)
			{
				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
				KernelHookImg.Add(shfileinfo.hIcon);
			}else
				KernelHookImg.Add(imgApp->LoadIconW(IDI_WHITE));	

			m_list->SetImageList(&KernelHookImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);

		}
		VirtualFree(InlineHookInfo,sizeof(INLINEHOOKINFO)*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"内核Hook扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
VOID UnHookKernelHookSelect(HWND m_hWnd,CMyList *m_list)
{
	DWORD dwReadByte;
	CString HookType;
	CString FunctionStr;
	CString FuncRealBase;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	WCHAR lpwzFunction[100];

	memset(lpwzFunction,0,sizeof(lpwzFunction));
	FunctionStr.Format(L"%s",m_list->GetItemText(Item,2));
	if (!wcslen(FunctionStr))
	{
		return;
	}
	if (wcsstr(FunctionStr,L"/NextCallHook"))
	{
		MessageBoxW(m_hWnd,L"暂时不支持未导出函数！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	HookType.Format(L"%s",m_list->GetItemText(Item,9));
	if (!wcslen(HookType))
	{
		return;
	}
	WCHAR lpwzFuncBase1[50] = {0};
	WCHAR lpwzFuncBase[50] = {0};
	CHAR lpszFuncBase[50] = {0};

	FuncRealBase.Format(L"%s",m_list->GetItemText(Item,1));
	wcscat(lpwzFuncBase1,FuncRealBase);
	memcpy(lpwzFuncBase,lpwzFuncBase1+wcslen(L"0x"),wcslen(lpwzFuncBase1)*2-wcslen(L"0x"));

	WideCharToMultiByte( CP_ACP,
		0,
		lpwzFuncBase,
		-1,
		lpszFuncBase,
		wcslen(lpwzFuncBase)*2,
		NULL,
		NULL
		);
	ULONG ulRealFuncBase = StringToHex(lpszFuncBase);

	WCHAR lpwzModuleBase1[50] = {0};
	WCHAR lpwzModuleBase[50] = {0};
	CHAR lpszModuleBase[50] = {0};
	CString ModuleRealBase;

	ModuleRealBase.Format(L"%s",m_list->GetItemText(Item,8));
	wcscat(lpwzModuleBase1,ModuleRealBase);
	memcpy(lpwzModuleBase,lpwzModuleBase1+wcslen(L"0x"),wcslen(lpwzModuleBase1)*2-wcslen(L"0x"));

	WideCharToMultiByte( CP_ACP,
		0,
		lpwzModuleBase,
		-1,
		lpszModuleBase,
		wcslen(lpwzModuleBase)*2,
		NULL,
		NULL
		);

	ULONG ulRealModuleBase = StringToHex(lpszModuleBase);

	if (_wcsnicmp(HookType,L"Select Inline",wcslen(L"Select Inline")) ==0)
	{
		//初始化原始的地址
		ReadFile((HANDLE)INIT_SET_SELECT_INLINE_HOOK,0,ulRealFuncBase,&dwReadByte,0);

		//初始化原始模块地址
		ReadFile((HANDLE)INIT_SET_SELECT_INLINE_HOOK_1,0,ulRealModuleBase,&dwReadByte,0);

		WCHAR lpwzModulePath[260] = {0};
		CString ModulePath;

		memset(lpwzModulePath,0,sizeof(lpwzModulePath));
		ModulePath.Format(L"%s",m_list->GetItemText(Item,7)); //获取原始模块路径
		wcscat(lpwzModulePath,ModulePath);

		ReadFile((HANDLE)SET_SELECT_INLINE_HOOK,lpwzModulePath,wcslen(lpwzModulePath),&dwReadByte,0);
		//删除所选的行
		m_list->DeleteItem(Item);
	}
	if (_wcsnicmp(HookType,L"Inline Hook",wcslen(L"Inline Hook")) ==0)
	{
		ReadFile((HANDLE)SET_INLINE_HOOK,0,ulRealFuncBase,&dwReadByte,0);

		m_list->DeleteAllItems();
		QueryKernelHook(m_hWnd,IDC_DebugStatus,m_list);
	}
	if (_wcsnicmp(HookType,L"Eat Hook",wcslen(L"Eat Hook")) ==0)
	{
		CString NumberOfFunctions;
		WCHAR Number[10] = {0};
		CHAR lpNumber[10] = {0};
		int x=0;

		memset(Number,0,sizeof(Number));
		NumberOfFunctions.Format(L"%s",m_list->GetItemText(Item,0)); //获取EAT的导入函数位置

 		wcscat_s(Number,NumberOfFunctions);

		WideCharToMultiByte( CP_ACP,
			0,
			Number,
			-1,
			lpNumber,
			wcslen(Number)*2,
			NULL,
			NULL);
		x = atoi(lpNumber);

		//初始化EAT的导入函数位置
 		ReadFile((HANDLE)INIT_EAT_NUMBER,0,x,&dwReadByte,0);

		//初始化EAT的原始地址
		ReadFile((HANDLE)INIT_EAT_REAL_ADDRESS,0,ulRealFuncBase,&dwReadByte,0);

		//初始化模块的基址，并恢复EAT hook
		ReadFile((HANDLE)SET_EAT_HOOK,0,ulRealModuleBase,&dwReadByte,0);
		
		//删除所选的行
		m_list->DeleteItem(Item);
	}
}
VOID ByPassKernelHook(HWND m_hWnd,CMyList *m_list)
{
	DWORD dwReadByte;
	CString HookType;
	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	WCHAR lpwzFunction[100];

	memset(lpwzFunction,0,sizeof(lpwzFunction));
	FunctionStr.Format(L"%s",m_list->GetItemText(Item,2));
	if (!wcslen(FunctionStr))
	{
		return;
	}
	if (wcsstr(FunctionStr,L"/NextCallHook"))
	{
		MessageBoxW(m_hWnd,L"暂时不支持未导出函数！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	HookType.Format(L"%s",m_list->GetItemText(Item,9));
	if (!wcslen(HookType))
	{
		return;
	}
	if (_wcsnicmp(HookType,L"Eat Hook",wcslen(L"Eat Hook")) ==0)
	{
		MessageBoxW(m_hWnd,L"暂时不支持绕过Eat Hook！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	WCHAR lpwzFuncBase1[50] = {0};
	WCHAR lpwzFuncBase[50] = {0};
	CHAR lpszFuncBase[50] = {0};
	CString FuncRealBase;

	FuncRealBase.Format(L"%s",m_list->GetItemText(Item,1));
	wcscat(lpwzFuncBase1,FuncRealBase);
	memcpy(lpwzFuncBase,lpwzFuncBase1+wcslen(L"0x"),wcslen(lpwzFuncBase1)*2-wcslen(L"0x"));

	WideCharToMultiByte( CP_ACP,
		0,
		lpwzFuncBase,
		-1,
		lpszFuncBase,
		wcslen(lpwzFuncBase)*2,
		NULL,
		NULL
		);
	ULONG ulRealFuncBase = StringToHex(lpszFuncBase);

	WCHAR lpwzModuleBase1[50] = {0};
	WCHAR lpwzModuleBase[50] = {0};
	CHAR lpszModuleBase[50] = {0};
	CString ModuleRealBase;

	ModuleRealBase.Format(L"%s",m_list->GetItemText(Item,8));
	wcscat(lpwzModuleBase1,ModuleRealBase);
	memcpy(lpwzModuleBase,lpwzModuleBase1+wcslen(L"0x"),wcslen(lpwzModuleBase1)*2-wcslen(L"0x"));

	WideCharToMultiByte( CP_ACP,
		0,
		lpwzModuleBase,
		-1,
		lpszModuleBase,
		wcslen(lpwzModuleBase)*2,
		NULL,
		NULL
		);

	ULONG ulRealModuleBase = StringToHex(lpszModuleBase);

	//MessageBoxW(0,lpwzFuncBase,ModuleRealBase,0);

	WCHAR lpwzForMat[256];
	memset(lpwzForMat,0,sizeof(lpwzForMat));
	wsprintfW(lpwzForMat,L"绕过\"%ws\"函数的hook有可能导致对方软件部分功能失效，是否继续？\r\n",FunctionStr);

	if (MessageBoxW(m_hWnd,lpwzForMat,L"A盾电脑防护",MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		if (_wcsnicmp(HookType,L"Inline Hook",wcslen(L"Inline Hook")) ==0)
		{
			//wcscat_s(lpwzFunction,FunctionStr);
			ReadFile((HANDLE)ANTI_INLINEHOOK,0,ulRealFuncBase,&dwReadByte,0);
		}
		if (_wcsnicmp(HookType,L"Select Inline",wcslen(L"Select Inline")) ==0)
		{
			//初始化原始的地址
			ReadFile((HANDLE)INIT_SET_SELECT_INLINE_HOOK,0,ulRealFuncBase,&dwReadByte,0);

			//初始化原始模块地址
			ReadFile((HANDLE)INIT_SET_SELECT_INLINE_HOOK_1,0,ulRealModuleBase,&dwReadByte,0);

			WCHAR lpwzModulePath[260] = {0};
			CString ModulePath;

			memset(lpwzModulePath,0,sizeof(lpwzModulePath));
			ModulePath.Format(L"%s",m_list->GetItemText(Item,7)); //获取原始模块路径
			wcscat(lpwzModulePath,ModulePath);

			//MessageBoxW(0,lpwzModulePath,0,0);

			ReadFile((HANDLE)ANTI_SELECT_INLINE_HOOK,lpwzModulePath,wcslen(lpwzModulePath),&dwReadByte,0);
		}
		MessageBoxW(m_hWnd,L"操作已经成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
	return;
}
void CopyKernelHookDataToClipboard(HWND m_hWnd,CMyList *m_list)
{
	CString KernelHook;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	KernelHook.Format(L"%s",m_list->GetItemText(Item,2));

	WCHAR lpwzKernelHook[260];

	memset(lpwzKernelHook,0,sizeof(lpwzKernelHook));
	wcscat_s(lpwzKernelHook,KernelHook);
	CHAR lpszKernelHook[1024];
	char *lpString = NULL;

	memset(lpwzKernelHook,0,sizeof(lpwzKernelHook));
	memset(lpszKernelHook,0,sizeof(lpszKernelHook));
	wcscat_s(lpwzKernelHook,KernelHook);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzKernelHook,
		-1,
		lpszKernelHook,
		wcslen(lpwzKernelHook)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszKernelHook);
	if (lpString)
	{
		MessageBoxW(m_hWnd,L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}
VOID QueryAllKernelHook(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD	dwsize;
	DWORD	dwSizeReturn;
	PUCHAR	pBuffer	=	NULL;
	PMODULES	pSmi=NULL;
	NTSTATUS	ntStatus=STATUS_UNSUCCESSFUL;
	int x=0;

	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation,
		pSmi, 
		0, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_INFO_LENGTH_MISMATCH)
	{
		return;
	}
	dwsize	=	dwSizeReturn*2;
	pSmi	=	(PMODULES)new char[dwsize];
	if (pSmi==NULL)
	{
		return;
	}

	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation, 
		pSmi,
		dwsize, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_SUCCESS)
	{
		return;
	}
	m_list->DeleteAllItems();

	for (int i=0;i<pSmi->ulCount;i++)
	{
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
			pSmi->smi[i].ImageName,
			-1, 
			lpwzFullSysName, 
			strlen(pSmi->smi[i].ImageName)
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
		CHAR ShartPath[50] = {0};
		WideCharToMultiByte( CP_ACP,
			0,
			lpwzDosFullPath,
			-1,
			lpszPath,
			wcslen(lpwzDosFullPath)*2,
			NULL,
			NULL
			);
		if (strstr(lpszPath,"\\") != 0)
		{
			memset(ShartPath,0,sizeof(ShartPath));
			wsprintfA(ShartPath,"%s",ExtractFileName(lpszPath));
		}
//		FixSelectModuleToKernel(pSmi->smi[i].Base,lpwzDosFullPath,lpszPath);
//		QueryKernelHook(m_hWnd,ID,m_list);

		if (strcmpi(ShartPath,"acpi.SYS") == 0 ||
			strcmpi(ShartPath,"atapi.SYS") == 0 ||
			strcmpi(ShartPath,"disk.SYS") == 0 ||
			strcmpi(ShartPath,"fltMgr.SYS") == 0 ||
			strcmpi(ShartPath,"http.SYS") == 0 ||
			strcmpi(ShartPath,"ipfltdrv.SYS") == 0 ||
			strcmpi(ShartPath,"kbdclass.SYS") == 0 ||
			strcmpi(ShartPath,"ndis.SYS") == 0 ||
			strcmpi(ShartPath,"ntfs.SYS") == 0 ||
			strcmpi(ShartPath,"tcpip.SYS") == 0 ||
			strcmpi(ShartPath,"tdi.SYS") == 0 ||
			strcmpi(ShartPath,"classpnp.SYS") == 0 ||
			strcmpi(ShartPath,"nsiproxy.SYS") == 0 ||
			strcmpi(ShartPath,"hal.dll") == 0 ||
			strcmpi(ShartPath,"KDCOM.dll") == 0 ||
			strcmpi(ShartPath,"win32k.sys") == 0 ||
			i == 0)  //i为0是ntkrnlpa
		{
			if (bIsStopHookScan){
				break;
			}
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"[%d-%d]%ws",17,x,lpwzDosFullPath);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			FixSelectModuleToKernel(pSmi->smi[i].Base,lpwzDosFullPath,lpszPath);
			QueryKernelHook(m_hWnd,ID,m_list);

			x++;
		}
	}
	SetDlgItemTextW(m_hWnd,ID,L"扫描完毕...");
}