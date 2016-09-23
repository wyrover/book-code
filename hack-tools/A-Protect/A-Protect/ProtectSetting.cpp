// ProtectSetting.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "ProtectSetting.h"
#include "afxdialogex.h"
#include "uninstall360.h"

// CProtectSetting 对话框

BOOL bDisLoadDriver = TRUE;
BOOL bDisCreateProcess = TRUE;
BOOL bDisWriteFile = TRUE;
BOOL bDisResetServices = TRUE;
BOOL bDisKernelThread = TRUE;
BOOL bDisSetWindowsHook = TRUE;
BOOL bDisDllFuck = TRUE;

IMPLEMENT_DYNAMIC(CProtectSetting, CDialogEx)

CProtectSetting::CProtectSetting(CWnd* pParent /*=NULL*/)
	: CDialogEx(CProtectSetting::IDD, pParent)
{
}

CProtectSetting::~CProtectSetting()
{
}

void CProtectSetting::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	if (bDisLoadDriver == FALSE){
		SetDlgItemText(IDC_DisLoadDriver,L"允许加载驱动");
	}
	if (bDisCreateProcess == FALSE){
		SetDlgItemText(IDC_DisCreateProcess,L"允许创建进程");
	}
	if (bDisWriteFile == FALSE){
		SetDlgItemText(IDC_DisCreateFile,L"允许创建文件");
	}
	if (bDisResetServices == FALSE){
		SetDlgItemText(IDC_DisSrvReset,L"允许服务回写");
	}
	if (bDisKernelThread == FALSE){
		SetDlgItemText(IDC_DisKernelThread,L"允许内核线程");
	}
	if (bDisSetWindowsHook == FALSE){
		SetDlgItemText(IDC_DisUserHook,L"允许全局钩子");
	}
	if (bDisDllFuck == FALSE){
		SetDlgItemText(IDC_DisDllFuck,L"开启DLL挟持防护");
	}
}


BEGIN_MESSAGE_MAP(CProtectSetting, CDialogEx)
	ON_BN_CLICKED(IDC_DeleteFile, &CProtectSetting::OnBnClickedDeletefile)
	ON_BN_CLICKED(IDC_DisLoadDriver, &CProtectSetting::OnBnClickedDisloaddriver)
	ON_BN_CLICKED(IDC_DisCreateProcess, &CProtectSetting::OnBnClickedDiscreateprocess)
	ON_BN_CLICKED(IDC_DisCreateFile, &CProtectSetting::OnBnClickedDiscreatefile)
	ON_BN_CLICKED(IDC_DisSrvReset, &CProtectSetting::OnBnClickedDissrvreset)
	ON_BN_CLICKED(IDC_DisKernelThread, &CProtectSetting::OnBnClickedDiskernelthread)
	ON_BN_CLICKED(IDC_DisUserHook, &CProtectSetting::OnBnClickedDisSetWindowsHook)
	ON_BN_CLICKED(IDC_DisDllFuck, &CProtectSetting::OnBnClickedDisDllFuck)
	

	ON_BN_CLICKED(IDC_ShutdownSystem, &CProtectSetting::OnBnClickedShutdownsystem)
	ON_BN_CLICKED(IDC_Uninstall360, &CProtectSetting::OnBnClickedUninstall360)
END_MESSAGE_MAP()


// CProtectSetting 消息处理程序


void CProtectSetting::OnBnClickedDeletefile()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("暴力结束进程"));
	DWORD dwReadByte;
	WCHAR lpwzDeletedFile[256];
	CString			m_path;

	//暂停不然无法因为保护着win7下无法打开对话框
	ReadFile((HANDLE)SUSPEND_PROTECT,0,0,&dwReadByte,0);
	//Sleep(3000);

	//强制删除文件的时候，要切换到内核安全模式去
	ReadFile((HANDLE)KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);

	CFileDialog dlg( TRUE,L"*.*",0, 0,L"所有文件|*.*");
	dlg.m_ofn.lpstrTitle= L"暴力删除文件";
	if ( dlg.DoModal() == IDOK )
	{
		m_path = dlg.GetPathName();

		memset(lpwzDeletedFile,0,sizeof(lpwzDeletedFile));
		wsprintfW(lpwzDeletedFile,L"\\??\\%ws",m_path);

		ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);

		ReadFile((HANDLE)DELETE_FILE,lpwzDeletedFile,wcslen(lpwzDeletedFile),&dwReadByte,0);
		if (GetFileAttributesW(m_path) == INVALID_FILE_ATTRIBUTES)
		{
			MessageBoxW(L"文件删除成功！",L"A盾电脑防护",MB_ICONWARNING);
		}
	}
	//再继续保护自己的进程
	ReadFile((HANDLE)RESUME_PROTECT,0,0,&dwReadByte,0);
}
void CProtectSetting::OnBnClickedDisloaddriver()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("暴力结束进程"));
	DWORD dwReadByte;

	if (bDisLoadDriver == TRUE)
	{
		ReadFile((HANDLE)DIS_LOAD_DRIVER,0,0,&dwReadByte,0);
		bDisLoadDriver = FALSE;
		SetDlgItemText(IDC_DisLoadDriver,L"允许加载驱动");

	}else
	{
		ReadFile((HANDLE)LOAD_DRIVER,0,0,&dwReadByte,0);
		bDisLoadDriver = TRUE;
		SetDlgItemText(IDC_DisLoadDriver,L"禁止加载驱动");
	}
}
void CProtectSetting::OnBnClickedDiscreateprocess()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("暴力结束进程"));
	DWORD dwReadByte;

	if (bDisCreateProcess == TRUE)
	{
		ReadFile((HANDLE)DIS_CREATE_PROCESS,0,0,&dwReadByte,0);
		bDisCreateProcess = FALSE;
		SetDlgItemText(IDC_DisCreateProcess,L"允许创建进程");

	}else
	{
		ReadFile((HANDLE)CREATE_PROCESS,0,0,&dwReadByte,0);
		bDisCreateProcess = TRUE;
		SetDlgItemText(IDC_DisCreateProcess,L"禁止创建进程");
	}
}

void CProtectSetting::OnBnClickedDiscreatefile()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("暴力结束进程"));
	DWORD dwReadByte;

	if (bDisWriteFile == TRUE)
	{
		ReadFile((HANDLE)DIS_WRITE_FILE,0,0,&dwReadByte,0);
		bDisWriteFile = FALSE;
		SetDlgItemText(IDC_DisCreateFile,L"允许创建文件");

	}else
	{
		ReadFile((HANDLE)WRITE_FILE,0,0,&dwReadByte,0);
		bDisWriteFile = TRUE;
		SetDlgItemText(IDC_DisCreateFile,L"禁止创建文件");
	}
}

void CProtectSetting::OnBnClickedDissrvreset()
{
	// TODO: 在此添加控件通知处理程序代码
	//AfxMessageBox(_T("暴力结束进程"));
	DWORD dwReadByte;

	if (bDisResetServices == TRUE)
	{
		ReadFile((HANDLE)DIS_RESET_SRV,0,0,&dwReadByte,0);
		bDisResetServices = FALSE;
		SetDlgItemText(IDC_DisSrvReset,L"允许服务回写");

	}else
	{
		ReadFile((HANDLE)RESET_SRV,0,0,&dwReadByte,0);
		bDisResetServices = TRUE;
		SetDlgItemText(IDC_DisSrvReset,L"禁止服务回写");
	}
}
void CProtectSetting::OnBnClickedDiskernelthread()
{
	DWORD dwReadByte;

	if (bDisKernelThread == TRUE)
	{
		ReadFile((HANDLE)DIS_KERNEL_THREAD,0,0,&dwReadByte,0);
		bDisKernelThread = FALSE;
		SetDlgItemText(IDC_DisKernelThread,L"允许内核线程");

	}else
	{
		ReadFile((HANDLE)KERNEL_THREAD,0,0,&dwReadByte,0);
		bDisKernelThread = TRUE;
		SetDlgItemText(IDC_DisKernelThread,L"禁止内核线程");
	}
}
void CProtectSetting::OnBnClickedDisSetWindowsHook()
{
	DWORD dwReadByte;

	if (bDisSetWindowsHook == TRUE)
	{
		ReadFile((HANDLE)DIS_SET_WINDOWS_HOOK,0,0,&dwReadByte,0);
		bDisSetWindowsHook = FALSE;
		SetDlgItemText(IDC_DisUserHook,L"允许全局钩子");

	}else
	{
		ReadFile((HANDLE)SET_WINDOWS_HOOK,0,0,&dwReadByte,0);
		bDisSetWindowsHook = TRUE;
		SetDlgItemText(IDC_DisUserHook,L"禁止全局钩子");
	}
}
void CProtectSetting::OnBnClickedDisDllFuck()
{
	DWORD dwReadByte;

	if (bDisDllFuck == TRUE)
	{
		ReadFile((HANDLE)DIS_DLL_FUCK,0,0,&dwReadByte,0);
		bDisDllFuck = FALSE;
		SetDlgItemText(IDC_DisDllFuck,L"开启DLL挟持防护");

	}else
	{
		ReadFile((HANDLE)DLL_FUCK,0,0,&dwReadByte,0);
		bDisDllFuck = TRUE;
		SetDlgItemText(IDC_DisDllFuck,L"关闭DLL挟持防护");
	}
}

void CProtectSetting::OnBnClickedShutdownsystem()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwReadByte;
	ReadFile((HANDLE)SHUT_DOWN_SYSTEM,0,0,&dwReadByte,0);
}

void ReadDeskPath(LPCSTR DaskPath)
{
	CHAR path[255];
	ZeroMemory(path,255);
	SHGetSpecialFolderPathA(0,path,CSIDL_DESKTOPDIRECTORY,0);
	lstrcatA((char *)DaskPath,path);
	return;
}

void CProtectSetting::OnBnClickedUninstall360()
{
	// TODO: 在此添加控件通知处理程序代码
	DWORD dwReadByte;

	HKEY hkRoot2 = HKEY_LOCAL_MACHINE;
	HKEY hkRoot3 = HKEY_LOCAL_MACHINE;
	BOOL bFound360 = FALSE;

	if(RegOpenKeyExW(hkRoot2,L"SYSTEM\\CurrentControlSet\\Services\\zhudongfangyu", 0, KEY_QUERY_VALUE, &hkRoot2) == ERROR_SUCCESS ||
		RegOpenKeyExW(hkRoot3,L"SYSTEM\\CurrentControlSet\\Services\\360rp", 0, KEY_QUERY_VALUE, &hkRoot3) == ERROR_SUCCESS)
	{
		bFound360 = TRUE;
	}
	if (!bFound360)
	{
		MessageBoxW(L"恭喜，您系统上没有发现360，不需要一键卸载！",L"A盾电脑防护",MB_ICONINFORMATION);
		return;
	}
	RegCloseKey(hkRoot2);
	RegCloseKey(hkRoot3);

	if (MessageBoxW(L"“A盾电脑防护”虽然提供一键卸载360的功能，但是真正的卸载操作是由用户主观意识去点击“一键卸载360”来完成卸载。\r\n\r\nAProtect再次确定是否要一键卸载360？\r\n",L"A盾电脑防护",MB_ICONINFORMATION|MB_YESNO) == IDYES)
	{
		//开始卸载360
		SetDlgItemText(IDC_Uninstall360,L"正在卸载...");

		//去掉保护
		ReadFile((HANDLE)UNPROTECT_360SAFE,0,0,&dwReadByte,0);
		Uninstall360();

		//清理快捷方式
		char lpszDesk[260] = {0};
		char lpsz360lnk[260] = {0};

		memset(lpszDesk,0,260);
		memset(lpsz360lnk,0,260);
		ReadDeskPath(lpszDesk);

		wsprintfA(lpsz360lnk,"%s\\360安全卫士.lnk",lpszDesk);
		DeleteFileA(lpsz360lnk);

		memset(lpsz360lnk,0,260);
		wsprintfA(lpsz360lnk,"%s\\360软件管家.lnk",lpszDesk);
		DeleteFileA(lpsz360lnk);

		memset(lpsz360lnk,0,260);
		wsprintfA(lpsz360lnk,"%s\\360杀毒.lnk",lpszDesk);
		DeleteFileA(lpsz360lnk);

		char lpWinDir[260] = {0};
		char lpSysDisk[10] = {0};
		GetWindowsDirectoryA(lpWinDir,sizeof(lpWinDir));
		memcpy(lpSysDisk,lpWinDir,4);

		memset(lpsz360lnk,0,260);
		wsprintfA(lpsz360lnk,"%s\\Documents and Settings\\All Users\\桌面\\360杀毒.lnk",lpSysDisk);
		DeleteFileA(lpsz360lnk);

		memset(lpsz360lnk,0,260);
		wsprintfA(lpsz360lnk,"%s\\Documents and Settings\\All Users\\Desktop\\360杀毒.lnk",lpSysDisk);
		DeleteFileA(lpsz360lnk);

		SetDlgItemText(IDC_Uninstall360,L"一键卸载360");
		if (MessageBoxW(L"一键卸载完毕，残余的360文件将在下次系统开机时自动删除！\r\n\r\n是否马上重启？",L"A盾电脑防护",MB_ICONINFORMATION | MB_YESNO) == IDYES)
		{
			ReadFile((HANDLE)SHUT_DOWN_SYSTEM,0,0,&dwReadByte,0);
		}
	}
}
