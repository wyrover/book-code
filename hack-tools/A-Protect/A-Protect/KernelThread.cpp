// KernelThread.cpp : 实现文件
//

#include "stdafx.h"
#include "A-ProtectView.h"
#include "KernelThread.h"

void QueryKernelThread(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i = 0;

	SHFILEINFO shfileinfo;
	KernelThreadImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描内核线程，请稍后...");

	if (KernelThread)
	{
		VirtualFree(KernelThread,sizeof(KERNEL_THREAD_INFO)*1025,MEM_RESERVE | MEM_COMMIT);
		KernelThread = NULL;
	}
	KernelThread = (PKERNEL_THREAD_INFO)VirtualAlloc(0, sizeof(KERNEL_THREAD_INFO)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (KernelThread)
	{
		memset(KernelThread,0,sizeof(KERNEL_THREAD_INFO)*1025);

		ReadFile((HANDLE)LIST_KERNEL_THREAD,KernelThread, sizeof(KERNEL_THREAD_INFO)*1025,&dwReadByte,0);

		for (i=0;i<KernelThread->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",KernelThread->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzHideType[256];           //0 正常模块，在driver_object中有   1 隐藏模块
			WCHAR lpwzStatus[256];             //线程状态，0运行，1退出
			WCHAR lpwzKernelThread[256];         //ETHREAD
			WCHAR lpwzThreadStart[256];          //开始
			WCHAR lpwzThreadModule[256];

			memset(lpwzHideType,0,sizeof(lpwzHideType));
			memset(lpwzStatus,0,sizeof(lpwzStatus));
			memset(lpwzKernelThread,0,sizeof(lpwzKernelThread));
			memset(lpwzThreadStart,0,sizeof(lpwzThreadStart));
			memset(lpwzThreadModule,0,sizeof(lpwzThreadModule));

			wsprintfW(lpwzKernelThread,L"0x%08X",KernelThread->KernelThreadInfo[i].KernelThread);
			wsprintfW(lpwzThreadStart,L"0x%08X",KernelThread->KernelThreadInfo[i].ThreadStart);
			
			MultiByteToWideChar(
				CP_ACP,
				0,
				KernelThread->KernelThreadInfo[i].lpszThreadModule,
				strlen(KernelThread->KernelThreadInfo[i].lpszThreadModule),
				lpwzThreadModule,
				sizeof(lpwzThreadModule)
				);

			WCHAR lpwzDosFullPath[256];
			WCHAR lpwzWinDir[256];
			WCHAR lpwzSysDisk[256];

			memset(lpwzWinDir,0,sizeof(lpwzWinDir));
			memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));

			GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
			memcpy(lpwzSysDisk,lpwzWinDir,4);

			if (wcsstr(lpwzThreadModule,L"\\??\\"))
			{
				//开始这种路径的处理
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcsncpy(lpwzDosFullPath,lpwzThreadModule+wcslen(L"\\??\\"),wcslen(lpwzThreadModule)-wcslen(L"\\??\\"));
				goto Next;
			}
			if (wcsstr(lpwzThreadModule,L"\\WINDOWS\\system32\\"))
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,lpwzThreadModule);
				//MessageBoxW(lpwzDosFullPath,lpwzFullSysName,0);
				goto Next;
			}
			if (wcsstr(lpwzThreadModule,L"\\SystemRoot\\"))
			{
				WCHAR lpwzTemp[256];
				memset(lpwzTemp,0,sizeof(lpwzTemp));
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzTemp,lpwzSysDisk);
				wcscat(lpwzTemp,L"\\WINDOWS\\");
				wcscat(lpwzDosFullPath,lpwzTemp);
				wcsncpy(lpwzDosFullPath+wcslen(lpwzTemp),lpwzThreadModule+wcslen(L"\\SystemRoot\\"),wcslen(lpwzThreadModule) - wcslen(L"\\SystemRoot\\"));
				goto Next;
			}
			//if (wcslen(lpwzHookModuleImage) == wcslen(lpwzHookModuleImage))
			//{
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzDosFullPath,lpwzSysDisk);
			wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
			wcscat(lpwzDosFullPath,lpwzThreadModule);
			goto Next;
			//}
Next:
			if (KernelThread->KernelThreadInfo[i].ulStatus == 1)
				wsprintfW(lpwzStatus,L"%ws",L"终止");
			else
				wsprintfW(lpwzStatus,L"%ws",L"运行");

			if (KernelThread->KernelThreadInfo[i].ulHideType == 1)
			{
				wsprintfW(lpwzHideType,L"%ws",L"隐藏");
				m_list->InsertItem(i,lpwzKernelThread,RGB(255,20,147));
			}
			else
			{
				wsprintfW(lpwzHideType,L"%ws",L"正常");
				m_list->InsertItem(i,lpwzKernelThread,RGB(77,77,77));
			}
			m_list->SetItemText(i,1,lpwzThreadStart);
			m_list->SetItemText(i,2,lpwzDosFullPath);
			m_list->SetItemText(i,3,lpwzStatus);
			m_list->SetItemText(i,4,lpwzHideType);

			hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
			KernelThreadImg.Add(shfileinfo.hIcon);
			m_list->SetImageList(&KernelThreadImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
		}
		VirtualFree(KernelThread,sizeof(KERNEL_THREAD_INFO)*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"内核线程扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
void ClearKernelThreadLog(CMyList *m_list)
{
	DWORD dwReadByte;

	m_list->DeleteAllItems();
	ReadFile((HANDLE)CLEAR_KERNEL_THREAD,0,0,&dwReadByte,0);
}