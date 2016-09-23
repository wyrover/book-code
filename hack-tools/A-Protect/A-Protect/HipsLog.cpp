#include "stdafx.h"
#include "A-ProtectView.h"
#include "HipsLog.h"
#include "A-Protect.h"

BOOL PrintfDosPath(__in LPCTSTR lpwzNtFullPath,__out LPCTSTR lpwzCreateProcess);

void GetSaveFileLog(WCHAR *lpwzFilePath)
{
	CString			m_path;
	WCHAR lpwzForMat[256];
	DWORD dwReadByte;

	memset(lpwzFilePath,0,sizeof(lpwzFilePath));
	memset(lpwzForMat,0,sizeof(lpwzForMat));

	//暂停不然无法因为保护着win7下无法打开对话框
	ReadFile((HANDLE)SUSPEND_PROTECT,0,0,&dwReadByte,0);

	CFileDialog dlg( FALSE,L"txt",0, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,L"所有文件|*.*");
	dlg.m_ofn.lpstrTitle= L"保存防御日志到文件";
	if ( dlg.DoModal() == IDOK )
	{
		m_path = dlg.GetPathName();
		wsprintfW(lpwzFilePath,L"%ws",m_path);

		ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);

// 		wsprintfW(lpwzForMat,L"操作成功，记录文件保存至:%ws",m_path);
// 		MessageBoxW(lpwzForMat,0,MB_ICONWARNING);
	}
	//再继续保护自己的进程
	ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);
}
VOID HipsLog(HWND m_hWnd,ULONG ID,CMyList *m_list,int Type)
{
	DWORD dwReadByte;
	int i=0;
	int ItemNum = m_list->GetItemCount();
	WCHAR lpwzFilePath[256];
	BOOL bIsSaveLogFile = FALSE;

	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();
	SHFILEINFO shfileinfo;
	HipsLogImg.Create(16,16, ILC_COLOR32, 2, 500);
	HIMAGELIST hImageList = NULL;
	bool HasFile=TRUE;

	SetDlgItemText(m_hWnd,ID,L"正在扫描防御日志，请稍后...");
	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---被动防御---]\r\n",PhysicalFile);
	}
	LogDefenseInfo = (PLOGDEFENSE)VirtualAlloc(0, sizeof(LOGDEFENSE)*1025,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (LogDefenseInfo)
	{
		ReadFile((HANDLE)LIST_LOG,LogDefenseInfo,sizeof(LOGDEFENSE)*1025,&dwReadByte,0);

		//导出日志啊
		if (Type == 1)
		{
			GetSaveFileLog(lpwzFilePath);
			bIsSaveLogFile = TRUE;
		}
		if (LogDefenseInfo->ulCount >= 1024)
		{
			if (MessageBoxW(m_hWnd,L"防御日志已经满内存，无法在记录，是否导出到文件？",L"A盾电脑防护",MB_YESNO | MB_ICONWARNING) == IDYES)
			{
				GetSaveFileLog(lpwzFilePath);
				bIsSaveLogFile = TRUE;
			}
		}
		for (i=0;i< LogDefenseInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",LogDefenseInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzType[100];
			WCHAR lpwzPID[100];
			WCHAR lpwzInherited[100];
			WCHAR lpwzProName[256];
			WCHAR lpwzEProcess[100];
			WCHAR lpwzMoreEvents[1024];

			memset(lpwzType,0,sizeof(lpwzType));
			memset(lpwzPID,0,sizeof(lpwzPID));
			memset(lpwzInherited,0,sizeof(lpwzInherited));
			memset(lpwzProName,0,sizeof(lpwzProName));
			memset(lpwzEProcess,0,sizeof(lpwzEProcess));
			memset(lpwzMoreEvents,0,sizeof(lpwzMoreEvents));

			if (LogDefenseInfo->LogDefense[i].Type == 1)
			{
				wcscat(lpwzType,L"进程结束");
				if (LogDefenseInfo->LogDefense[i].ulPID)
					wsprintfW(lpwzMoreEvents,L"进程[[%d]试图结束 A盾电脑防护，已被阻止",LogDefenseInfo->LogDefense[i].ulPID);
				else
					wcscat(lpwzMoreEvents,L"(已退出)进程[Unknown]试图结束 A盾电脑防护，已被阻止");

			}else if (LogDefenseInfo->LogDefense[i].Type == 2)
			{
				wcscat(lpwzType,L"网络连接");

				if (LogDefenseInfo->LogDefense[i].ulPID)
					wsprintfW(lpwzMoreEvents,L"进程[[%d]试图连接网络(普通)",LogDefenseInfo->LogDefense[i].ulPID);
				else
					wcscat(lpwzMoreEvents,L"(已退出)进程[Unknown]试图连接网络(普通)");
			}
			else if (LogDefenseInfo->LogDefense[i].Type == 3)
			{
				wcscat(lpwzType,L"进程创建");

				if (LogDefenseInfo->LogDefense[i].ulPID)
					wsprintfW(lpwzMoreEvents,L"进程[%d]创建进程，可疑的木马行为(如果不是自己操作的话)/(危险)",LogDefenseInfo->LogDefense[i].ulPID);
				else
					wcscat(lpwzMoreEvents,L"(已退出)进程[Unknown]创建进程，可疑的木马行为(如果不是自己操作的话)/(危险)");

			}else if (LogDefenseInfo->LogDefense[i].Type == 4)
			{
				wcscat(lpwzType,L"DLL挟持");

				if (LogDefenseInfo->LogDefense[i].ulPID)
					wsprintfW(lpwzMoreEvents,L"发现进程[%d]被Dll挟持，已拒绝！",LogDefenseInfo->LogDefense[i].ulPID);
			}
			else if (LogDefenseInfo->LogDefense[i].Type == 5)
			{
				wcscat(lpwzType,L"系统服务创建或修改");

				if (LogDefenseInfo->LogDefense[i].ulPID)
					wsprintfW(lpwzMoreEvents,L"进程[%d]试图创建/修改一个服务:%ws",LogDefenseInfo->LogDefense[i].ulPID,LogDefenseInfo->LogDefense[i].lpwzMoreEvents);
			}
			else if (LogDefenseInfo->LogDefense[i].Type == 6)
			{
				wcscat(lpwzType,L"驱动加载");

				wcscat(lpwzMoreEvents,L"一个驱动程序已被加载进系统内核，如果您不认识该驱动，则表明您系统已被rootkit攻击");
			}
			wsprintfW(lpwzPID,L"%d",LogDefenseInfo->LogDefense[i].ulPID);
			wsprintfW(lpwzInherited,L"%d",LogDefenseInfo->LogDefense[i].ulInheritedFromProcessId);

			if (strlen(LogDefenseInfo->LogDefense[i].lpszProName))
			{
				MultiByteToWideChar(CP_ACP,0,LogDefenseInfo->LogDefense[i].lpszProName,strlen(LogDefenseInfo->LogDefense[i].lpszProName),lpwzProName,sizeof(lpwzProName));
			}else
				wcscat(lpwzProName,L"Unknown");

			WCHAR lpwzCreateProcess[260];
			memset(lpwzCreateProcess,0,sizeof(lpwzCreateProcess));

			//如果是DLL挟持，则打印完整路径
			if (LogDefenseInfo->LogDefense[i].Type == 4)
			{
				PrintfDosPath(LogDefenseInfo->LogDefense[i].lpwzCreateProcess,lpwzCreateProcess);
			}
			else if (LogDefenseInfo->LogDefense[i].Type == 6)
			{
				WCHAR lpwzWinDir[256];
				WCHAR lpwzSysDisk[256];
				WCHAR lpwzHookModuleImage[256];

				memset(lpwzWinDir,0,sizeof(lpwzWinDir));
				memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
				memset(lpwzCreateProcess,0,sizeof(lpwzCreateProcess));
				memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));

				wcscat(lpwzHookModuleImage,LogDefenseInfo->LogDefense[i].lpwzCreateProcess);

				GetWindowsDirectoryW(lpwzWinDir,sizeof(lpwzWinDir));
				memcpy(lpwzSysDisk,lpwzWinDir,4);

				if (wcsstr(lpwzHookModuleImage,L"\\??\\"))
				{
					//开始这种路径的处理
					memset(lpwzCreateProcess,0,sizeof(lpwzCreateProcess));
					wcsncpy(lpwzCreateProcess,lpwzHookModuleImage+wcslen(L"\\??\\"),wcslen(lpwzHookModuleImage)-wcslen(L"\\??\\"));
					goto Next;
				}
				if (wcsstr(lpwzHookModuleImage,L"\\WINDOWS\\system32\\"))
				{
					memset(lpwzCreateProcess,0,sizeof(lpwzCreateProcess));
					wcscat(lpwzCreateProcess,lpwzSysDisk);
					wcscat(lpwzCreateProcess,lpwzHookModuleImage);
					//MessageBoxW(lpwzCreateProcess,lpwzFullSysName,0);
					goto Next;
				}
				if (wcsstr(lpwzHookModuleImage,L"\\SystemRoot\\"))
				{
					WCHAR lpwzTemp[256];
					memset(lpwzTemp,0,sizeof(lpwzTemp));
					memset(lpwzCreateProcess,0,sizeof(lpwzCreateProcess));
					wcscat(lpwzTemp,lpwzSysDisk);
					wcscat(lpwzTemp,L"\\WINDOWS\\");
					wcscat(lpwzCreateProcess,lpwzTemp);
					wcsncpy(lpwzCreateProcess+wcslen(lpwzTemp),lpwzHookModuleImage+wcslen(L"\\SystemRoot\\"),wcslen(lpwzHookModuleImage) - wcslen(L"\\SystemRoot\\"));
					goto Next;
				}
				memset(lpwzCreateProcess,0,sizeof(lpwzCreateProcess));
				wcscat(lpwzCreateProcess,lpwzSysDisk);
				wcscat(lpwzCreateProcess,L"\\WINDOWS\\system32\\drivers\\");
				wcscat(lpwzCreateProcess,lpwzHookModuleImage);
			}else{
				wcscat(lpwzCreateProcess,LogDefenseInfo->LogDefense[i].lpwzCreateProcess);
			}
Next:
			char lpszDLLPath[256] = {0};
			WCHAR lpwzMd5[256] = {0};
			WCHAR lpwzTrue[256] = {0};

			memset(lpszDLLPath,0,sizeof(lpszDLLPath));
			memset(lpwzMd5,0,sizeof(lpwzMd5));
			memset(lpwzTrue,0,sizeof(lpwzTrue));
			if (LogDefenseInfo->LogDefense[i].Type == 3 ||
				LogDefenseInfo->LogDefense[i].Type == 4 ||
				LogDefenseInfo->LogDefense[i].Type == 6)
			{
				WideCharToMultiByte( CP_ACP,
					0,
					lpwzCreateProcess,
					-1,
					lpszDLLPath,
					wcslen(lpwzCreateProcess)*2,
					NULL,
					NULL
					);
				FILE * fp=fopen(lpszDLLPath,"rb");
				if(fp)
				{
					MD5VAL val;
					val = md5File(fp);
					wsprintfW(lpwzMd5,L"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
					FileVerify(lpszDLLPath,lpwzMd5,lpwzTrue);

					fclose(fp);
				}
			}
			if (LogDefenseInfo->LogDefense[i].EProcess)
			{
				if (LogDefenseInfo->LogDefense[i].Type == 6)
					wsprintfW(lpwzEProcess,L"驱动基址:0x%08X",LogDefenseInfo->LogDefense[i].EProcess);
				else
					wsprintfW(lpwzEProcess,L"0x%08X",LogDefenseInfo->LogDefense[i].EProcess);

			}
			else{

				if (LogDefenseInfo->LogDefense[i].Type == 6)
					wcscat(lpwzEProcess,L"驱动已卸载");
				else
					wcscat(lpwzEProcess,L"-");
			}

			WCHAR lpwzLog[1024];
			CHAR lpszLog[2024];
			//保存文件
			if (bIsSaveLogFile)
			{
				memset(lpwzLog,0,sizeof(lpwzLog));
				memset(lpszLog,0,sizeof(lpszLog));
				wsprintfW(lpwzLog,L"进程ID:%ws\r\n父进程ID:%ws\r\n映像路径:%ws\r\nEPROCESS:%ws\r\n事件:%ws\r\n事件详细内容:%ws\r\n被创建进程:%ws\r\nMD5/数字签名:%ws\r\n\r\n",
								   lpwzPID,
								   lpwzInherited,
								   lpwzProName,
								   lpwzEProcess,
								   lpwzType,
								   lpwzMoreEvents,
								   lpwzCreateProcess,
								   lpwzTrue);

				WideCharToMultiByte( CP_ACP,
					0,
					lpwzLog,
					-1,
					lpszLog,
					wcslen(lpwzLog)*2,
					NULL,
					NULL
					);
				SaveToFile(lpszLog,lpwzFilePath);
			}
			if (!wcslen(lpwzProName))
			{
				wcscat_s(lpwzProName,L"-");
			}
			if (!wcslen(lpwzMoreEvents))
			{
				wcscat_s(lpwzMoreEvents,L"-");
			}
			if (!wcslen(lpwzCreateProcess))
			{
				wcscat_s(lpwzCreateProcess,L"-");
				HasFile=FALSE;
			}
			//这里是一键体检的数据，不需要插入界面了
			if (bIsPhysicalCheck){
				//如果没有hook，就返回
				if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0 ||
					_wcsnicmp(lpwzTrue,L"文件不存在，无法验证",wcslen(L"文件不存在，无法验证")) == 0 ||
					LogDefenseInfo->LogDefense[i].Type == 5)    //创建修改服务，要突显
				{
					WCHAR lpwzSaveBuffer[1024] ={0};
					CHAR lpszSaveBuffer[2024] ={0};
					memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
					memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

					wsprintfW(lpwzSaveBuffer,L"          --> 被动防御:进程PID:%ws | 进程名:%ws | EPROCESS:%ws | 事件:%ws | 内容:%ws\r\n",
						lpwzPID,lpwzProName,lpwzEProcess,lpwzMoreEvents,lpwzCreateProcess);

					m_list->InsertItem(0,L"被动防御",RGB(77,77,77));
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
				}
				continue;
			}
			if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0)
			{
				m_list->InsertItem(i,lpwzPID,RGB(255,20,147));

			}else
			{
				if (LogDefenseInfo->LogDefense[i].Type == 5 ||
					LogDefenseInfo->LogDefense[i].Type == 4 )  //特别关注的要突显
					m_list->InsertItem(i,lpwzPID,RGB(255,20,147));
				else
					m_list->InsertItem(i,lpwzPID,RGB(77,77,77));
			}

			m_list->SetItemText(i,1,lpwzInherited);
			m_list->SetItemText(i,2,lpwzProName);
			m_list->SetItemText(i,3,lpwzEProcess);
			m_list->SetItemText(i,4,lpwzType);
			m_list->SetItemText(i,5,lpwzMoreEvents);
			m_list->SetItemText(i,6,lpwzCreateProcess);
			m_list->SetItemText(i,7,lpwzTrue);

			if(HasFile)
			{
				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzCreateProcess,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
				HipsLogImg.Add(shfileinfo.hIcon);
			}else
			{
				HipsLogImg.Add(imgApp->LoadIconW(IDI_WHITE));
			}
			m_list->SetImageList(&HipsLogImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
			HasFile=TRUE;
		}
		VirtualFree(LogDefenseInfo,sizeof(LOGDEFENSE)*1025,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"防御日志扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

	//如果是导出日志的话，就不需要清空了
	if (Type == 0 &&
		bIsSaveLogFile)
	{
		ReadFile((HANDLE)CLEAR_LIST_LOG,0,0,&dwReadByte,0);
		bIsSaveLogFile = FALSE;
	}
}
void ClearListLog(CMyList *m_list)
{
	DWORD dwReadByte;
	m_list->DeleteAllItems();
	ReadFile((HANDLE)CLEAR_LIST_LOG,0,0,&dwReadByte,0);
}
void ConnectScan(HWND m_hWnd)
{
	DWORD dwReadByte;
	if (MessageBoxW(m_hWnd,L"此功能将在系统重启后自动收集全部的可疑行为\r\n系统重启后点击“查看防御日志”即可\r\n是否重启？\r\n\r\n严重警告：使用此功能请先关闭或者暂停杀毒软件，否则杀毒软件误删了A盾释放的文件会导致系统重启崩溃\r\n"
		,L"A盾电脑防护",MB_YESNO | MB_ICONWARNING) == IDYES)
	{
		ReadFile((HANDLE)KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);

		if (InstallDepthServicesScan("A-Protect"))
		{
			ReadFile((HANDLE)EXIT_PROCESS,0,0,&dwReadByte,0);
			Sleep(2000);
			ShutdownWindows(EWX_REBOOT | EWX_FORCE);
			ExitProcess(0);
		}
		ReadFile((HANDLE)NO_KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);
	}
}
void SaveToFile(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	m_list->DeleteAllItems();
	HipsLog(m_hWnd,ID,m_list,1);
}