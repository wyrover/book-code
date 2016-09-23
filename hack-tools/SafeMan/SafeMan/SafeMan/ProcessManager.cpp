// ProcessManager.cpp : 实现文件
//

#include "stdafx.h"
#include "SafeMan.h"
#include "ProcessManager.h"
#include "tlhelp32.h"
#include "shlwapi.h"
#include "psapi.h"
#pragma comment(lib, "Psapi.Lib")
#include "DriverManager.h"
#include <winioctl.h>
#include "Ioctls.h"

// CProcessManager 对话框

IMPLEMENT_DYNAMIC(CProcessManager, CDialog)

CProcessManager::CProcessManager(CWnd* pParent /*=NULL*/)
	: CDialog(CProcessManager::IDD, pParent)
{
	 if( CDriverManager::LoadNTDriver(_T("ProcessDriver"), _T("..\\ProcessDriver\\objchk\\i386\\ProcessDriver.sys")) == FALSE)
	 //if( CDriverManager::LoadNTDriver(_T("ProcessDriver"), _T("ProcessDriver.sys")) == FALSE)
	 {
		 MessageBox(_T("加载驱动失败！"));
		 CDriverManager::UnloadNTDriver(_T("ProcessDriver"));  // 卸载驱动 
	 }
}

CProcessManager::~CProcessManager()
{
	CDriverManager::UnloadNTDriver(_T("ProcessDriver"));  // 卸载驱动   
}

void CProcessManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_ListProcess);
	//DDX_Control(pDX, IDC_BUTTON_LISTPROCESS_RING3, m_ListProcessByRing3);
	//DDX_Control(pDX, IDC_BUTTON_LISTPROCESS_RING0, m_ListProcessByRing0);
}


BEGIN_MESSAGE_MAP(CProcessManager, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_LISTPROCESS_RING3, &CProcessManager::OnBnClickedButtonListprocessRing3)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, &CProcessManager::OnNMRclickListProcess)
	ON_BN_CLICKED(IDC_BUTTON_LISTPROCESS_RING0, &CProcessManager::OnBnClickedButtonPspcidtable)
	ON_COMMAND(ID_MENU_TERMINATE_PROCESS_RING0, &CProcessManager::OnMenuTerminateProcessRing0)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_PROCESS, &CProcessManager::OnNMCustomdrawListProcess)
	ON_BN_CLICKED(IDC_BUTTON_ACTIVE_PROCESS_LINKS, &CProcessManager::OnBnClickedButtonActiveProcessLinks)
	ON_BN_CLICKED(IDC_BUTTON_OBJECT_TABLE, &CProcessManager::OnBnClickedButtonObjectTable)
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CProcessManager 消息处理程序


// 初始化对话框
//
BOOL CProcessManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	// 进程列表框添加列
	m_ListProcess.InsertColumn(0, _T("进程名"), LVCFMT_LEFT, 155);
	m_ListProcess.InsertColumn(1, _T("PID"), LVCFMT_LEFT, 80);
	m_ListProcess.InsertColumn(2, _T("进程路径"), LVCFMT_LEFT, 315);

	m_ListProcess.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);  // 进程列表框每行全选且带网格线  

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 改变对话框和控件背景色
//
HBRUSH CProcessManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255,255,255));  // 设置控件背景色（白色）
		return m_brush;  // 返回自定义的画刷句柄
	}
	return hbr;  // 否则的话返回系统默认的画刷句柄
}


// Ring3列举进程按钮事件
//
void CProcessManager::OnBnClickedButtonListprocessRing3()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ListProcess.DeleteAllItems();  // 删除列表所有项

	HANDLE hSnapshotProcess = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);  // 建立快照，获取所有进程
	if (hSnapshotProcess == INVALID_HANDLE_VALUE)
	{
		return;
	}

	PROCESSENTRY32 processInfo;  // 进程信息结构体
	processInfo.dwSize = sizeof(PROCESSENTRY32);

	HANDLE hProcess;  // 进程句柄
	CString path;  // 进程完整路径
	CString processID;  // PID
	DWORD dwRet;  // 返回值

	INT i = 0;
	BOOL bRet = FALSE;
	// 遍历快照中的所有进程
	for (bRet = Process32First(hSnapshotProcess, &processInfo); bRet; bRet = Process32Next(hSnapshotProcess, &processInfo), i++)
	{
		AdjustPurview();  // 调整权限，主要是为了OpenProcess成功返回Handle
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.th32ProcessID);  // 根据进程ID打开进程，返回进程句柄

		dwRet =GetModuleFileNameEx(hProcess, NULL, path.GetBuffer(MAX_PATH), MAX_PATH);  // 通过进程句柄获得完整路径
		if(dwRet == 0)
		{
			path = _T("路径获取失败");
		}

		int iRow = m_ListProcess.InsertItem(i, processInfo.szExeFile);  // 进程的可执行文件名称
		processID.Format(_T("%u"), processInfo.th32ProcessID);  // DWORD转CString
		m_ListProcess.SetItemText(iRow, 1, processID);  // PID 
		m_ListProcess.SetItemText(iRow, 2, path);  // 进程完整路径 

		CloseHandle(hProcess);  // 关闭进程句柄
	}
}


// // 通过Pspcidtable列举进程
//
void CProcessManager::OnBnClickedButtonPspcidtable()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ListProcess.DeleteAllItems();  // 删除列表所有项

	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\ProcessDriver"), 
		GENERIC_WRITE | GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);  
	if(hDevice == INVALID_HANDLE_VALUE)  
	{
		MessageBox(_T("打开设备失败！"));  
		return;
	}

	// 获取进程数目
	DWORD dwProcessNumber = 0;
	DWORD dwDeviceRet = 0;
	BOOL bRet = DeviceIoControl(
		hDevice,       
		IOCTL_PROCESSDRIVER_PSPCIDTABLE_GETPROCESSNUMBER, 
		&dwProcessNumber,
		sizeof(DWORD),        
		&dwProcessNumber,                  
		sizeof(DWORD),                       
		&dwDeviceRet,  
		0);
	if(bRet == FALSE)
	{
		MessageBox(_T("获取进程数目失败！"));
		CloseHandle(hDevice);  // 关闭设备
		return;
	}

	// 获取进程信息
	PVOID pProcessInfo = (PVOID)new BYTE[dwProcessNumber * sizeof(ProcessInfo)];  // 分配指定大小的连续内存存放每一个进程对象信息
	memset(pProcessInfo, 0, dwProcessNumber * sizeof(ProcessInfo));  // 初始化内存
	bRet = DeviceIoControl(
		hDevice,       
		IOCTL_PROCESSDRIVER_PSPCIDTABLE_GETPROCESSINFO,               
		pProcessInfo,           
		dwProcessNumber*sizeof(ProcessInfo),        
		pProcessInfo,                         
		dwProcessNumber*sizeof(ProcessInfo),                         
		&dwDeviceRet,               
		0);
	if(bRet == FALSE)
	{
		MessageBox(_T("获取进程信息失败！"));
		CloseHandle(hDevice);  // 关闭设备
		return;
	}
                         
	CloseHandle(hDevice);  // 关闭设备


	HANDLE hProcess;  // 进程句柄
	CString path;  // 进程完整路径
	CString processID;  // PID
	DWORD dwRet;  // 返回值
	ProcessInfo processInfo;  // 进程信息结构体	
	for (DWORD i = 0; i < dwProcessNumber; i++)
	{
		processInfo = *((PProcessInfo)(pProcessInfo) + i);  // 从连续的内存区域中取出每个进程对象
		AdjustPurview();  // 调整权限，主要是为了OpenProcess成功返回Handle
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.pid);  // 根据进程ID打开进程，返回进程句柄

		dwRet = GetModuleFileNameEx(hProcess, NULL, path.GetBuffer(MAX_PATH), MAX_PATH);  // 通过进程句柄获得完整路径
		if(dwRet == 0)
		{
			path = _T("路径获取失败");
		}

		CString exeName(processInfo.name);  // 进程的可执行文件名称
		int iRow = m_ListProcess.InsertItem(i, exeName);
		processID.Format(_T("%u"), processInfo.pid);  // ULONG转CString
		m_ListProcess.SetItemText(iRow, 1, processID);  // PID  
		m_ListProcess.SetItemText(iRow, 2, path);  // 进程完整路径 

		CloseHandle(hProcess);  // 关闭进程句柄
	}

	delete pProcessInfo;  // 释放内存
}


// 列表框单击右键事件
//
void CProcessManager::OnNMRclickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码

	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
	if (pNMListView->iItem != -1 && pNMListView->iSubItem != -1)
	{ 
		CPoint pt;
		GetCursorPos(&pt);
		CMenu menu;
		menu.LoadMenu(IDR_MENU_PROCESS);
		CMenu* pMenu = menu.GetSubMenu(0);
		pMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON, pt.x, pt.y, this);
	}

	*pResult = 0;
}


// Ring0结束进程
//
void CProcessManager::OnMenuTerminateProcessRing0()
{
	// TODO: 在此添加命令处理程序代码

	int index = m_ListProcess.GetNextItem(-1, LVNI_SELECTED);  // 得到选择的项的行号
	CString strProcessID = m_ListProcess.GetItemText(index, 1);  // 根据行号和列号取得PID
	DWORD dwProcessID = _ttol(strProcessID);  // CString转DWORD 


	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\ProcessDriver"), 
		GENERIC_WRITE | GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);  
	if(hDevice == INVALID_HANDLE_VALUE)  
	{
		MessageBox(_T("打开设备失败！"));
		return;
	}

	// 结束进程
	DWORD dwDeviceRet = 0;
	BOOL bRet = DeviceIoControl(
		hDevice,      
		IOCTL_PROCESSDRIVER_TERMINATEPROCESS,         
		&dwProcessID,          
		sizeof(DWORD),         
		NULL,                          
		0,                             
		&dwDeviceRet,                
		0); 
	if(bRet == FALSE)
	{
		MessageBox(_T("结束进程失败！"));
	}

	CloseHandle(hDevice);  // 关闭设备	
}


// 调整权限，主要是为了OpenProcess成功返回Handle
//
BOOL CProcessManager::AdjustPurview()
{
	TOKEN_PRIVILEGES TokenPrivileges;
	bool bRet;
	HANDLE hToken;

	LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &TokenPrivileges.Privileges[0].Luid);   
	OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &hToken);

	TokenPrivileges.PrivilegeCount = 1;   
	TokenPrivileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

	bRet = !!AdjustTokenPrivileges(hToken, FALSE, &TokenPrivileges, 0, NULL, NULL);

	CloseHandle(hToken);
	return bRet ;
}


// 绘制列表
//
void CProcessManager::OnNMCustomdrawListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);
	*pResult = CDRF_DODEFAULT;

	if (CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (CDDS_ITEMPREPAINT == pLVCD->nmcd.dwDrawStage)
	{
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if ( (CDDS_ITEMPREPAINT | CDDS_SUBITEM) == pLVCD->nmcd.dwDrawStage )
	{
		COLORREF clrNewBkColor;

		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		// 设置背景色
		if (nItem%2 ==0)
		{
			clrNewBkColor = RGB(240, 240, 240);	// 偶数行背景色为灰色
		}
		else
		{
			clrNewBkColor = RGB(255, 255, 255);	// 奇数行背景色为白色
		}

		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}
}


// 通过ActiveProcessLinks列举进程
//
void CProcessManager::OnBnClickedButtonActiveProcessLinks()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ListProcess.DeleteAllItems();  // 删除列表所有项

	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\ProcessDriver"), 
		GENERIC_WRITE | GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);  
	if(hDevice == INVALID_HANDLE_VALUE)  
	{
		MessageBox(_T("打开设备失败！"));  
		return;
	}

	// 试探所需分配内存大小
	DWORD dwDeviceRet = 0;
	BOOL bRet = DeviceIoControl(
		hDevice,       
		IOCTL_PROCESSDRIVER_ACTIVEPROCESSLINKS,               
		NULL,           
		0,        
		NULL,                         
		0,                         
		&dwDeviceRet,               
		0);
	if (bRet == FALSE)
	{
		MessageBox(_T("获取进程信息失败！"));
		CloseHandle(hDevice);  // 关闭设备
		return;
	}

	PVOID pProcessInfo = (PVOID)new BYTE[dwDeviceRet];  // 分配指定大小的连续内存存放每一个进程对象信息
	memset(pProcessInfo, 0, dwDeviceRet);  // 初始化内存
	
	// 这次真的去取进程信息
	bRet = DeviceIoControl(
		hDevice,       
		IOCTL_PROCESSDRIVER_ACTIVEPROCESSLINKS,               
		pProcessInfo,           
		dwDeviceRet,        
		pProcessInfo,                         
		dwDeviceRet,                         
		&dwDeviceRet,               
		0);
	if (bRet == FALSE)
	{
		MessageBox(_T("获取进程信息失败！"));
		CloseHandle(hDevice);  // 关闭设备
		return;
	}

	CloseHandle(hDevice);  // 关闭设备

	HANDLE hProcess;  // 进程句柄
	CString path;  // 进程完整路径
	CString processID;  // PID
	DWORD dwRet;  // 返回值
	ProcessInfo processInfo;  // 进程信息结构体	
	for (DWORD i = 0; i < dwDeviceRet/sizeof(ProcessInfo); i++)
	{
		processInfo = *((PProcessInfo)(pProcessInfo) + i);  // 从连续的内存区域中取出每个进程对象
		AdjustPurview();  // 调整权限，主要是为了OpenProcess成功返回Handle
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.pid);  // 根据进程ID打开进程，返回进程句柄

		dwRet = GetModuleFileNameEx(hProcess, NULL, path.GetBuffer(MAX_PATH), MAX_PATH);  // 通过进程句柄获得完整路径
		if(dwRet == 0)
		{
			path = _T("路径获取失败");
		}

		CString exeName(processInfo.name);  // 进程的可执行文件名称
		int iRow = m_ListProcess.InsertItem(i, exeName);
		processID.Format(_T("%u"), processInfo.pid);  // ULONG转CString
		m_ListProcess.SetItemText(iRow, 1, processID);  // PID  
		m_ListProcess.SetItemText(iRow, 2, path);  // 进程完整路径 

		CloseHandle(hProcess);  // 关闭进程句柄
	}

	delete pProcessInfo;  // 释放内存
}


// 通过ObjectTable列举进程
//
void CProcessManager::OnBnClickedButtonObjectTable()
{
	// TODO: 在此添加控件通知处理程序代码

	m_ListProcess.DeleteAllItems();  // 删除列表所有项

	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\ProcessDriver"), 
		GENERIC_WRITE | GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);  
	if(hDevice == INVALID_HANDLE_VALUE)  
	{
		MessageBox(_T("打开设备失败！"));  
		return;
	}

	// 试探所需分配内存大小
	DWORD dwDeviceRet = 0;
	BOOL bRet = DeviceIoControl(
		hDevice,       
		IOCTL_PROCESSDRIVER_OBJECTTABLE,               
		NULL,           
		0,        
		NULL,                         
		0,                         
		&dwDeviceRet,               
		0);
	if (bRet == FALSE)
	{
		MessageBox(_T("获取进程信息失败！"));
		CloseHandle(hDevice);  // 关闭设备
		return;
	}

	PVOID pProcessInfo = (PVOID)new BYTE[dwDeviceRet];  // 分配指定大小的连续内存存放每一个进程对象信息
	memset(pProcessInfo, 0, dwDeviceRet);  // 初始化内存

	// 这次真的去取进程信息
	bRet = DeviceIoControl(
		hDevice,       
		IOCTL_PROCESSDRIVER_OBJECTTABLE,               
		pProcessInfo,           
		dwDeviceRet,        
		pProcessInfo,                         
		dwDeviceRet,                         
		&dwDeviceRet,               
		0);
	if (bRet == FALSE)
	{
		MessageBox(_T("获取进程信息失败！"));
		CloseHandle(hDevice);  // 关闭设备
		return;
	}

	CloseHandle(hDevice);  // 关闭设备

	HANDLE hProcess;  // 进程句柄
	CString path;  // 进程完整路径
	CString processID;  // PID
	DWORD dwRet;  // 返回值
	ProcessInfo processInfo;  // 进程信息结构体	
	for (DWORD i = 0; i < dwDeviceRet/sizeof(ProcessInfo); i++)
	{
		processInfo = *((PProcessInfo)(pProcessInfo) + i);  // 从连续的内存区域中取出每个进程对象
		AdjustPurview();  // 调整权限，主要是为了OpenProcess成功返回Handle
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, processInfo.pid);  // 根据进程ID打开进程，返回进程句柄

		dwRet = GetModuleFileNameEx(hProcess, NULL, path.GetBuffer(MAX_PATH), MAX_PATH);  // 通过进程句柄获得完整路径
		if(dwRet == 0)
		{
			path = _T("路径获取失败");
		}

		CString exeName(processInfo.name);  // 进程的可执行文件名称
		int iRow = m_ListProcess.InsertItem(i, exeName);
		processID.Format(_T("%u"), processInfo.pid);  // ULONG转CString
		m_ListProcess.SetItemText(iRow, 1, processID);  // PID  
		m_ListProcess.SetItemText(iRow, 2, path);  // 进程完整路径 

		CloseHandle(hProcess);  // 关闭进程句柄
	}

	delete pProcessInfo;  // 释放内存
}


// 设置鼠标样式
//
BOOL CProcessManager::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CString strClassName;
	GetClassName(pWnd->GetSafeHwnd(), strClassName.GetBuffer(80), 80);

	if (strClassName == "Button")  // 按钮类型
	{
		SetCursor( LoadCursor(NULL, IDC_HAND) );  // 鼠标样式为手型
		return TRUE;
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}
