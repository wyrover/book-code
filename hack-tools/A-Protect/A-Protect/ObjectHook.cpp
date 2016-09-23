#include "stdafx.h"
#include "ObjectHook.h"
#include "A-ProtectView.h"
#include "A-Protect.h"

VOID QueryObjectHook(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;
	
	SHFILEINFO shfileinfo;
	ObjectHookImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;
	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描ObjectHook，请稍后...");

	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---ObjectHook---]\r\n",PhysicalFile);
	}
	ObjectHookInfo = (POBJECTHOOKINFO)VirtualAlloc(0, sizeof(OBJECTHOOKINFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (ObjectHookInfo)
	{
		memset(ObjectHookInfo,0,sizeof(OBJECTHOOKINFO)*1025);

		ReadFile((HANDLE)LIST_OBJECT_HOOK,ObjectHookInfo, sizeof(OBJECTHOOKINFO)*1025,&dwReadByte,0);

		for ( i=0;i< (int)ObjectHookInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",ObjectHookInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);


			WCHAR lpwzMemoryFunctionBase[256] = {0};
			WCHAR lpwzMemoryHookBase[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzObjectTypeBase[256] = {0};
			WCHAR lpwzObjectTypeName[256] = {0};
			WCHAR lpwzHookType[256] = {0};

			memset(lpwzMemoryFunctionBase,0,sizeof(lpwzMemoryFunctionBase));
			memset(lpwzMemoryHookBase,0,sizeof(lpwzMemoryHookBase));
			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzObjectTypeBase,0,sizeof(lpwzObjectTypeBase));
			memset(lpwzObjectTypeName,0,sizeof(lpwzObjectTypeName));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			wsprintfW(lpwzMemoryHookBase,L"0x%08X",ObjectHookInfo->ObjectHook[i].ulMemoryHookBase);

			if (ObjectHookInfo->ObjectHook[i].ulMemoryFunctionBase != 0)
			
				wsprintfW(lpwzMemoryFunctionBase,L"0x%08X",ObjectHookInfo->ObjectHook[i].ulMemoryFunctionBase);
			else
				wsprintfW(lpwzMemoryFunctionBase,L"%ws",L"-");

			MultiByteToWideChar(
				CP_ACP,
				0, 
				ObjectHookInfo->ObjectHook[i].lpszFunction,
				-1, 
				lpwzFunction, 
				strlen(ObjectHookInfo->ObjectHook[i].lpszFunction)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				ObjectHookInfo->ObjectHook[i].lpszHookModuleImage,
				-1, 
				lpwzHookModuleImage, 
				strlen(ObjectHookInfo->ObjectHook[i].lpszHookModuleImage)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				ObjectHookInfo->ObjectHook[i].lpszObjectTypeName,
				-1, 
				lpwzObjectTypeName, 
				strlen(ObjectHookInfo->ObjectHook[i].lpszObjectTypeName)
				);
			wsprintfW(lpwzObjectTypeBase,L"0x%08X",ObjectHookInfo->ObjectHook[i].ulObjectTypeBase);

			switch (ObjectHookInfo->ObjectHook[i].ulHookType)
			{
			case 0:
				wcscat_s(lpwzHookType,L"-");
				if (bIsPhysicalCheck){
					goto _InsertData;
				}
				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(77,77,77));
				break;
			case 1:
				wcscat_s(lpwzHookType,L"ObjectHook");
				if (bIsPhysicalCheck){
					goto _InsertData;
				}
				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(255,20,147));
				break;
			case 2:
				wcscat_s(lpwzHookType,L"ObjectInlineHook");
				if (bIsPhysicalCheck){
					goto _InsertData;
				}
				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(255,20,147));
				break;
			}
_InsertData:
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
				if (ObjectHookInfo->ObjectHook[i].ulHookType == 0){
					continue;
				}
				WCHAR lpwzSaveBuffer[1024] ={0};
				CHAR lpszSaveBuffer[2024] ={0};
				memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
				memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

				wsprintfW(lpwzSaveBuffer,L"          --> 发现Hook:当前地址:%ws | 原始地址:%ws | 函数名:%ws | 内核模块:%ws | Hook类型:%ws\r\n",
					lpwzMemoryFunctionBase,lpwzFunction,lpwzObjectTypeName,lpwzDosFullPath,lpwzHookType);

				m_list->InsertItem(0,L"ObjectHook",RGB(77,77,77));
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
			m_list->SetItemText(i,1,lpwzMemoryFunctionBase);
			m_list->SetItemText(i,2,lpwzFunction);
			m_list->SetItemText(i,3,lpwzObjectTypeName);
			m_list->SetItemText(i,4,lpwzObjectTypeBase);
			m_list->SetItemText(i,5,lpwzDosFullPath);
			m_list->SetItemText(i,6,lpwzHookType);

			if(GetFileAttributes(lpwzDosFullPath)!=INVALID_FILE_ATTRIBUTES)
			{
				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
				ObjectHookImg.Add(shfileinfo.hIcon);
			}else
				ObjectHookImg.Add(imgApp->LoadIconW(IDI_WHITE));
			m_list->SetImageList(&ObjectHookImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
		}
		VirtualFree(ObjectHookInfo,sizeof(OBJECTHOOKINFO)*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"ObjectHook扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}