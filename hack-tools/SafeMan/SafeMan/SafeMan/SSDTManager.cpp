// SSDTManager.cpp : 实现文件
//

#include "stdafx.h"
#include "SafeMan.h"
#include "SSDTManager.h"
#include "DriverManager.h"
#include <winioctl.h>
#include "Ioctls.h"

// CSSDTManager 对话框

IMPLEMENT_DYNAMIC(CSSDTManager, CDialog)

CSSDTManager::CSSDTManager(CWnd* pParent /*=NULL*/)
	: CDialog(CSSDTManager::IDD, pParent)
{
	if( CDriverManager::LoadNTDriver(_T("SSDTDriver"), _T("..\\SSDTDriver\\objchk\\i386\\SSDTDriver.sys")) == FALSE)
	//if( CDriverManager::LoadNTDriver(_T("SSDTDriver"), _T("SSDTDriver.sys")) == FALSE)
	{
		MessageBox(_T("加载驱动失败！"));
		CDriverManager::UnloadNTDriver(_T("SSDTDriver"));  // 卸载驱动 
	}
}

CSSDTManager::~CSSDTManager()
{
	CDriverManager::UnloadNTDriver(_T("SSDTDriver"));  // 卸载驱动 
}

void CSSDTManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SSDT, m_ListSSDT);
	DDX_Control(pDX, IDC_BUTTON_SSDT_REPAIR_SELECTED, m_ButRepairSel);
}


BEGIN_MESSAGE_MAP(CSSDTManager, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_BUTTON_SSDT_REPAIR_SELECTED, &CSSDTManager::OnBnClickedButtonSsdtRepairSelected)
	ON_STN_CLICKED(IDC_STATIC_CHECK, &CSSDTManager::OnStnClickedStaticCheck)
	ON_STN_CLICKED(IDC_STATIC_UNCHECK, &CSSDTManager::OnStnClickedStaticUncheck)
	ON_WM_SETCURSOR()
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST_SSDT, &CSSDTManager::OnNMCustomdrawListSsdt)
	ON_BN_CLICKED(IDC_BUTTON_SSDT_REFRESH, &CSSDTManager::OnBnClickedButtonSsdtRefresh)
END_MESSAGE_MAP()


// CSSDTManager 消息处理程序


// 初始化对话框
//
BOOL CSSDTManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	// SSDT列表框添加列
	m_ListSSDT.InsertColumn(0, _T("服务号"), LVCFMT_LEFT, 80);
	m_ListSSDT.InsertColumn(1, _T("当前地址"), LVCFMT_LEFT, 80);
	m_ListSSDT.InsertColumn(2, _T("原始地址"), LVCFMT_LEFT, 80);
	m_ListSSDT.InsertColumn(3, _T("服务函数名"), LVCFMT_LEFT, 200);
	m_ListSSDT.InsertColumn(4, _T("模块名"), LVCFMT_LEFT, 100);

	// SSDT列表框每行全选且带网格线和选择框
	m_ListSSDT.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_CHECKBOXES);  

	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	EnumSSDT();  // 遍历SSDT

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


// 改变对话框和控件背景色
//
HBRUSH CSSDTManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	//return hbr;
	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255,255,255));  // 设置控件背景色（白色）

		if (pWnd->GetDlgCtrlID() == IDC_STATIC_CHECK || pWnd->GetDlgCtrlID() == IDC_STATIC_UNCHECK)
		{
			pDC->SetTextColor(RGB(165,205,246));
		}
		return m_brush;  // 返回自定义的画刷句柄
	}
	return hbr;  // 否则的话返回系统默认的画刷句柄
}


// 遍历SSDT
//
VOID CSSDTManager::EnumSSDT()
{
	m_ListSSDT.DeleteAllItems();  // 删除列表所有项

	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\SSDTDriver"), 
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

	HMODULE hNtDll = GetModuleHandle( _T("ntdll.dll"));  // 获得模块句柄

	DWORD dwOffset = (DWORD)hNtDll;

	dwOffset += ( (PIMAGE_DOS_HEADER)hNtDll )->e_lfanew + sizeof(DWORD);  // Image_File_Header
	dwOffset += sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) - 
		IMAGE_NUMBEROF_DIRECTORY_ENTRIES * sizeof(IMAGE_DATA_DIRECTORY);  // 第一个数据目录
	dwOffset = (DWORD)hNtDll + ( (PIMAGE_DATA_DIRECTORY)dwOffset )->VirtualAddress;  // 导出表
	PIMAGE_EXPORT_DIRECTORY pExpDir = (PIMAGE_EXPORT_DIRECTORY)dwOffset;

	DWORD numOfNames = pExpDir->NumberOfNames;  // 有名函数的总数

	PDWORD pNameArray = (PDWORD)( (DWORD)hNtDll + pExpDir->AddressOfNames );  // FNT  

	PSYSTEM_MODULE_INFORMATION pSysModInfo = GetModuleList(hNtDll);  // 得到系统模块

	CString funName;  // 函数名
	USHORT rowCount = 0;  // 列表框行数
	for (UINT i = 0; i < numOfNames; i++)
	{
		funName = CString((PCSTR)( pNameArray[i] + (DWORD)hNtDll ));  // ANSI???

		if ( funName.GetAt(0) == _T('N') && funName.GetAt(1) == _T('t') )  // 函数名开头两个字符是Nt
		{
			// FOT
			PWORD pOrdNameArray = (PWORD)( (DWORD)hNtDll + pExpDir->AddressOfNameOrdinals );
			// FAT
			PDWORD pFunArray = (PDWORD)( (DWORD)hNtDll + pExpDir->AddressOfFunctions );

			PVOID pFun = (PVOID)( (DWORD)hNtDll + pFunArray[ pOrdNameArray[i] ] );

			SSDTENTRY entry;
			CopyMemory( &entry, pFun, sizeof(SSDTENTRY) );
			if (entry.byMov == MOV)
			{
				ULONG ulAddr;
				GetFunAddrByIndex(hDevice, entry.dwIndex, &ulAddr);  // 通过索引得到函数地址

				CString strModule = GetModuleByAddr(ulAddr, pSysModInfo);  // 通过地址找到所属模块

				CString strIndex;
				strIndex.Format(_T("0x%04X"), entry.dwIndex);  // DWORD转CString

				CString strAddr;
				strAddr.Format(_T("0x%08X"), ulAddr);  // ULONG转CString

				int iRow = m_ListSSDT.InsertItem(entry.dwIndex, strIndex);  
				m_ListSSDT.SetItemText(iRow, 1, strAddr);  
				m_ListSSDT.SetItemText(iRow, 3, funName);
				m_ListSSDT.SetItemText(iRow, 4, strModule);

				rowCount++;
			}
		}
	}

	GetOrigFunAddr();  // 得到服务函数的原始地址

	delete pSysModInfo;  // 删除模块列表
	CloseHandle(hDevice);  // 关闭设备
}


// 得到SSDT
//
BOOL CSSDTManager::GetSSDT(IN HANDLE hDevice, OUT PSSDT pSSDT)
{
	DWORD dwDeviceRet = 0;

	BOOL bRet = DeviceIoControl(
		hDevice, 
		IOCTL_SSDTDRIVER_GETSSDT, 
		NULL, 
		0, 
		pSSDT,
		sizeof(SSDT), 
		&dwDeviceRet, 
		0);

	return bRet;
}


// 通过索引得到函数地址
//
BOOL CSSDTManager::GetFunAddrByIndex(IN HANDLE hDevice, IN ULONG ulIndex, OUT PULONG pFunAddr)
{
    DWORD dwDeviceRet = 0;

    BOOL bRet = DeviceIoControl( 
		hDevice, 
		IOCTL_SSDTDRIVER_GETFUNADDRBYINDEX, 
		&ulIndex,
        sizeof(ULONG), 
		pFunAddr, 
		sizeof(ULONG), 
		&dwDeviceRet, 
		0);

    return bRet;
}


// 得到系统模块
//
PSYSTEM_MODULE_INFORMATION CSSDTManager::GetModuleList(IN HMODULE hNtDll)
{
	// 函数指针类型定义
	typedef DWORD (WINAPI * NTQUERYSYSTEMINFORMATION)(
		SYSTEM_INFORMATION_CLASS, 
		LPVOID, 
		DWORD,
		PULONG);

	 // 检索指定的动态链接库(DLL)中的输出库函数地址
	NTQUERYSYSTEMINFORMATION NtQuerySystemInformation =
		(NTQUERYSYSTEMINFORMATION)GetProcAddress(hNtDll, "NtQuerySystemInformation"); 

	ULONG size;

	// 试探一下所需内存大小，返回需要size大小
	NtQuerySystemInformation(SystemModuleInformation, &size, 0, &size); 

	// 分配size个字节的大小
	PSYSTEM_MODULE_INFORMATION pSysModInfo = (PSYSTEM_MODULE_INFORMATION)new BYTE[size];
	
	// 这次真的去取信息
	NtQuerySystemInformation(SystemModuleInformation, pSysModInfo, size, 0); 

	return pSysModInfo;
} 


// 通过地址找到所属模块
//
CString CSSDTManager::GetModuleByAddr(IN ULONG ulAddr, IN PSYSTEM_MODULE_INFORMATION pSysModInfo)
{
	for (UINT i = 0; i < pSysModInfo->Count; i++)
	{
		ULONG ulBase = (ULONG)pSysModInfo->Module[i].Base;
		ULONG ulMax  = ulBase + pSysModInfo->Module[i].Size;
		if (ulBase <= ulAddr && ulAddr < ulMax)
		{
			return CString(pSysModInfo->Module[i].ImageName + pSysModInfo->Module[i].PathLength);
		}
	}
	return NULL;
}


// 得到服务函数的原始地址
//
VOID CSSDTManager::GetOrigFunAddr()
{
	PSYSTEM_MODULE_INFORMATION pSysModInfo =   
		GetModuleList(GetModuleHandle( _T("ntdll.dll")));  // 得到系统模块  

	// 重新加载内核文件(ntkrnlpa.exe)，不载入其他相关模块
	HMODULE hNtDll = LoadLibraryExA(
		pSysModInfo->Module[0].ImageName + pSysModInfo->Module[0].PathLength,   // 系统模块里第一个就是内核文件
		NULL, 
		DONT_RESOLVE_DLL_REFERENCES);
	if (hNtDll == NULL)
	{
		return;
	}

	DWORD dwKSDT = (DWORD)GetProcAddress(hNtDll, "KeServiceDescriptorTable");  // 在内核文件中查找KeServiceDescriptorTable变量地址 
	if (dwKSDT == 0)
	{
		return;
	}
 
	dwKSDT -= (DWORD)hNtDll;  // 获取KeServiceDescriptorTable变量的RVA 

	DWORD dwKiServiceTable = GetKiServiceTable(hNtDll, dwKSDT);  // 得到KiServiceTable的RVA
	if (dwKiServiceTable == 0)
	{
		return;
	}

	PIMAGE_FILE_HEADER pfh = NULL;
	PIMAGE_OPTIONAL_HEADER poh = NULL;
	PIMAGE_SECTION_HEADER psh = NULL;
	GetPEHeaders(hNtDll, pfh, poh, psh);  // 得到PE文件里的各种头

	DWORD dwKernelBase = (DWORD)pSysModInfo->Module[0].Base;  // ntkrnlpa.exe加载基址

	ULONG ulAddr = 0;
	CString strAddr;
	DWORD dwIndex = 0;
	for (PDWORD pService = (PDWORD)( (DWORD)hNtDll + dwKiServiceTable ); 
		*pService - poh->ImageBase < poh->SizeOfImage; 
		pService++, dwIndex++)
	{
		ulAddr = *pService - poh->ImageBase + dwKernelBase;  // 服务函数的原始地址
		strAddr.Format(_T("0x%08X"), ulAddr);  // ULONG转CString
		m_ListSSDT.SetItemText(dwIndex, 2, strAddr);
	}

	FreeLibrary(hNtDll);  // 与LoadLibraryExA对应
	delete pSysModInfo;
}


// 得到KiServiceTable的RVA
//
DWORD CSSDTManager::GetKiServiceTable(HMODULE hModule, DWORD dwKSDT) 
{ 
	PIMAGE_FILE_HEADER pfh = NULL; 
	PIMAGE_OPTIONAL_HEADER poh = NULL; 
	PIMAGE_SECTION_HEADER psh = NULL; 
	
	GetPEHeaders(hModule, pfh, poh, psh);  // 得到PE文件里的各种头 

	if ( (poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) && 
		!((pfh->Characteristics)&IMAGE_FILE_RELOCS_STRIPPED) )  // 判断是否存在重定位信息
	{ 
		// 第一个IMAGE_BASE_RELOCATION
		PIMAGE_BASE_RELOCATION pbr = 
			(PIMAGE_BASE_RELOCATION)(
			(DWORD)(poh->DataDirectory[IMAGE_DIRECTORY_ENTRY_BASERELOC].VirtualAddress) + (DWORD)hModule); 

		BOOL bFirstChunk = TRUE; 
		PIMAGE_FIXUP_ENTRY pfe = NULL; 
		// 遍历IMAGE_BASE_RELOCATION，ntoskrnl中第一个IMAGE_BASE_RELOCATION.VirtualAddress是0
		while (bFirstChunk || pbr->VirtualAddress) 
		{
			bFirstChunk = FALSE; 

			pfe = (PIMAGE_FIXUP_ENTRY)( (DWORD)pbr + sizeof(IMAGE_BASE_RELOCATION) );  // IMAGE_BASE_RELOCATION.TypeOffset[i](每项2字节) 

			// 遍历IMAGE_BASE_RELOCATION.TypeOffset[i]
			for (DWORD i = 0; i < ( pbr->SizeOfBlock - sizeof(IMAGE_BASE_RELOCATION) )>>1; i++, pfe++)  
			{ 
				if (pfe->type == IMAGE_REL_BASED_HIGHLOW) 
				{ 
					DWORD dwPointerRva = pbr->VirtualAddress + pfe->offset;  // 指向需要修改的地址数据 
					DWORD dwPointsToRva = *(PDWORD)( (DWORD)hModule + dwPointerRva ) - (DWORD)poh->ImageBase; 

					if (dwPointsToRva == dwKSDT)  // 1.判断是否为KeServiceDescriptorTable
					{ 
						// "mov ds:_KeServiceDescriptorTable.Base, offset _KiServiceTable" from the KiInitSystem.
						if ( *(PWORD)( (DWORD)hModule + dwPointerRva - 2 ) == 0x05C7 )  // 2.判断是否为MOV指令
						{ 
							// 求出并返回KiServiceTable的RVA
							DWORD dwKiServiceTable = *(PDWORD)( (DWORD)hModule + dwPointerRva + 4 ) - poh->ImageBase; 
							return dwKiServiceTable; 
						} 
					} 
				}  
			} 

			pbr = (PIMAGE_BASE_RELOCATION)( (DWORD)pbr + pbr->SizeOfBlock );  // 下一个IMAGE_BASE_RELOCATION
		} 
	}     

	return 0; 
} 


// 得到PE文件里的各种头
//
BOOL CSSDTManager::GetPEHeaders(HMODULE hModule, PIMAGE_FILE_HEADER& pfh,
				 PIMAGE_OPTIONAL_HEADER& poh, PIMAGE_SECTION_HEADER& psh)
{
	PIMAGE_DOS_HEADER pdh = (PIMAGE_DOS_HEADER)hModule;
	if (pdh->e_magic != IMAGE_DOS_SIGNATURE)  // 标志位 
	{
		return FALSE;
	}

	PIMAGE_NT_HEADERS pnh = (PIMAGE_NT_HEADERS)( (DWORD)hModule + pdh->e_lfanew );
	if (pnh->Signature != IMAGE_NT_SIGNATURE)  // 标志位  
	{
		return FALSE;
	}

	pfh = (PIMAGE_FILE_HEADER)( &(pnh->FileHeader) );

	poh = (PIMAGE_OPTIONAL_HEADER)( &(pnh->OptionalHeader) );
	if (poh->Magic != IMAGE_NT_OPTIONAL_HDR32_MAGIC)  // 标志位 
	{
		return FALSE;
	}

	psh = (PIMAGE_SECTION_HEADER)( (PBYTE)poh + sizeof(IMAGE_OPTIONAL_HEADER) );

	return TRUE;
}


// SSDT修复选中项按钮事件
//
void CSSDTManager::OnBnClickedButtonSsdtRepairSelected()
{
	// TODO: 在此添加控件通知处理程序代码

	// 查看用户有没有选择一项
	for (int i = 0; i < m_ListSSDT.GetItemCount(); i++)
	{
		if (m_ListSSDT.GetCheck(i) == TRUE)
		{
			break;
		}
		if (i == (m_ListSSDT.GetItemCount() - 1) && m_ListSSDT.GetCheck(i) == FALSE)
		{
			MessageBox(_T("必须选择一项进行修复！"));
			return;
		}
	}

	m_ButRepairSel.EnableWindow(FALSE);  // 修复选中项按钮无效

	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\SSDTDriver"), 
		GENERIC_WRITE | GENERIC_READ, 
		0, 
		NULL, 
		OPEN_EXISTING, 
		0, 
		NULL);  
	if (hDevice == INVALID_HANDLE_VALUE)  
	{
		MessageBox(_T("打开设备失败！"));  
		return;
	}

	// 遍历列表每一行
	for (int i = 0; i < m_ListSSDT.GetItemCount(); i++)
	{
		// 选择框选中并且前后地址不一样
		if ( m_ListSSDT.GetCheck(i) == TRUE && 
			m_ListSSDT.GetItemText(i, 1) != m_ListSSDT.GetItemText(i, 2) )
		{
			if (RestoreSSDTItem(hDevice, i) == FALSE)
			{
				MessageBox(_T("修复失败！"));
			}
		}
	}

	CloseHandle(hDevice);  // 关闭设备

	EnumSSDT();  // 遍历SSDT

	m_ButRepairSel.EnableWindow(TRUE);  // 修复选中项按钮有效
}


// 重置SSDT项
//
BOOL CSSDTManager::RestoreSSDTItem(IN HANDLE hDevice, IN ULONG ulIndex)
{
	CString strAddr = m_ListSSDT.GetItemText(ulIndex, 2);
	DWORD dwAddr;
	_stscanf_s(strAddr.Mid(2), _T("%x"), &dwAddr);  // CString转DWORD

	RESTORE_INFO restoreInfo;
	restoreInfo.ulIndex = ulIndex;  // 服务号
	restoreInfo.ulAddr = dwAddr;  // 服务函数的真实地址

	// 恢复SSDT项
	DWORD dwDeviceRet = 0;
	BOOL bRet = DeviceIoControl(
		hDevice,
		IOCTL_SSDTDRIVER_RESTOREFUNADDR, 
		&restoreInfo, 
		sizeof(restoreInfo), 
		NULL, 
		0, 
		&dwDeviceRet, 
		NULL);

	return bRet;
}


// 全选按钮事件(Notify属性要选上)
//
void CSSDTManager::OnStnClickedStaticCheck()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_ListSSDT.GetItemCount(); i++)
	{
		m_ListSSDT.SetCheck(i);  // 选择框打勾
	}
}


// 全不选按钮事件(Notify属性要选上)
//
void CSSDTManager::OnStnClickedStaticUncheck()
{
	// TODO: 在此添加控件通知处理程序代码
	for (int i = 0; i < m_ListSSDT.GetItemCount(); i++)
	{
		m_ListSSDT.SetCheck(i, FALSE);  // 选择框不打勾
	}
}


// 设置鼠标样式
//
BOOL CSSDTManager::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString strClassName;
	GetClassName(pWnd->GetSafeHwnd(), strClassName.GetBuffer(80), 80);

	if (strClassName == "Static" || strClassName == "Button")  // 静态控件或按钮类型
	{
		SetCursor( LoadCursor(NULL, IDC_HAND) );  // 鼠标样式为手型
		return TRUE;
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


// 绘制列表
//
void CSSDTManager::OnNMCustomdrawListSsdt(NMHDR *pNMHDR, LRESULT *pResult)
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
		COLORREF clrNewTextColor, clrNewBkColor;

		int nItem = static_cast<int>(pLVCD->nmcd.dwItemSpec);

		// 如果当前SSDT地址与原始地址不一样,则显示字体为红色,否则为黑色
		if ( m_ListSSDT.GetItemText(nItem, 1) != m_ListSSDT.GetItemText(nItem, 2) )		
		{
			clrNewTextColor = RGB(255, 0, 0);  // 设置为红色
		}
		else		
		{
			clrNewTextColor = RGB(0, 0, 0);  // 设置为黑色
		}

		// 设置背景色
		if (nItem%2 ==0)
		{
			clrNewBkColor = RGB(240, 240, 240);	// 偶数行背景色为灰色
		}
		else
		{
			clrNewBkColor = RGB(255, 255, 255);	// 奇数行背景色为白色
		}

		pLVCD->clrText = clrNewTextColor;
		pLVCD->clrTextBk = clrNewBkColor;

		*pResult = CDRF_DODEFAULT;
	}
}


// 刷新按钮事件
//
void CSSDTManager::OnBnClickedButtonSsdtRefresh()
{
	// TODO: 在此添加控件通知处理程序代码
	EnumSSDT();  // 遍历SSDT
}
