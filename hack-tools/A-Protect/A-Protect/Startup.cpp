#include "stdafx.h"
#include "Startup.h"
#include "A-ProtectView.h"
#include "A-Protect.h"

VOID QuerySystemStartup(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;
// 
// 	SHFILEINFO shfileinfo;
// 	StartupImg.Create(16,16, ILC_COLOR32, 2, 100);
// 	HIMAGELIST hImageList = NULL;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描启动项，请稍后...");
	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---启动项---]\r\n",PhysicalFile);
	}

	StartupInfo = (PSTARTUP_INFO)VirtualAlloc(0, sizeof(STARTUP_INFO)*800,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (StartupInfo)
	{
		memset(StartupInfo,0,sizeof(STARTUP_INFO)*800);

		ReadFile((HANDLE)LIST_START_UP,StartupInfo, sizeof(STARTUP_INFO)*800,&dwReadByte,0);

		for ( i=0;i< (int)StartupInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",StartupInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
			
			WCHAR lpwzNumber[20] = {0};
			memset(lpwzNumber,0,sizeof(lpwzNumber));
			wsprintfW(lpwzNumber,L"%d",i);

			//这里是一键体检的数据，不需要插入界面了
			if (bIsPhysicalCheck){
				WCHAR lpwzSaveBuffer[1024] ={0};
				CHAR lpszSaveBuffer[2024] ={0};
				memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
				memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

				wsprintfW(lpwzSaveBuffer,L"          --> 启动项（可能是安全的）:启动名称:%ws | 注册表路径:%ws | 注册表键值:%ws\r\n",
					StartupInfo->Startup[i].lpwzName,StartupInfo->Startup[i].lpwzKeyPath,StartupInfo->Startup[i].lpwzKeyValue);

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
			m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));
			m_list->SetItemText(i,1,StartupInfo->Startup[i].lpwzName);
			m_list->SetItemText(i,2,StartupInfo->Startup[i].lpwzKeyPath);
			m_list->SetItemText(i,3,StartupInfo->Startup[i].lpwzKeyValue);

// 			WCHAR lpwzRegedit[20] = {0};
// 			memset(lpwzRegedit,0,sizeof(lpwzRegedit));
// 			GetWindowsDirectory(lpwzRegedit, sizeof(lpwzRegedit));
// 			wcscat(lpwzRegedit,L"\\regedit.exe");
// 
// 			hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzRegedit,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
// 			StartupImg.Add(shfileinfo.hIcon);
// 			m_list->SetImageList(&StartupImg);
// 			m_list->SetItemImageId(i,i);
// 			DestroyIcon(shfileinfo.hIcon);
		}
		VirtualFree(StartupInfo,sizeof(STARTUP_INFO)*800,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"启动项扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}