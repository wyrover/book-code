#include "stdafx.h"
#include "MessageHook.h"
#include "A-ProtectView.h"
#include "A-Protect.h"

VOID QueryMessageHook(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;
	
	SHFILEINFO shfileinfo;
	MessageHookImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;
	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描MessageHook，请稍后...");

	MessageHookInfo = (PMESSAGE_HOOK_INFO)VirtualAlloc(0, (sizeof(MESSAGE_HOOK_INFO)+sizeof(HOOK_INFO))*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (MessageHookInfo)
	{
		memset(MessageHookInfo,0,(sizeof(MESSAGE_HOOK_INFO)+sizeof(HOOK_INFO))*1025);

		ReadFile((HANDLE)LIST_MESSAGE_HOOK,MessageHookInfo, (sizeof(MESSAGE_HOOK_INFO)+sizeof(HOOK_INFO))*1025,&dwReadByte,0);

		for ( i=0;i< (int)MessageHookInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",MessageHookInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);


			WCHAR lpwzHandle[256] = {0};
			WCHAR lpwzFuncOffset[256] = {0};
			WCHAR lpwzFuncBaseAddr[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzPID[256] = {0};
			WCHAR lpwzTID[256] = {0};
			WCHAR lpwzProName[256] = {0};

			memset(lpwzHandle,0,sizeof(lpwzHandle));
			memset(lpwzFuncOffset,0,sizeof(lpwzFuncOffset));
			memset(lpwzFuncBaseAddr,0,sizeof(lpwzFuncBaseAddr));
			memset(lpwzPID,0,sizeof(lpwzPID));
			memset(lpwzTID,0,sizeof(lpwzTID));
			memset(lpwzProName,0,sizeof(lpwzProName));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			wsprintfW(lpwzHandle,L"%X",MessageHookInfo->MessageHook[i].Handle);

			if (MessageHookInfo->MessageHook[i].FuncBaseAddr != 0)
			
				wsprintfW(lpwzFuncBaseAddr,L"0x%08X",MessageHookInfo->MessageHook[i].FuncBaseAddr);
			else
				wsprintfW(lpwzFuncBaseAddr,L"%ws",L"-");

			MultiByteToWideChar(
				CP_ACP,
				0, 
				MessageHookInfo->MessageHook[i].ProcessName,
				-1, 
				lpwzProName, 
				strlen(MessageHookInfo->MessageHook[i].ProcessName)
				);
// 			MultiByteToWideChar(
// 				CP_ACP,
// 				0, 
// 				MessageHookInfo->MessageHook[i].lpszHookModuleImage,
// 				-1, 
// 				lpwzHookModuleImage, 
// 				strlen(MessageHookInfo->MessageHook[i].lpszHookModuleImage)
// 				);

			wsprintfW(lpwzFuncOffset,L"0x%08X",MessageHookInfo->MessageHook[i].FuncOffset);

// 			switch (MessageHookInfo->MessageHook[i].ulHookType)
// 			{
// 			case 0:
// 				wcscat_s(lpwzHookType,L"-");
// 				if (bIsPhysicalCheck){
// 					goto _InsertData;
// 				}
// 				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(77,77,77));
// 				break;
// 			case 1:
// 				wcscat_s(lpwzHookType,L"MessageHook");
// 				if (bIsPhysicalCheck){
// 					goto _InsertData;
// 				}
// 				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(255,20,147));
// 				break;
// 			case 2:
// 				wcscat_s(lpwzHookType,L"ObjectInlineHook");
// 				if (bIsPhysicalCheck){
// 					goto _InsertData;
// 				}
// 				m_list->InsertItem(i,lpwzMemoryHookBase,RGB(255,20,147));
// 				break;
// 			}
// _InsertData:
// 			WCHAR lpwzDosFullPath[256];
// 			WCHAR lpwzWinDir[256];
// 			WCHAR lpwzSysDisk[256];
// 
// 			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
// 			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
// 			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
// 
// 			GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
// 			memcpy(lpwzSysDisk,lpwzWinDir,4);
// 
// 			if (wcsstr(lpwzHookModuleImage,L"\\??\\"))
// 			{
// 				//开始这种路径的处理
// 				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
// 				wcsncpy(lpwzDosFullPath,lpwzHookModuleImage+wcslen(L"\\??\\"),wcslen(lpwzHookModuleImage)-wcslen(L"\\??\\"));
// 				goto Next;
// 			}
// 			if (wcsstr(lpwzHookModuleImage,L"\\WINDOWS\\system32\\"))
// 			{
// 				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
// 				wcscat(lpwzDosFullPath,lpwzSysDisk);
// 				wcscat(lpwzDosFullPath,lpwzHookModuleImage);
// 				//MessageBoxW(lpwzDosFullPath,lpwzFullSysName,0);
// 				goto Next;
// 			}
// 			if (wcsstr(lpwzHookModuleImage,L"\\SystemRoot\\"))
// 			{
// 				WCHAR lpwzTemp[256];
// 				memset(lpwzTemp,0,sizeof(lpwzTemp));
// 				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
// 				wcscat(lpwzTemp,lpwzSysDisk);
// 				wcscat(lpwzTemp,L"\\WINDOWS\\");
// 				wcscat(lpwzDosFullPath,lpwzTemp);
// 				wcsncpy(lpwzDosFullPath+wcslen(lpwzTemp),lpwzHookModuleImage+wcslen(L"\\SystemRoot\\"),wcslen(lpwzHookModuleImage) - wcslen(L"\\SystemRoot\\"));
// 				goto Next;
// 			}
// 			//if (wcslen(lpwzHookModuleImage) == wcslen(lpwzHookModuleImage))
// 			//{
// 			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
// 			wcscat(lpwzDosFullPath,lpwzSysDisk);
// 			wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
// 			wcscat(lpwzDosFullPath,lpwzHookModuleImage);
// 			goto Next;
// 			//}
// Next:
// 			m_list->SetItemText(i,1,lpwzMemoryFunctionBase);
// 			m_list->SetItemText(i,2,lpwzFunction);
// 			m_list->SetItemText(i,3,lpwzObjectTypeName);
// 			m_list->SetItemText(i,4,lpwzObjectTypeBase);
// 			m_list->SetItemText(i,5,lpwzDosFullPath);
// 			m_list->SetItemText(i,6,lpwzHookType);

// 			if(GetFileAttributes(lpwzDosFullPath)!=INVALID_FILE_ATTRIBUTES)
// 			{
// 				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
// 				MessageHookImg.Add(shfileinfo.hIcon);
// 			}else
// 				MessageHookImg.Add(imgApp->LoadIconW(IDI_WHITE));
// 
// 			m_list->SetImageList(&MessageHookImg);
// 			m_list->SetItemImageId(i,i);
// 			DestroyIcon(shfileinfo.hIcon);
		}
		VirtualFree(MessageHookInfo,(sizeof(MESSAGE_HOOK_INFO)+sizeof(HOOK_INFO))*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"MessageHook扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}