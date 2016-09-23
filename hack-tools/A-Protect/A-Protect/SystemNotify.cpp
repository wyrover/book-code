#include "stdafx.h"
#include "A-Protectview.h"
#include "SystemNotify.h"

void QuerySystemNotify(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;
	int ItemNum = m_list->GetItemCount();

	SHFILEINFO shfileinfo;
	SystemNotiImg.Create(16,16, ILC_COLOR32, 2, 80);
	HIMAGELIST hImageList = NULL;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描系统回调，请稍后...");

	if (SystemNotify)
	{
		VirtualFree(SystemNotify,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1025,MEM_RESERVE | MEM_COMMIT);
		SystemNotify = 0;
	}
	SystemNotify = (PSYSTEM_NOTIFY)VirtualAlloc(0,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (SystemNotify)
	{
		memset(SystemNotify,0,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1025);

		ReadFile((HANDLE)LIST_SYSTEM_NOTIFY,SystemNotify,(sizeof(SYSTEM_NOTIFY)+sizeof(SYSTEM_NOTIFY_INFORMATION))*1025,&dwReadByte,0);

		for (i=0;i< (int)SystemNotify->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",SystemNotify->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNotifyBase[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzObject[256] = {0};
			WCHAR lpwzFullSysName[256] = {0};

			memset(lpwzNotifyBase,0,sizeof(lpwzNotifyBase));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzObject,0,sizeof(lpwzObject));
			memset(lpwzFullSysName,0,sizeof(lpwzFullSysName));

			wsprintfW(lpwzNotifyBase,L"0x%08X",SystemNotify->NotifyInfo[i].ulNotifyBase);

			//wsprintfW(lpwzHookModuleImage,L"%ws",SystemNotify->KbdclassDispatch[i].lpszBaseModule);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				SystemNotify->NotifyInfo[i].lpszModule,
				-1, 
				lpwzFullSysName, 
				strlen(SystemNotify->NotifyInfo[i].lpszModule)
				);
//********************************************************************
			WCHAR lpwzDosFullPath[256];
			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[256];

			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));

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
				//MessageBoxW(lpwzDosFullPath,lpwzFullSysName,0);
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
			if (!wcsstr(lpwzFullSysName,L"\\"))
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
				wcscat(lpwzDosFullPath,lpwzFullSysName);
				goto Next;
			}
Next:

//********************************************************************
			wsprintfW(lpwzHookType,L"%ws",SystemNotify->NotifyInfo[i].lpwzType);
			wsprintfW(lpwzObject,L"%ws",SystemNotify->NotifyInfo[i].lpwzObject);

			m_list->InsertItem(i,lpwzNotifyBase,RGB(77,77,77));
			m_list->SetItemText(i,1,lpwzHookType);
			m_list->SetItemText(i,2,lpwzObject);
			m_list->SetItemText(i,3,lpwzDosFullPath);

			hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
			SystemNotiImg.Add(shfileinfo.hIcon);
			m_list->SetImageList(&SystemNotiImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"系统回调扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
void KillSystemNotify(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	CString str;

	CString FunctionStr;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	str.Format(L"%s",m_list->GetItemText(Item,0));

	if (!wcslen(str))
	{
		return;
	}
	WCHAR lpwzAdd[50];

	for (int i = 0;i< (int)SystemNotify->ulCount;i++)
	{
		memset(lpwzAdd,0,sizeof(lpwzAdd));
		wsprintfW(lpwzAdd,L"0x%08X",SystemNotify->NotifyInfo[i].ulNotifyBase);

		if (_wcsicmp(lpwzAdd,str) == 0)
		{
			FunctionStr.Format(L"%s",m_list->GetItemText(Item,1));

			if (_wcsicmp(FunctionStr,L"CreateProcess") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,1,&dwReadByte,0);
			}
			else if (_wcsicmp(FunctionStr,L"CreateThread") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,2,&dwReadByte,0);
			}
			else if (_wcsicmp(FunctionStr,L"LoadImage") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,3,&dwReadByte,0);
			}
			else if (_wcsicmp(FunctionStr,L"BugCheckCallBack") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,4,&dwReadByte,0);
				ReadFile((HANDLE)KILL_SYSTEM_NOTIFY,0,SystemNotify->NotifyInfo[i].ulObject,&dwReadByte,0);
				break;
			}
			else if (_wcsicmp(FunctionStr,L"BugCheckReasonCallBack") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,5,&dwReadByte,0);
				ReadFile((HANDLE)KILL_SYSTEM_NOTIFY,0,SystemNotify->NotifyInfo[i].ulObject,&dwReadByte,0);
				break;
			}
			else if (_wcsicmp(FunctionStr,L"ShutDown") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,6,&dwReadByte,0);
				ReadFile((HANDLE)KILL_SYSTEM_NOTIFY,0,SystemNotify->NotifyInfo[i].ulObject,&dwReadByte,0);
				break;
			}
			else if (_wcsicmp(FunctionStr,L"SeFileSystem") == 0)
			{
				ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,7,&dwReadByte,0);
			}
			//未实现
			else if (_wcsicmp(FunctionStr,L"FsNotifyChange") == 0)
			{
				//ReadFile((HANDLE)INIT_KILL_SYSTEM_NOTIFY,0,8,&dwReadByte,0);
				MessageBoxW(m_hWnd,L"暂不支持FsNotifyChange类型回调的删除",L"A盾电脑防护",MB_OK);
				break;
			}
			ReadFile((HANDLE)KILL_SYSTEM_NOTIFY,0,SystemNotify->NotifyInfo[i].ulNotifyBase,&dwReadByte,0);
			break;
		}
	}
	m_list->DeleteAllItems();
	QuerySystemNotify(m_hWnd,ID,m_list);
}
void CopySystemNotifyDataToClipboard(HWND m_hWnd,CMyList *m_list)
{
	CString FunctionName;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	FunctionName.Format(L"%s",m_list->GetItemText(Item,3));

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
		MessageBoxW(m_hWnd,L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}