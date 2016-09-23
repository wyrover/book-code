#include "stdafx.h"
#include "A-ProtectView.h"
#include "Nsiproxy.h"

void QueryNsipHook(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int ItemNum = m_list->GetItemCount();
	int i=0;

	if (!IsWindows7())
	{
		SetDlgItemTextW(m_hWnd,ID,L"不支持当前系统Nsiproxy的枚举...");
		return;
	}
	SHFILEINFO shfileinfo;
	NsiproxyImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描Nsiproxy/Dispatch，请稍后...");

	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---NSIPROXY.SYS---]\r\n",PhysicalFile);
	}
	if (NsiproxyDispatchBakUp)
	{
		VirtualFree(NsiproxyDispatchBakUp,sizeof(NSIPROXYDISPATCHBAKUP)*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT);
		NsiproxyDispatchBakUp = 0;
	}
	NsiproxyDispatchBakUp = (PNSIPROXYDISPATCHBAKUP)VirtualAlloc(0,sizeof(NSIPROXYDISPATCHBAKUP)*IRP_MJ_MAXIMUM_FUNCTION*2,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (NsiproxyDispatchBakUp)
	{
		memset(NsiproxyDispatchBakUp,0,sizeof(NSIPROXYDISPATCHBAKUP)*IRP_MJ_MAXIMUM_FUNCTION*2);

		ReadFile((HANDLE)LIST_NSIPROXY_HOOK,NsiproxyDispatchBakUp,sizeof(NSIPROXYDISPATCHBAKUP)*IRP_MJ_MAXIMUM_FUNCTION*2,&dwReadByte,0);

		for (i=0;i< (int)NsiproxyDispatchBakUp->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",NsiproxyDispatchBakUp->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzCurrentNtfsDispatch[256] = {0};
			WCHAR lpwsNtfsDispatch[256] = {0};

			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzCurrentNtfsDispatch,0,sizeof(lpwzCurrentNtfsDispatch));
			memset(lpwsNtfsDispatch,0,sizeof(lpwsNtfsDispatch));

			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			switch (NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"nsiprroxy hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"nsiprroxy Inline hook");
				break;
			}

			wsprintfW(lpwzNumber,L"%d",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNumber);

			//wsprintfW(lpwzHookModuleImage,L"%ws",NtfsDispatchBakUp->NtfsDispatch[i].lpszBaseModule);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule,
				-1, 
				lpwzHookModuleImage, 
				strlen(NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpszBaseModule)
				);
			wsprintfW(lpwzFunction,L"%ws",NsiproxyDispatchBakUp->NsiproxyDispatch[i].lpwzNsiproxyDispatchName);
			wsprintfW(lpwzCurrentNtfsDispatch,L"0x%08X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulCurrentNsiproxyDispatch);
			wsprintfW(lpwsNtfsDispatch,L"0x%08X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNsiproxyDispatch);
			wsprintfW(lpwzHookModuleBase,L"0x%X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulModuleBase);
			wsprintfW(lpwzHookModuleSize,L"0x%X",NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulModuleSize);

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
			//if (wcslen(lpwzHookModuleImage) == wcslen(lpwzHookModuleImage))
			//{
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzDosFullPath,lpwzSysDisk);
			wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
			wcscat(lpwzDosFullPath,lpwzHookModuleImage);
			goto Next;
			//}
Next:
			//这里是一键体检的数据，不需要插入界面了
			if (bIsPhysicalCheck){
				//如果没有hook，就返回
				if (NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked == 0){
					continue;
				}
				WCHAR lpwzSaveBuffer[1024] ={0};
				CHAR lpszSaveBuffer[2024] ={0};
				memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
				memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

				wsprintfW(lpwzSaveBuffer,L"          --> 发现Hook:ID:%ws | 当前地址:%ws | 原始地址:%ws | 函数名:%ws | 内核模块:%ws | Hook类型:%ws\r\n",
					lpwzNumber,lpwzCurrentNtfsDispatch,lpwsNtfsDispatch,lpwzFunction,lpwzDosFullPath,lpwzHookType);

				m_list->InsertItem(0,L"NSIPROXY.SYS",RGB(77,77,77));
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
			if (NsiproxyDispatchBakUp->NsiproxyDispatch[i].Hooked == 0)
			{
				m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(i,lpwzNumber,RGB(255,20,147));
			}
			m_list->SetItemText(i,1,lpwzFunction);
			m_list->SetItemText(i,2,lpwzCurrentNtfsDispatch);
			m_list->SetItemText(i,3,lpwsNtfsDispatch);
			m_list->SetItemText(i,4,lpwzDosFullPath);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
			m_list->SetItemText(i,7,lpwzHookType);

			hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
			NsiproxyImg.Add(shfileinfo.hIcon);
			m_list->SetImageList(&NsiproxyImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"Nsiproxy/Dispatch 扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
void NsipHookResetOne(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	CString str;
	CString Num;
	CString Address;

	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	str.Format(L"%s",m_list->GetItemText(Item,7));

	if (!wcslen(str))
	{
		return;
	}
	if (wcsstr(str,L"-") != NULL)
	{
		AfxMessageBox(_T("该函数不需要恢复！"));
		return;
	}
	WCHAR lpwzNum[10];
	char lpszNum[10];

	if (wcsstr(str,L"nsiprroxy hook") != NULL)
	{
		//取序号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);
		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);
		ReadFile((HANDLE)INIT_SET_NSIPROXY_HOOK,0,atoi(lpszNum),&dwReadByte,0);

		for (int i = 0;i<NsiproxyDispatchBakUp->ulCount;i++)
		{
			if (atoi(lpszNum) == NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNumber)
			{
				ReadFile((HANDLE)SET_NSIPROXY_HOOK,0,NsiproxyDispatchBakUp->NsiproxyDispatch[i].ulNsiproxyDispatch,&dwReadByte,0);
				break;
			}
		}

		//删除当前选择的一行
		m_list->DeleteAllItems();
		QueryNsipHook(m_hWnd,ID,m_list);
	}
}
void CopyNsipDataToClipboard(HWND hWnd,CMyList *m_list)
{
	CString FunctionName;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	FunctionName.Format(L"%s",m_list->GetItemText(Item,1));

	WCHAR lpwzFunctionName[260];

	memset(lpwzFunctionName,0,sizeof(lpwzFunctionName));
	wcscat_s(lpwzFunctionName,FunctionName);
	CHAR lpszFunctionName[1024];
	char *lpString = NULL;

	memset(lpwzFunctionName,0,sizeof(lpwzFunctionName));
	memset(lpszFunctionName,0,sizeof(lpszFunctionName));
	wcscat_s(lpwzFunctionName,FunctionName);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzFunctionName,
		-1,
		lpszFunctionName,
		wcslen(lpwzFunctionName)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszFunctionName);
	if (lpString)
	{
		MessageBoxW(hWnd,L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}