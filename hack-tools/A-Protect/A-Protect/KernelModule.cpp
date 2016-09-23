#include "stdafx.h"
#include "A-ProtectView.h"
#include "KernelModule.h"
#include "A-Protect.h"
#include "DisplayDecvice.cpp"

#include "Windows7Home_CN.h"
#include "Windows2003SP1_CN.h"
#include "Windows2003SP2_CN.h"
#include "Windows7SP1_CN.h"
#include "WindowsXPSP2_CN.h"
#include "WindowsXPSP3_CN.h"

BOOL VerifyEmbeddedSignature( LPCWSTR lpFileName );
BOOL IsWindows7();
void RunAProcess(char *comline);
LPSTR ExtractFilePath(LPSTR lpcFullFileName);

extern BOOL TrustQuery;

extern unsigned char szQueryValue[256];

BOOL FileVerify(char *lpszFullPath,WCHAR *lpwzFileMd5,WCHAR *lpwzTrue)
{
	memset(lpwzTrue,0,sizeof(lpwzTrue));
	wcscat(lpwzTrue,L"无法确认文件来源");

	OSVERSIONINFOEX   osvi;   
	BOOL   bOsVersionInfoEx;   

	ZeroMemory(&osvi, sizeof(OSVERSIONINFOEX));   
	osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFOEX);   
	if(!(bOsVersionInfoEx = GetVersionEx((OSVERSIONINFO*)&osvi)))   
	{   
		osvi.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);   
		if(!GetVersionEx((OSVERSIONINFO*)&osvi))     
			return FALSE;   
	}
	for (int x=0;x<2888;x++)
	{
		if (osvi.dwBuildNumber == 2600 &&
			osvi.wServicePackMajor == 3 &&
			GetSystemDefaultLCID() == 2052)  //xpSP3_cn
		{
			if (!wcslen(WindowsXPSP3_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(WindowsXPSP3_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"MD5(√)/签名(-)");
				break;
			}
		}else if (osvi.dwBuildNumber == 2600 &&
			osvi.wServicePackMajor == 2 &&
			GetSystemDefaultLCID() == 2052)  //xpSP2_cn
		{
			if (!wcslen(WindowsXPSP2_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(WindowsXPSP2_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"MD5(√)/签名(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 3790 &&
			osvi.wServicePackMajor == 1 &&
			GetSystemDefaultLCID() == 2052)  //2003SP1_cn
		{
			if (!wcslen(Windows2003SP1_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows2003SP1_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"MD5(√)/签名(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 3790 &&
			osvi.wServicePackMajor == 2 &&
			GetSystemDefaultLCID() == 2052)  //2003SP2_cn
		{
			if (!wcslen(Windows2003SP2_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows2003SP2_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"MD5(√)/签名(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 7600 &&
			osvi.wServicePackMajor == 0 &&
			GetSystemDefaultLCID() == 2052)  //win7 home
		{
			if (!wcslen(Windows7Home_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows7Home_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"MD5(√)/签名(-)");
				break;
			}
		}
		else if (osvi.dwBuildNumber == 7601 &&
			osvi.wServicePackMajor == 1 &&
			GetSystemDefaultLCID() == 2052)  //win7 旗舰 SP1_CN
		{
			if (!wcslen(Windows7SP1_CN[x]))
			{
				break;
			}
			if (_wcsnicmp(Windows7SP1_CN[x],lpwzFileMd5,wcslen(lpwzFileMd5)) == NULL)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"MD5(√)/签名(-)");
				break;
			}
		}
		else
		{
			memset(lpwzTrue,0,sizeof(lpwzTrue));
			wcscat(lpwzTrue,L"不支持当前系统");
			break;
		}
	}
	if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0 ||
		_wcsnicmp(lpwzTrue,L"不支持当前系统",wcslen(L"不支持当前系统")) == 0)
	{
		if (TrustQuery)
	{
		CHAR lpszProcFullPath[260] = {0};
		WCHAR lpwzProcFullPath[260] = {0};
		memset(lpwzProcFullPath,0,sizeof(lpwzProcFullPath));
		memset(lpszProcFullPath,0,sizeof(lpszProcFullPath));

		strcat(lpszProcFullPath,lpszFullPath);
		MultiByteToWideChar(
			CP_ACP,
			0, 
			lpszProcFullPath,
			-1, 
			lpwzProcFullPath, 
			strlen(lpszProcFullPath)
			);
		if (VerifyEmbeddedSignature(lpwzProcFullPath))
		{
			memset(lpwzTrue,0,sizeof(lpwzTrue));
			wcscat(lpwzTrue,L"MD5(-)/签名(√)");
		}
	}
	}
	return TRUE;
}

VOID QueryKernelModule(HWND m_hWnd,ULONG ID,CMyList *m_list,int IntLookType)
{
	DWORD dwReadByte;
	int ItemNum = m_list->GetItemCount();
	int x = 0;
	int i = 0;
	int FileNotExist=false;
	CMyAProtectApp *imgApp=(CMyAProtectApp*)AfxGetApp();

	SetDlgItemTextW(m_hWnd,ID,L"正在扫描驱动模块，请稍后...");

	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---驱动模块---]\r\n",PhysicalFile);
	}
	if (SysModuleInfo)
	{
		VirtualFree(SysModuleInfo,sizeof(SYSINFO)*264,MEM_RESERVE | MEM_COMMIT);
		SysModuleInfo = NULL;
	}
	SysModuleInfo = (PSYSINFO)VirtualAlloc(0, sizeof(SYSINFO)*264,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (SysModuleInfo)
	{
		//为进程图标服务
		SHFILEINFO shfileinfo;
		KernalModuleImg.Create(16,16, ILC_COLOR32, 2, 100);
		HIMAGELIST hImageList = NULL;
/*
		//win7系统下禁用显卡再枚举，不然会造成假死，原因不明~！！！
		if (IsWindows7()){
			//没有体检的情况下，才会提示
			if (!bIsPhysicalCheck){
				MessageBox(0,L"win7下枚举驱动模块会降低屏幕分辨率，枚举完成即可自动恢复，这是正常现象，亲莫慌！", L"友情提示",MB_ICONWARNING);
			}
			char lpAptDir[260] = {0};
			char lpModule[260] = {0};
			char lpCommand[260] = {0};
			char lpDecviceKey[260] = {0};

			GetModuleFileNameA(NULL,lpModule,sizeof(lpModule));
			wsprintfA(lpAptDir,"%s",ExtractFilePath(lpModule)); //得到路径的目录

			QueryDisplayDecvicePath(lpDecviceKey);
			strcat(lpCommand,lpAptDir);
			strcat(lpCommand,"winspapi.exe");
			strcat(lpCommand," disable ");    //禁用显卡驱动，避免造成假死
			strcat(lpCommand,lpDecviceKey);
			WinExec(lpCommand,SW_HIDE);
			Sleep(3000);
		}
*/
		ReadFile((HANDLE)LIST_SYS_MODULE,SysModuleInfo,sizeof(SYSINFO)*264,&dwReadByte,0);
/*
		//启用显卡
		if (IsWindows7()){
			char lpAptDir[260] = {0};
			char lpModule[260] = {0};
			char lpCommand[260] = {0};
			char lpDecviceKey[260] = {0};

			GetModuleFileNameA(NULL,lpModule,sizeof(lpModule));
			wsprintfA(lpAptDir,"%s",ExtractFilePath(lpModule)); //得到路径的目录

			QueryDisplayDecvicePath(lpDecviceKey);
			strcat(lpCommand,lpAptDir);
			strcat(lpCommand,"winspapi.exe");   //启动显卡驱动
			strcat(lpCommand," enable ");
			strcat(lpCommand,lpDecviceKey);
			WinExec(lpCommand,SW_HIDE);
		}
*/
		for (i=0;i< SysModuleInfo->ulCount;i++)
		{
			WCHAR lpwzTextOut[100];
			memset(lpwzTextOut,0,sizeof(lpwzTextOut));
			wsprintfW(lpwzTextOut,L"共有 %d 个数据，正在扫描第 %d 个，请稍后...",SysModuleInfo->ulCount,i);
			SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

			WCHAR lpwzSysBase[256] = {0};
			WCHAR lpwzSizeOfImage[256] = {0};

			WCHAR lpwzFullSysName[256] = {0};
			WCHAR lpwzBaseSysName[256] = {0};
			WCHAR lpwzServiceName[256] = {0};

			WCHAR lpwzDriverObject[256] = {0};

			WCHAR IntHideType[256] = {0};

			memset(lpwzSysBase,0,sizeof(lpwzSysBase));
			memset(lpwzSizeOfImage,0,sizeof(lpwzSizeOfImage));

			memset(lpwzFullSysName,0,sizeof(lpwzFullSysName));
			memset(lpwzBaseSysName,0,sizeof(lpwzBaseSysName));

			memset(lpwzServiceName,0,sizeof(lpwzServiceName));
			memset(lpwzDriverObject,0,sizeof(lpwzDriverObject));

			memset(IntHideType,0,sizeof(IntHideType));

			if (SysModuleInfo->SysInfo[i].ulSysBase)
				wsprintfW(lpwzSysBase,L"0x%08X",SysModuleInfo->SysInfo[i].ulSysBase);
			else
				wsprintfW(lpwzSysBase,L"%ws",L"-");

			if (SysModuleInfo->SysInfo[i].SizeOfImage)
				wsprintfW(lpwzSizeOfImage,L"0x%X",SysModuleInfo->SysInfo[i].SizeOfImage);
			else
				wsprintfW(lpwzSizeOfImage,L"%ws",L"-");

			wcscat(lpwzFullSysName,SysModuleInfo->SysInfo[i].lpwzFullSysName);
			wcscat(lpwzBaseSysName,SysModuleInfo->SysInfo[i].lpwzBaseSysName);

			if (wcsstr(SysModuleInfo->SysInfo[i].lpwzServiceName,L"\\Driver\\")){
				memcpy(lpwzServiceName,SysModuleInfo->SysInfo[i].lpwzServiceName+wcslen(L"\\Driver\\"),wcslen(SysModuleInfo->SysInfo[i].lpwzServiceName)*2-wcslen(L"\\Driver\\"));
			}
			if (wcsstr(SysModuleInfo->SysInfo[i].lpwzServiceName,L"\\FileSystem\\")){
				memcpy(lpwzServiceName,SysModuleInfo->SysInfo[i].lpwzServiceName+wcslen(L"\\FileSystem\\"),wcslen(SysModuleInfo->SysInfo[i].lpwzServiceName)*2-wcslen(L"\\FileSystem\\"));
			}
			if (!wcslen(SysModuleInfo->SysInfo[i].lpwzServiceName)){
				memcpy(lpwzServiceName,L"-",wcslen(L"-"));
			}
			if (SysModuleInfo->SysInfo[i].DriverObject)
				wsprintfW(lpwzDriverObject,L"0x%08X",SysModuleInfo->SysInfo[i].DriverObject);
			else
				wsprintfW(lpwzDriverObject,L"%ws",L"-");

			/////
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
			if (!wcslen(lpwzFullSysName) &&
				!wcslen(lpwzBaseSysName)){
					wcscat(lpwzDosFullPath,L"-");
					goto Next;
			}
			if (wcslen(lpwzFullSysName) == wcslen(lpwzBaseSysName) &&
				wcsncmp(lpwzFullSysName,L"-",wcslen(L"-")) != 0)
			{
				memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
				wcscat(lpwzDosFullPath,lpwzSysDisk);
				wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
				wcscat(lpwzDosFullPath,lpwzBaseSysName);
				goto Next;
			}
Next:
			/////
			//处理MD5
			//计算md5
			char lpszFullPath[5024] = {0};
			WCHAR lpwzMD5[256];
			WCHAR lpwzTrue[256];

			memset(lpszFullPath,0,sizeof(lpszFullPath));
			memset(lpwzMD5,0,sizeof(lpwzMD5));
			memset(lpwzTrue,0,sizeof(lpwzTrue));
			WideCharToMultiByte( CP_ACP,
				0,
				lpwzDosFullPath,
				-1,
				lpszFullPath,
				wcslen(lpwzDosFullPath)*2,
				NULL,
				NULL);
			FILE * fp=fopen(lpszFullPath,"rb");
			if(fp)
			{
				MD5VAL val;
				val = md5File(fp);
				wsprintfW(lpwzMD5,L"%08x%08x%08x%08x",conv(val.a),conv(val.b),conv(val.c),conv(val.d));
				FileVerify(lpszFullPath,lpwzMD5,lpwzTrue);
				//MessageBoxW(lpwzDosFullPath,lpwzMD5,0);
				fclose(fp);
			}
			//文件不存在
			if (GetFileAttributesA(lpszFullPath) == -1)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"文件不存在，无法验证");
			}
			//这里是一键体检的数据，不需要插入界面了
			if (bIsPhysicalCheck){
				//如果没有hook，就返回
				if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0 ||
					_wcsnicmp(lpwzTrue,L"文件不存在，无法验证",wcslen(L"文件不存在，无法验证")) == 0 ||
					SysModuleInfo->SysInfo[i].IntHideType == FALSE)
				{
					WCHAR lpwzSaveBuffer[1024] ={0};
					CHAR lpszSaveBuffer[2024] ={0};
					memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
					memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

					wsprintfW(lpwzSaveBuffer,L"          --> 发现无法识别内核模块：DriverObject:%ws | 驱动映像:%ws | 驱动服务:%ws | 驱动路径:%ws\r\n",
						lpwzDriverObject,lpwzBaseSysName,lpwzServiceName,lpwzDosFullPath);

					m_list->InsertItem(0,L"驱动模块",RGB(77,77,77));
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
			if (SysModuleInfo->SysInfo[i].IntHideType == FALSE)
			{
				memset(lpwzTrue,0,sizeof(lpwzTrue));
				wcscat(lpwzTrue,L"无法识别的驱动对象");
				m_list->InsertItem(i,lpwzSysBase,RGB(255,20,147));
				FileNotExist=true;
				goto InsertData;
			}
			if (_wcsnicmp(lpwzTrue,L"无法确认文件来源",wcslen(L"无法确认文件来源")) == 0 ||
				SysModuleInfo->SysInfo[i].IntHideType == FALSE)  //SysModuleInfo->SysInfo[i].IntHideType = FASLE 为隐藏驱动
			{
				m_list->InsertItem(i,lpwzSysBase,RGB(238,118,0));

			}else if (_wcsnicmp(lpwzTrue,L"文件不存在，无法验证",wcslen(L"文件不存在，无法验证")) == 0)
			{
				m_list->InsertItem(i,lpwzSysBase,RGB(255,20,147));
				FileNotExist=true;
			}else
			{
				m_list->InsertItem(i,lpwzSysBase,RGB(77,77,77));
			}
InsertData:
			m_list->SetItemText(i,1,lpwzSizeOfImage);
			m_list->SetItemText(i,2,lpwzDriverObject);

			m_list->SetItemText(i,3,lpwzBaseSysName);
			m_list->SetItemText(i,4,lpwzDosFullPath);
			m_list->SetItemText(i,5,lpwzServiceName);

			m_list->SetItemText(i,6,lpwzTrue);
		
			hImageList=(HIMAGELIST)::SHGetFileInfo(lpwzDosFullPath,0,&shfileinfo,sizeof(shfileinfo),SHGFI_ICON);
			if(SUCCEEDED(hImageList))
			{
				if(!FileNotExist)
					KernalModuleImg.Add(shfileinfo.hIcon);
				else
					KernalModuleImg.Add(imgApp->LoadIconW(IDI_WHITE));
				m_list->SetImageList(&KernalModuleImg);
				m_list->SetItemImageId(i,i);
				DestroyIcon(shfileinfo.hIcon);
			}
			FileNotExist=false;
		}
	}else{
		WCHAR lpwzTextOut[100];
		memset(lpwzTextOut,0,sizeof(lpwzTextOut));
		wsprintfW(lpwzTextOut,L"申请内存错误， 请重新运行A盾\r\n错误代码:%d\n",GetLastError());
		MessageBox(0,lpwzTextOut,0,0);
	}
	WCHAR lpwzTextOut[100];
	memset(lpwzTextOut,0,sizeof(lpwzTextOut));
	wsprintfW(lpwzTextOut,L"内核模块扫描完毕，共有 %d 个数据",i);
	SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);
}
void CopyKernelModuleDataToClipboard(HWND m_hWnd,CMyList *m_list)
{
	CString KernelModule;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	KernelModule.Format(L"%s",m_list->GetItemText(Item,3));

	WCHAR lpwzKernelModule[260];

	memset(lpwzKernelModule,0,sizeof(lpwzKernelModule));
	wcscat(lpwzKernelModule,KernelModule);
	CHAR lpszKernelModule[1024];
	char *lpString = NULL;

	memset(lpwzKernelModule,0,sizeof(lpwzKernelModule));
	memset(lpszKernelModule,0,sizeof(lpszKernelModule));
	wcscat(lpwzKernelModule,KernelModule);
	WideCharToMultiByte( CP_ACP,
		0,
		lpwzKernelModule,
		-1,
		lpszKernelModule,
		wcslen(lpwzKernelModule)*2,
		NULL,
		NULL
		);
	lpString = setClipboardText(lpszKernelModule);
	if (lpString)
	{
		MessageBoxW(m_hWnd,L"操作成功！",L"A盾电脑防护",MB_ICONWARNING);
	}
}
BOOL SaveToFile(WCHAR *lpwzFilePath,PVOID Buffer,ULONG ulBufferSize)
{
	DWORD dwBytesWrite = 0;
	BOOL bRetOK = FALSE;

	HANDLE	hFile = CreateFileW(lpwzFilePath, GENERIC_WRITE, FILE_SHARE_WRITE,
		NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	if (hFile != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(hFile, 0, 0, FILE_END);
		WriteFile(hFile,Buffer,ulBufferSize, &dwBytesWrite, NULL);
		CloseHandle(hFile);
		bRetOK = TRUE;
	}
	return bRetOK;
}
void OnDumpmemorydatatofile(HWND hWnd,CMyList *m_list)
{
	// TODO: 在此添加命令处理程序代码


	CString			m_path;

	CString KernelModuleBase;
	int ItemNum = m_list->GetItemCount();
	POSITION pos = m_list->GetFirstSelectedItemPosition(); //判断列表框中是否有选择项
	int Item = m_list->GetNextSelectedItem(pos); //将列表中被选择的下一项索引值保存到数组中

	KernelModuleBase.Format(L"%s",m_list->GetItemText(Item,0));

	WCHAR lpwzKernelModuleBase[260];
	WCHAR lpwzSysBase[260];
	CHAR lpszSysSize[260];

	WCHAR lpwzFilePath[260];
	DWORD dwReadByte;
	WCHAR lpwzForMat[260];

	memset(lpwzForMat,0,sizeof(lpwzForMat));
	memset(lpwzFilePath,0,sizeof(lpwzFilePath));
	memset(lpwzSysBase,0,sizeof(lpwzSysBase));
	memset(lpwzKernelModuleBase,0,sizeof(lpwzKernelModuleBase));

	wcscat(lpwzKernelModuleBase,KernelModuleBase);
	if (!wcslen(lpwzKernelModuleBase))
	{
		return;
	}

	//暂停不然无法因为保护着win7下无法打开对话框
	ReadFile((HANDLE)SUSPEND_PROTECT,0,0,&dwReadByte,0);

	CFileDialog dlg( FALSE,L"txt",0, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY,L"所有文件|*.*");
	dlg.m_ofn.lpstrTitle= L"保存dump";
	if ( dlg.DoModal() == IDOK )
	{
		m_path = dlg.GetPathName();
		wsprintfW(lpwzFilePath,L"\\??\\%ws",m_path);

		ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);

		for (int i=0;i< (int)SysModuleInfo->ulCount;i++)
		{
			wsprintfW(lpwzSysBase,L"0x%08X",SysModuleInfo->SysInfo[i].ulSysBase);
			wsprintfA(lpszSysSize,"%d",SysModuleInfo->SysInfo[i].SizeOfImage);

			if (_wcsnicmp(lpwzKernelModuleBase,lpwzSysBase,wcslen(lpwzSysBase)) == 0)
			{
				ReadFile((HANDLE)INIT_DUMP_KERNEL_MODULE_MEMORY,0,SysModuleInfo->SysInfo[i].ulSysBase,&dwReadByte,0);
				ReadFile((HANDLE)INIT_DUMP_KERNEL_MODULE_MEMORY_1,0,SysModuleInfo->SysInfo[i].SizeOfImage,&dwReadByte,0);
				ReadFile((HANDLE)DUMP_KERNEL_MODULE_MEMORY,lpwzFilePath,wcslen(lpwzFilePath),&dwReadByte,0);

				wsprintfW(lpwzForMat,L"操作成功，dump文件保存至:%ws",m_path);
				MessageBoxW(hWnd,lpwzForMat,L"A盾电脑防护",MB_ICONWARNING);
				break;
			}
		}
	}
	ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);
}
