// MonitorManager.cpp : 实现文件
//

#include "stdafx.h"
#include "SafeMan.h"
#include "MonitorManager.h"
#include "DriverManager.h"
#include <winioctl.h>
#include "Ioctls.h"

// CMonitorManager 对话框
char static chInteractionBuffer[264];			//充当与内核进行交互的公用缓冲区
DWORD static dwControlBuffer[2];	//dwControlBuffer[0]中存放ZwCreateProcessEx函数的地址
												//dwControlBuffer[1]中存放chInteractionBuffer数组的起始地址
void static ThreadMonitor(LPVOID lpParam);

struct ThreadInfo
{
	CStatic *p_Count;
};

int iCount = 0;	//计数

IMPLEMENT_DYNAMIC(CMonitorManager, CDialog)

CMonitorManager::CMonitorManager(CWnd* pParent /*=NULL*/)
	: CDialog(CMonitorManager::IDD, pParent)
{
	if( CDriverManager::LoadNTDriver(_T("MONITORDriver"), _T("..\\MonitorDriver\\objchk\\i386\\MonitorDriver.sys")) == FALSE)
	//if( CDriverManager::LoadNTDriver(_T("MONITORDriver"), _T("MonitorDriver.sys")) == FALSE)
	{
		MessageBox(_T("加载驱动失败！"));
		CDriverManager::UnloadNTDriver(_T("MONITORDriver"));  // 卸载驱动 
	}

	//初始化
	m_pProcessMonitor = 0;
	DWORD RealZwCreateProcessAddress = GetZwCreateProcessAddress();		//得到实际地址
	ZeroMemory(dwControlBuffer,2);
	dwControlBuffer[0] = RealZwCreateProcessAddress;		//存放在dwControlBuffer[0]
	ZeroMemory(chInteractionBuffer,264);		//清空
	dwControlBuffer[1] = (DWORD)&chInteractionBuffer[0];	//将首地址存放在dwControlBuffer[1]中

	ThreadInfo *m_ThreadInfo = new ThreadInfo();
	m_ThreadInfo->p_Count = &m_Count;

	DWORD dwThreadId;
	//创建交互线程
	::CreateThread(	0,
							0,
							(LPTHREAD_START_ROUTINE)ThreadMonitor,
							m_ThreadInfo,
							0,
							&dwThreadId);
}

CMonitorManager::~CMonitorManager()
{
	CDriverManager::UnloadNTDriver(_T("MONITORDriver"));  // 卸载驱动
}

void CMonitorManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROCESSMONITORCONTROL, m_ButtonProcessMonitor);
	DDX_Control(pDX, IDC_ACCUMULATION, m_Count);
	DDX_Control(pDX, IDC_STATUS, m_Status);
}


BEGIN_MESSAGE_MAP(CMonitorManager, CDialog)
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_PROCESSMONITORCONTROL, &CMonitorManager::OnBnClickedProcessmonitorcontrol)
END_MESSAGE_MAP()


// CMonitorManager 消息处理程序

HBRUSH CMonitorManager::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	if(nCtlColor == CTLCOLOR_DLG || nCtlColor == CTLCOLOR_STATIC)
	{
		pDC->SetBkColor(RGB(255,255,255));  // 设置控件背景色（白色）

		if (pWnd->GetDlgCtrlID() == IDC_ACCUMULATION)
		{
			pDC->SetTextColor(RGB(255,0,0));
		}

		return m_brush;  // 返回自定义的画刷句柄
	}
	return hbr;
}

BOOL CMonitorManager::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_brush.CreateSolidBrush(RGB(255,255,255));  // 创建白色画刷

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CMonitorManager::OnBnClickedProcessmonitorcontrol()
{
	// TODO: 在此添加控件通知处理程序代码
	// 打开设备
	HANDLE hDevice = CreateFile(
		_T("\\\\.\\MONITORDriver"), 
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

	DWORD dwDeviceRet = 0;
	//开启进程监控
	if (m_pProcessMonitor == 0)
	{
		ULONG input;
		
		DWORD RealZwCreateProcessAddress = GetZwCreateProcessAddress();
		BOOL bRet = DeviceIoControl(
			hDevice,
			IOCTL_PROCESS_MONITORDRIVER_ON, 
			dwControlBuffer, 
			8, 
			dwControlBuffer, 
			8, 
			&dwDeviceRet, 
			NULL);
		m_pProcessMonitor = 1;
		CString close("关闭");
		m_ButtonProcessMonitor.SetWindowText(close);
		CString text("已开启！");
		m_Status.SetWindowText(text);

		//LOGFONT lf;                        // Used to create the CFont.
		//memset(&lf, 0, sizeof(LOGFONT));   // Clear out structure.
		//lf.lfHeight = 20;   // Request a 20-pixel-high font
		//strcpy(lf.lfFaceName, "Arial");    //    with face name "Arial".
		//CFont m_font;
		//m_font.CreateFontIndirect(&lf);    // Create the font.

		//m_Status.SetFont(&m_font);
	} 
	else
	{
		ULONG input;
		DWORD RealZwCreateProcessAddress = GetZwCreateProcessAddress();
		BOOL bRet = DeviceIoControl(
			hDevice,
			IOCTL_PROCESS_MONITORDRIVER_OFF, 
			&dwControlBuffer,
			sizeof(dwControlBuffer), 
			&dwControlBuffer, 
			sizeof(dwControlBuffer), 
			&dwDeviceRet, 
			NULL);
		m_pProcessMonitor = 0;
		CString open("开启");
		m_ButtonProcessMonitor.SetWindowText(open);
		CString text("尚未开启，建议开启！");
		m_Status.SetWindowText(text);

	}
	
	CloseHandle(hDevice);  // 关闭设备
}

// 获取ZwCreateProcessEx的真实地址
DWORD CMonitorManager::GetZwCreateProcessAddress(void)
{
	HMODULE hNtDll = GetModuleHandle( _T("ntdll.dll"));  // 获得模块句柄

	DWORD dwOffset = (DWORD)hNtDll;
	
	dwOffset += ( (PIMAGE_DOS_HEADER)hNtDll )->e_lfanew + sizeof(DWORD);  // Image_File_Header
	dwOffset += sizeof(IMAGE_FILE_HEADER) + sizeof(IMAGE_OPTIONAL_HEADER) - 
		IMAGE_NUMBEROF_DIRECTORY_ENTRIES * sizeof(IMAGE_DATA_DIRECTORY);  // 第一个数据目录
	dwOffset = (DWORD)hNtDll + ( (PIMAGE_DATA_DIRECTORY)dwOffset )->VirtualAddress;  // 导出表
	PIMAGE_EXPORT_DIRECTORY pExpDir = (PIMAGE_EXPORT_DIRECTORY)dwOffset;

	DWORD pNumberOfNames = pExpDir->NumberOfNames;  // 有名函数的总数

	PDWORD pAddressOfNames = (PDWORD)( (DWORD)hNtDll + pExpDir->AddressOfNames );  // FNT  

	// FOT
	PWORD arrayOfFunctionOrdinals = (PWORD)( (DWORD)hNtDll + pExpDir->AddressOfNameOrdinals );
	// FAT
	PDWORD arrayOfFunctionAddresses = (PDWORD)( (DWORD)hNtDll + pExpDir->AddressOfFunctions );

	CString funName;
	CString functionName("ZwCreateProcessEx");
	DWORD Base = pExpDir->Base;

	for (UINT i = 0; i < pNumberOfNames; i++)
	{
		funName = CString((PCSTR)( pAddressOfNames[i] + (DWORD)hNtDll ));

		if (funName.Compare(functionName) == 0)
		{
			WORD functionOrdinal = arrayOfFunctionOrdinals[i] + Base - 1;
			DWORD functionAddress = (DWORD)( (BYTE*)hNtDll + arrayOfFunctionAddresses[functionOrdinal]);
			
			return functionAddress;
		}
	}
	return 0;
}

// 用户态与内核态之间的线程交互
void static ThreadMonitor(LPVOID lpParam)
{
	DWORD dwProcessMonitor = 0;		//进程监控开关与否
	DWORD dwGoOrNot = 0;				//是否允许进程执行
	char chMessageBuffer[512] = {0};

	while(1)
	{
		memmove(&dwProcessMonitor, &chInteractionBuffer[0],4);

		if (!dwProcessMonitor)		//进程监控关闭
		{
			Sleep(10);		//单位，毫秒
			continue;		//跳出去继续执行循环
		}
		
		char *message = (char *)&chInteractionBuffer[8];		//获取进程相关信息
		char *positon = strstr(message, "##");						//返回message中“##”第一次出现的位置
		if (positon != NULL)
		{
			int iIntdex = positon - message;		//提取长度
			strcpy(chMessageBuffer, "是否允许");
			strncat(chMessageBuffer, &chInteractionBuffer[8], iIntdex);		//strncat可以限定长度
			strcat(chMessageBuffer, "运行");
			strcat(chMessageBuffer, &chInteractionBuffer[iIntdex+10]);    //????+10
		}

		ThreadInfo *m_ThreadInfo = (ThreadInfo *)lpParam;
		
		if(IDYES == MessageBoxA(0, chMessageBuffer, "WARNING", MB_YESNO|MB_ICONQUESTION|0x00200000L))
		{
			dwGoOrNot = 1; //允许进程执行
		}
		else 
		{
			dwGoOrNot = 0;//不允许进程执行
			//计数拦截进程
			iCount++;
			CString csCount;
			csCount.Format(TEXT("%d"), iCount);
			m_ThreadInfo->p_Count->SetWindowText(csCount);
		}

		memmove(&chInteractionBuffer[4], &dwGoOrNot, 4);
		
		dwProcessMonitor = 0;	//告知驱动方，用户态执行完毕，唤醒
		
		memmove(&chInteractionBuffer[0], &dwProcessMonitor, 4);
	}
}
