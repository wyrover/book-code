#include "StdAfx.h"
#include "SSDT.h"
#include "A-ProtectView.h"
#include "wininet.h"
#include "A-Protect.h"

#pragma comment(lib,"wininet.lib")

BOOL QueryUserAgent(HKEY HKey,char *lpSubKey,char *lpValueName,char *OutBuffer);

BOOL bIsEyeOpen = EYEUNKNOWN;

VOID QuerySSDTHook(HWND m_hWnd,ULONG ID,int IntType,CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0;

	SHFILEINFO shfileinfo;
	SSDTImg.Create(16,16, ILC_COLOR32, 2, 100);
	HIMAGELIST hImageList = NULL;
	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();

	SetDlgItemTextW(m_hWnd,IDC_DebugStatus,L"正在扫描SSDT，请稍后...");

	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---SSDT---]\r\n",PhysicalFile);
	}

	SSDTInfo = (PSSDTINFO)VirtualAlloc(0, sizeof(SSDTINFO)*810,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (SSDTInfo)
	{
		memset(SSDTInfo,0,sizeof(SSDTINFO)*810);
		if (IntType == 0)
		{
			ReadFile((HANDLE)LIST_SSDT,SSDTInfo,sizeof(SSDTINFO)*810,&dwReadByte,0);
		}else
			ReadFile((HANDLE)LIST_SSDT_ALL,SSDTInfo,sizeof(SSDTINFO)*810,&dwReadByte,0);

		for ( i=0;i< (int)SSDTInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",SSDTInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzNumber[256] = {0};
			WCHAR lpwzHookType[256] = {0};
			WCHAR lpwzFunction[256] = {0};
			WCHAR lpwzHookModuleImage[256] = {0};
			WCHAR lpwzMemoryFunctionBase[256] = {0};
			WCHAR lpwzRealFunctionBase[256] = {0};
			WCHAR lpwzHookModuleBase[256] = {0};
			WCHAR lpwzHookModuleSize[256] = {0};

			memset(lpwzNumber,0,sizeof(lpwzNumber));
			memset(lpwzHookType,0,sizeof(lpwzHookType));

			memset(lpwzFunction,0,sizeof(lpwzFunction));
			memset(lpwzHookModuleImage,0,sizeof(lpwzHookModuleImage));
			memset(lpwzMemoryFunctionBase,0,sizeof(lpwzMemoryFunctionBase));
			memset(lpwzRealFunctionBase,0,sizeof(lpwzRealFunctionBase));
			memset(lpwzHookModuleBase,0,sizeof(lpwzHookModuleBase));
			memset(lpwzHookModuleSize,0,sizeof(lpwzHookModuleSize));

			if (SSDTInfo->SSDT[i].ulNumber == 0xffffffff)
			{
				StrCatW(lpwzNumber,L"*");
			}
			else
				wsprintfW(lpwzNumber,L"%d",SSDTInfo->SSDT[i].ulNumber);

			MultiByteToWideChar(
				CP_ACP,
				0, 
				SSDTInfo->SSDT[i].lpszFunction,
				-1, 
				lpwzFunction, 
				strlen(SSDTInfo->SSDT[i].lpszFunction)
				);
			MultiByteToWideChar(
				CP_ACP,
				0, 
				SSDTInfo->SSDT[i].lpszHookModuleImage,
				-1, 
				lpwzHookModuleImage, 
				strlen(SSDTInfo->SSDT[i].lpszHookModuleImage)
				);
			wsprintfW(lpwzMemoryFunctionBase,L"0x%08X",SSDTInfo->SSDT[i].ulMemoryFunctionBase);
			wsprintfW(lpwzRealFunctionBase,L"0x%08X",SSDTInfo->SSDT[i].ulRealFunctionBase);
			wsprintfW(lpwzHookModuleBase,L"0x%08X",SSDTInfo->SSDT[i].ulHookModuleBase);
			wsprintfW(lpwzHookModuleSize,L"%d KB",SSDTInfo->SSDT[i].ulHookModuleSize/1024);

			switch (SSDTInfo->SSDT[i].IntHookType)
			{
			case 0:
				StrCatW(lpwzHookType,L"-");
				break;
			case 1:
				StrCatW(lpwzHookType,L"SSDT hook");
				break;
			case 2:
				StrCatW(lpwzHookType,L"SSDT Inline hook");
				break;
			case 3:
				StrCatW(lpwzHookType,L"Inline hook");
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
				//如果没有hook，就返回
				if (SSDTInfo->SSDT[i].IntHookType == 0){
					continue;
				}
				WCHAR lpwzSaveBuffer[1024] ={0};
				CHAR lpszSaveBuffer[2024] ={0};
				memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
				memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

				wsprintfW(lpwzSaveBuffer,L"          --> 发现Hook:ID:%ws | 当前地址:%ws | 原始地址:%ws | 函数名:%ws | 内核模块:%ws | Hook类型:%ws\r\n",
					lpwzNumber,lpwzMemoryFunctionBase,lpwzRealFunctionBase,lpwzFunction,lpwzDosFullPath,lpwzHookType);

				m_list->InsertItem(0,L"SSDT",RGB(77,77,77));
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
			if (SSDTInfo->SSDT[i].IntHookType == 0)
			{
				m_list->InsertItem(i,lpwzNumber,RGB(77,77,77));

			}else
			{
				m_list->InsertItem(i,lpwzNumber,RGB(255,20,147));
			}
			
			m_list->SetItemText(i,1,lpwzMemoryFunctionBase);
			m_list->SetItemText(i,2,lpwzRealFunctionBase);
			m_list->SetItemText(i,3,lpwzFunction);
			m_list->SetItemText(i,4,lpwzDosFullPath);
			m_list->SetItemText(i,5,lpwzHookModuleBase);
			m_list->SetItemText(i,6,lpwzHookModuleSize);
			m_list->SetItemText(i,7,lpwzHookType);

			if(GetFileAttributes(lpwzDosFullPath)!=INVALID_FILE_ATTRIBUTES)
			{
				hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
				SSDTImg.Add(shfileinfo.hIcon);
			}else
				SSDTImg.Add(imgApp->LoadIconW(IDI_WHITE));
			m_list->SetImageList(&SSDTImg);
			m_list->SetItemImageId(i,i);
			DestroyIcon(shfileinfo.hIcon);
		}
		VirtualFree(SSDTInfo,sizeof(SSDTINFO)*810,MEM_RESERVE | MEM_COMMIT);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"SSDT扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

}
VOID UnHookSSDTAll(HWND m_hWnd,CMyList *m_list)
{
	DWORD dwReadByte;

	m_list->DeleteAllItems();

	ReadFile((HANDLE)SET_ALL_SSDT,0,0,&dwReadByte,0);

	QuerySSDTHook(m_hWnd,IDC_DebugStatus,0,m_list);
}
VOID UnHookSSDT(HWND m_hWnd,CMyList *m_list)
{
	DWORD dwReadByte;
	CString str;
	CString Num;

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
	FunctionStr.Format(L"%s",m_list->GetItemText(Item,2));
	if (wcsstr(FunctionStr,L"KiFastCallEntry") != NULL)
	{
		WCHAR lpwzFunction[100];

		memset(lpwzFunction,0,sizeof(lpwzFunction));

		wcscat_s(lpwzFunction,FunctionStr);
		ReadFile((HANDLE)SET_INLINE_HOOK,lpwzFunction,wcslen(lpwzFunction),&dwReadByte,0);
		m_list->DeleteAllItems();
		QuerySSDTHook(m_hWnd,IDC_DebugStatus,0,m_list);
	}
	if (wcsstr(str,L"SSDT") != NULL)
	{
		WCHAR lpwzNum[10];
		char lpszNum[10];

		//取服务号
		Num.Format(L"%s",m_list->GetItemText(Item,0));

		memset(lpszNum,0,sizeof(lpszNum));
		memset(lpwzNum,0,sizeof(lpwzNum));
		wcscat_s(lpwzNum,Num);

		WideCharToMultiByte (CP_OEMCP,NULL,lpwzNum,-1,lpszNum,wcslen(lpwzNum),NULL,FALSE);

		//MessageBoxA(0,lpszNum,0,0);
		ReadFile((HANDLE)SET_ONE_SSDT,0,atoi(lpszNum),&dwReadByte,0);

		//删除当前选择的一行
		//m_SSDTList.DeleteItem(Item);
		m_list->DeleteAllItems();
		QuerySSDTHook(m_hWnd,IDC_DebugStatus,0,m_list);
	}
}
void CopySSDTDataToClipboard(HWND hWnd,CMyList *m_list)
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
		MessageBoxW(hWnd,L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}
BOOL GetInterNetURLText(CHAR *lpcInterNetURL,CHAR *lpwzReadData)
{
	HINTERNET hSession;
	HINTERNET hRequest;
	BOOL bResult = FALSE;
	DWORD dwBytesRead;
	CHAR lpwzUserAgent[256];

	memset(lpwzUserAgent,0,sizeof(lpwzUserAgent));
	QueryUserAgent(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Internet Settings","User Agent",lpwzUserAgent);
	//QueryUserAgent(lpwzUserAgent);
	//MessageBoxA(0,lpwzUserAgent,0,0);
	hSession = InternetOpenA(lpwzUserAgent, INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (hSession != NULL)
	{
		//MessageBoxA(0,"InternetOpenA",0,0);
		hRequest = InternetOpenUrlA(hSession, lpcInterNetURL, NULL, 0, INTERNET_FLAG_RELOAD, 0);
		if (hRequest != NULL)
		{
			//MessageBoxA(0,"InternetOpenUrlA",0,0);
			if (InternetReadFile(hRequest, lpwzReadData,256, &dwBytesRead))
			{
				//MessageBoxA(0,lpwzReadData,0,0);
				bResult = TRUE;
			}
			InternetCloseHandle(hRequest);
		}
		InternetCloseHandle(hSession);
	}
	return bResult;
}
VOID QueryInternetInfo(HWND hWnd)
{
	CHAR lpszText[256] = {0};
	CHAR lpszText1[256] = {0};
	WCHAR lpwzText[256];
	WSADATA wsaData;

	//添加统计代码
	RunAProcess("sc delete A-ProtectUserCount");
	RunAProcess("sc Create A-ProtectUserCount binPath= \"C:\\Program Files\\Internet Explorer\\iexplore.exe http://www.3600safe.com/A-ProtectCount.php\"");
	RunAProcess("sc start A-ProtectUserCount");
	RunAProcess("sc delete A-ProtectUserCount");

	memset(lpOpenUrl,0,sizeof(lpOpenUrl));

	ULONG err = WSAStartup(MAKEWORD( 2,2 ), &wsaData );
	if (err == 0)
	{
		memset(lpwzText,0,sizeof(lpwzText));
		if (gethostbyname("www.baidu.com"))
		{
			memset(lpszText,0,sizeof(lpszText));
			GetInterNetURLText("http://www.3600safe.com/A-ProtectEye.html",lpszText);
			if (strlen(lpszText) != 1){
				bIsEyeOpen = 8888;
			}else
				bIsEyeOpen = atoi(lpszText);

			//发送统计
			//GetInterNetURLText("http://www.3600safe.com/A-ProtectCount.asp",lpszText);

			//ShellExecuteA(0,0,"http://www.3600safe.com/A-ProtectCount.php",0,0,SW_HIDE);

			//获取内容
			memset(lpszText,0,sizeof(lpszText));
			GetInterNetURLText("http://www.3600safe.com/A-ProtectNews.html",lpszText);
			char *p = strstr(lpszText,"[A-Protect]");
			if (p)
			{
				char *p1 = strstr(lpszText,"[URL]");
				strcat_s(lpOpenUrl,p1+strlen("[URL]"));

				memcpy(lpszText1,lpszText,strlen(lpszText)-strlen("[URL]")-strlen(lpOpenUrl));
				//MessageBoxA(0,lpOpenUrl,lpszText1,0);
			}else
			{
				strcat_s(lpszText1,"A盾电脑防护 Anti-Rootkit Tools For Windows XP/2003/7 (32Bit)");
			}
		}else
		{
			strcat_s(lpszText1,"A盾电脑防护 Anti-Rootkit Tools For Windows XP/2003/7 (32Bit)");
			bIsEyeOpen = EYEUNKNOWN;
		}
		MultiByteToWideChar(CP_ACP,0,lpszText1,strlen(lpszText1),lpwzText,sizeof(lpwzText));
		SetDlgItemTextW(hWnd,IDC_InterNetInfo,lpwzText);

		WSACleanup();
	}
	RunAProcess("taskkill /f /im sc.exe /t");
}