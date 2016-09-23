// SelectAnyModule.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "A-ProtectView.h"

#include "SelectAnyModule.h"
#include "afxdialogex.h"

#define  SystemModuleInformation 11

#define STATUS_INFO_LENGTH_MISMATCH ((NTSTATUS)0xC0000004L)
#define STATUS_SUCCESS        0x00000000 
#define STATUS_UNSUCCESSFUL (0xC0000001L)
typedef LONG NTSTATUS;

typedef struct _SYSTEM_MODULE_INFORMATION  // 系统模块信息
{
	ULONG  Reserved[2];  
	ULONG  Base;        
	ULONG  Size;         
	ULONG  Flags;        
	USHORT Index;       
	USHORT Unknown;     
	USHORT LoadCount;   
	USHORT ModuleNameOffset;
	CHAR   ImageName[256];   
} SYSTEM_MODULE_INFORMATION, *PSYSTEM_MODULE_INFORMATION;

typedef struct _tagSysModuleList {          //模块链结构
	ULONG ulCount;
	SYSTEM_MODULE_INFORMATION smi[1];
} MODULES, *PMODULES;

extern BOOL bIsStopHookScan;

extern "C" NTSTATUS __stdcall  ZwQuerySystemInformation(
	__in       ULONG SystemInformationClass,
	__inout    PVOID SystemInformation,
	__in       ULONG SystemInformationLength,
	__out_opt  PULONG ReturnLength
	);
VOID FixSelectModuleToKernel(ULONG ulModuleBase,WCHAR *ModulePath,char *lpszModulePath);
VOID QueryKernelHook(HWND m_hWnd,ULONG ID,CMyList *m_list);
//*********************************************
typedef struct _SELECT_MODULE_INFORMATION  // 所选择的模块信息
{
	ULONG  Base;        
	CHAR   lpszImageFileName[256];
	WCHAR  lpwzImageName[256];

} SELECT_MODULE_INFORMATION, *PSELECT_MODULE_INFORMATION;

typedef struct _SELECTMODULE {          //模块链结构
	ULONG ulCount;
	SELECT_MODULE_INFORMATION Module[1];
} SELECTMODULE, *PSELECTMODULE;

PSELECTMODULE SelectModuleHook;

ULONG ulKernelSize;
DWORD StringToHex(char* strSource);
//*********************************************
#define	 MODULE_MAX_COLUMN	2

wchar_t	SelectModuleStr[MODULE_MAX_COLUMN][260]  = {_T("基址"),_T("模块路径")};										 
int		SelectModuleWidth[MODULE_MAX_COLUMN]= {90,300};

// CSelectAnyModule 对话框

IMPLEMENT_DYNAMIC(CSelectAnyModule, CDialogEx)

CSelectAnyModule::CSelectAnyModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectAnyModule::IDD, pParent)
{

}

CSelectAnyModule::~CSelectAnyModule()
{
}

void CSelectAnyModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SELECTANYMODULE, m_SelectAnyModuleList);
}


BEGIN_MESSAGE_MAP(CSelectAnyModule, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_SELECTALL, &CSelectAnyModule::OnBnClickedBtnSelectall)
	ON_BN_CLICKED(IDC_BTN_CANCELSELECT, &CSelectAnyModule::OnBnClickedBtnCancelSelectall)
	
	ON_BN_CLICKED(IDC_BTN_SCAN, &CSelectAnyModule::OnBnClickedBtnScan)

END_MESSAGE_MAP()


// CSelectAnyModule 消息处理程序

BOOL CSelectAnyModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LONG lStyle;
    lStyle = GetWindowLong(m_SelectAnyModuleList.m_hWnd, GWL_STYLE);//获取当前窗口style
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置style
    SetWindowLong(m_SelectAnyModuleList.m_hWnd, GWL_STYLE, lStyle);//设置style

    DWORD dwStyle = m_SelectAnyModuleList.GetExtendedStyle();
	//选中某行使整行高亮（只适用与report风格的listctrl）LVS_EX_DOUBLEBUFFER双缓冲区，解决闪烁问题
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER|LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT;
    m_SelectAnyModuleList.SetExtendedStyle(dwStyle); //设置扩展风格
	m_SelectAnyModuleList.SetExtendedStyle(m_SelectAnyModuleList.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);

	//在这里加入数据的初始化，插入，我就不写了，一会该回去了，可以参考上次我加入的那个类，类外生命函数名
	for(int Index = 0; Index < MODULE_MAX_COLUMN; Index++)
	{
		m_SelectAnyModuleList.InsertColumn(Index,SelectModuleStr[Index] ,LVCFMT_LEFT, SelectModuleWidth[Index]);
	}
	//***********************************************************************
	DWORD	dwsize;
	DWORD	dwSizeReturn;
	PUCHAR	pBuffer	=	NULL;
	PMODULES	pSmi=NULL;
	NTSTATUS	ntStatus=STATUS_UNSUCCESSFUL;
	int x=0;

	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation,
		pSmi, 
		0, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_INFO_LENGTH_MISMATCH)
	{
		return 0;
	}
	dwsize	=	dwSizeReturn*2;
	pSmi	=	(PMODULES)new char[dwsize];
	if (pSmi==NULL)
	{
		return 0;
	}

	ntStatus = ZwQuerySystemInformation(
		SystemModuleInformation, 
		pSmi,
		dwsize, 
		&dwSizeReturn
		);
	if (ntStatus!=STATUS_SUCCESS)
	{
		return 0;
	}
	for (int i=0;i<pSmi->ulCount;i++)
	{
		//得到内核大小
		if (i == 0){
			ulKernelSize = pSmi->smi[i].Size;
		}
		WCHAR lpwzDosFullPath[256];
		WCHAR lpwzWinDir[256];
		WCHAR lpwzSysDisk[256];
		WCHAR lpwzFullSysName[1024] = {0};
		CHAR lpszPath[1024] = {0};

		memset(lpwzWinDir,0,sizeof(lpwzWinDir));
		memset(lpwzSysDisk,0,sizeof(lpwzSysDisk));
		memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
		memset(lpwzFullSysName,0,sizeof(lpwzFullSysName));
		memset(lpszPath,0,sizeof(lpszPath));

		MultiByteToWideChar(
			CP_ACP,
			0, 
			pSmi->smi[i].ImageName,
			-1, 
			lpwzFullSysName, 
			strlen(pSmi->smi[i].ImageName)
			);

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
		if (wcsstr(lpwzFullSysName,L"\\") == 0)
		{
			memset(lpwzDosFullPath,0,sizeof(lpwzDosFullPath));
			wcscat(lpwzDosFullPath,lpwzSysDisk);
			wcscat(lpwzDosFullPath,L"\\WINDOWS\\system32\\drivers\\");
			wcscat(lpwzDosFullPath,lpwzFullSysName);
		}
Next:
// 		CHAR ShartPath[50] = {0};
// 		WideCharToMultiByte( CP_ACP,
// 			0,
// 			lpwzDosFullPath,
// 			-1,
// 			lpszPath,
// 			wcslen(lpwzDosFullPath)*2,
// 			NULL,
// 			NULL
// 			);
		//这里插入数据
		WCHAR lpwzBase[50] = {0};
		wsprintfW(lpwzBase,L"%08x",pSmi->smi[i].Base);
		m_SelectAnyModuleList.InsertItem(i,lpwzBase,RGB(255,20,147));
		m_SelectAnyModuleList.SetItemText(i,1,lpwzDosFullPath);
		//
	}
	//************************************************************************
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

//全选
void CSelectAnyModule::OnBnClickedBtnSelectall()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_SelectAnyModuleList.GetItemCount();i++)
		ListView_SetCheckState(m_SelectAnyModuleList,i,TRUE);
}
//取消全选
void CSelectAnyModule::OnBnClickedBtnCancelSelectall()
{
	// TODO: 在此添加控件通知处理程序代码
	for(int i=0;i<m_SelectAnyModuleList.GetItemCount();i++)
		ListView_SetCheckState(m_SelectAnyModuleList,i,FALSE);
}
void CSelectAnyModule::OnBnClickedBtnScan()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	CString BaseText;
	CString PathText;
	int x=0;
	WCHAR lpwzBaseText[50];
	 CHAR lpszBaseText[50];

	WCHAR lpwzPathText[260];

	if (!m_SelectAnyModuleList.GetItemCount())
	{
		return;
	}
	SelectModuleHook = (PSELECTMODULE)VirtualAlloc(0, ulKernelSize+1024,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!SelectModuleHook){
		return;
	}
	memset(SelectModuleHook,0,ulKernelSize+1024);
	
	for(int i=0;i<m_SelectAnyModuleList.GetItemCount();i++)
	{
		if(ListView_GetCheckState(m_SelectAnyModuleList,i)==TRUE)
		{
			BaseText = m_SelectAnyModuleList.GetItemText(i,0);
			PathText = m_SelectAnyModuleList.GetItemText(i,1);
			//str.Format(_T("%d"),i);
			//MessageBox(PathText,BaseText,0);
			memset(lpwzPathText,0,sizeof(lpwzPathText));
			memset(lpwzBaseText,0,sizeof(lpwzBaseText));
			wcscat(lpwzBaseText,BaseText);
			wcscat(lpwzPathText,PathText);

			CHAR lpszPath[260] = {0};
			WideCharToMultiByte( CP_ACP,
				0,
				lpwzPathText,
				-1,
				lpszPath,
				wcslen(lpwzPathText)*2,
				NULL,
				NULL
				);
			wcscat(SelectModuleHook->Module[x].lpwzImageName,lpwzPathText);
			strcat(SelectModuleHook->Module[x].lpszImageFileName,lpszPath);

			WideCharToMultiByte( CP_ACP,
				0,
				lpwzBaseText,
				-1,
				lpszBaseText,
				wcslen(lpwzBaseText)*2,
				NULL,
				NULL
				);
			SelectModuleHook->Module[x].Base = StringToHex(lpszBaseText);
			SelectModuleHook->ulCount = x;
			x++;
		}
	}
	EndDialog(0);
}
void SearchSelectModuleHook(HWND m_hWnd,ULONG ID,CMyList *m_list)
{
	if (!SelectModuleHook){
		SetDlgItemTextW(m_hWnd,ID,L"没有选择要扫描的项目...");
		return;
	}
	for (int i=0;i<=SelectModuleHook->ulCount;i++)
	{
		if (bIsStopHookScan){
			break;
		}
		//MessageBoxW(0,SelectModuleHook->Module[i].lpwzImageName,0,0);
		WCHAR lpwzTextOut[100];
		memset(lpwzTextOut,0,sizeof(lpwzTextOut));
		wsprintfW(lpwzTextOut,L"[%d-%d]%ws",SelectModuleHook->ulCount,i,SelectModuleHook->Module[i].lpwzImageName);
		SetDlgItemTextW(m_hWnd,ID,lpwzTextOut);

		FixSelectModuleToKernel(SelectModuleHook->Module[i].Base,SelectModuleHook->Module[i].lpwzImageName,SelectModuleHook->Module[i].lpszImageFileName);
		QueryKernelHook(m_hWnd,ID,m_list);
	}
	SetDlgItemTextW(m_hWnd,ID,L"扫描完毕...");
}