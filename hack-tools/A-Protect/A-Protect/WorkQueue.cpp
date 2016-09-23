#include "stdafx.h"
#include "WorkQueue.h"
#include "A-ProtectView.h"
#include "A-Protect.h"

VOID QueryWorkQueue(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;

	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();
	SHFILEINFO shfileinfo;
	WorkQueueImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;
	BOOL bIsFile = FALSE;

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描工作队列线程，请稍后...");

	if (WorkQueueThread){
		VirtualFree(WorkQueueThread,sizeof(WORKQUEUE)*800,MEM_RESERVE | MEM_COMMIT);
		WorkQueueThread = NULL;
	}
	WorkQueueThread = (PWORKQUEUE)VirtualAlloc(0, sizeof(WORKQUEUE)*800,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (WorkQueueThread)
	{
		memset(WorkQueueThread,0,sizeof(WORKQUEUE)*800);

		ReadFile((HANDLE)LIST_WORKQUEUE,WorkQueueThread, sizeof(WORKQUEUE)*800,&dwReadByte,0);

		for ( i=0;i< (int)WorkQueueThread->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",WorkQueueThread->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
			
			WCHAR lpwzEthread[20] = {0};
			WCHAR lpwzBasePriority[50] = {0};
			WCHAR lpwzWorkerRoutine[50] = {0};
			WCHAR lpwzHookModuleImage[260] = {0};
			WCHAR lpwzSuspendCount[260] = {0};

			memset(lpwzEthread,0,sizeof(lpwzEthread));
			memset(lpwzBasePriority,0,sizeof(lpwzBasePriority));
			memset(lpwzWorkerRoutine,0,sizeof(lpwzWorkerRoutine));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzSuspendCount,0,sizeof(lpwzSuspendCount));

			wsprintfW(lpwzEthread,L"0x%08X",WorkQueueThread->WorkQueueInfo[i].ulEthread);
			wsprintfW(lpwzSuspendCount,L"%d",WorkQueueThread->WorkQueueInfo[i].SuspendCount);

			if (WorkQueueThread->WorkQueueInfo[i].ulWorkerRoutine)
			{
				wsprintfW(lpwzWorkerRoutine,L"0x%08X",WorkQueueThread->WorkQueueInfo[i].ulWorkerRoutine);
			}else
				wsprintfW(lpwzWorkerRoutine,L"%ws",L"-");

			switch ((int)WorkQueueThread->WorkQueueInfo[i].ulBasePriority)
			{
			case 12:
				wcscat(lpwzBasePriority,L"DelayedWorkQueue");
				break;
			case 13:
				wcscat(lpwzBasePriority,L"CriticalWorkQueue");
				break;
			case 15:
				wcscat(lpwzBasePriority,L"HyperCriticalWorkQueue");
				break;
			}
			MultiByteToWideChar(
				CP_ACP,
				0, 
				WorkQueueThread->WorkQueueInfo[i].lpszModule,
				-1, 
				lpwzHookModuleImage, 
				strlen(WorkQueueThread->WorkQueueInfo[i].lpszModule)
				);
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
			if (wcsicmp(lpwzHookModuleImage,L"-") == 0){
					wcscat(lpwzDosFullPath,L"-");
					goto Next;
			}
			if (WorkQueueThread->WorkQueueInfo[i].ulWorkerRoutine){
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
				wcscat(lpwzDosFullPath,lpwzHookModuleImage);
				goto Next;
			}
Next:

			if (GetFileAttributesW(lpwzDosFullPath) == -1)
			{
				bIsFile = FALSE;
			}
			m_list->InsertItem(i,lpwzEthread,RGB(77,77,77));

			m_list->SetItemText(i,1,lpwzBasePriority);
			m_list->SetItemText(i,2,lpwzWorkerRoutine);
			m_list->SetItemText(i,3,lpwzSuspendCount);
			m_list->SetItemText(i,4,lpwzDosFullPath);

			if(bIsFile)
			{
				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
				WorkQueueImg.Add(shfileinfo.hIcon);
			}else
			{
				WorkQueueImg.Add(imgApp->LoadIconW(IDI_WHITE));
			}
			m_list->SetImageList(&WorkQueueImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
			bIsFile= TRUE;

		}
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"工作队列线程扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
void KillWorkQueue(HWND hWnd,CMyList *m_list,HANDLE ulCommand)
{
	CString PidNum;
	DWORD dwReadByte;

	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	PidNum.Format(L"%s",m_list->GetItemText(Item,0));

	WCHAR lpwzNum[50];
	char lpszNum[50];
	WCHAR lpwzEthread[50];

	memset(lpszNum,0,sizeof(lpszNum));
	memset(lpwzNum,0,sizeof(lpwzNum));
	wcscat_s(lpwzNum,PidNum);

	if (!wcslen(lpwzNum))
	{
		return;
	}
	if ((ULONG)ulCommand == KILL_SYSTEM_THREAD)
	{
		if (MessageBoxW(hWnd,L"结束工作队列线程有可能使系统崩溃，是否确定？\r\n",L"A盾电脑防护",MB_YESNO | MB_ICONWARNING) == IDNO)
		{
			return;
		}
	}
	WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum)*2,NULL,FALSE);
	for (int i=0;i<WorkQueueThread->ulCount;i++)
	{
		memset(lpwzEthread,0,sizeof(lpwzEthread));
		wsprintfW(lpwzEthread,L"0x%08X",WorkQueueThread->WorkQueueInfo[i].ulEthread);

		if (_wcsnicmp(lpwzEthread,lpwzNum,wcslen(lpwzNum)) == 0)
		{
			ReadFile(ulCommand,0,WorkQueueThread->WorkQueueInfo[i].ulEthread,&dwReadByte,0);
			break;
		}
	}
}
void KillWorkThread(HWND hWnd,ULONG ID,CMyList *m_list)
{
	//结束掉，然后再清空Items，不然获取不到listview数据
	KillWorkQueue(hWnd,m_list,(HANDLE)KILL_SYSTEM_THREAD);
	m_list->DeleteAllItems();
	QueryWorkQueue(hWnd,ID,m_list);
}
//暂停线程
void SuspendWorkQueueThread(HWND hWnd,ULONG ID,CMyList *m_list)
{
	KillWorkQueue(hWnd,m_list,(HANDLE)SUSPEND_THREAD);
	m_list->DeleteAllItems();
	QueryWorkQueue(hWnd,ID,m_list);
}
//恢复线程运行
void ResumeWorkQueueThread(HWND hWnd,ULONG ID,CMyList *m_list)
{
	KillWorkQueue(hWnd,m_list,(HANDLE)RESUME_THREAD);
	m_list->DeleteAllItems();
	QueryWorkQueue(hWnd,ID,m_list);
}