// StackThread.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "StackThread.h"
#include "afxdialogex.h"

typedef struct _THREAD_INFO{
	ULONG ulHideType;           //1 找不到线程所在模块，既认为隐藏线程，显示粉红色，0 正常
	ULONG ThreadID;
	ULONG KernelThread;
	ULONG TEB;
	ULONG ThreadStart;
	ULONG HideDebug;            //如果是0，则显示褐色
	CHAR lpszThreadModule[256];
	UCHAR ulStatus;
	UCHAR ulPriority;    //优先级
	ULONG ulContextSwitches;

}THREAD_INFO,*PTHREAD_INFO;

typedef struct _SYSTEM_THREAD_INFO {          //InlineHook
	ULONG ulCount;
	THREAD_INFO SystemThreadInfo[1];
} SYSTEM_THREAD_INFO, *PSYSTEM_THREAD_INFO;

PSYSTEM_THREAD_INFO SystemThreadTemp;
//---------------------------------------------------------------------------------------
//线程堆栈
//---------------------------------------------------------------------------------------
/*
ring3界面需要显示的效果如下：

lkd> dt_kthread 821b95b8
     ...
	 +0x028 KernelStack      : 0xf8ac179c Void

lkd> dd 0xf8ac179c 

ulAddress ulStack1 ulStack2 ulStack3 ulStack4

f8ac179c  f8ac1dcc 00000246 80542822 f8ac17f8
f8ac17ac  821b95b8 ffdff120 821b9628 80501ce6
f8ac17bc  00000000 821b95b8 804faaf2 806d32d0
f8ac17cc  007c3000 00000000 00000100 00000000
f8ac17dc  00000023 00000023 00000000 00000002
f8ac17ec  00000000 821b9628 01ac1dcc f8ac1844
f8ac17fc  8051a4e3 00000002 f8ac182c 00000001
f8ac180c  00000008 00000000 00000000 00000000
*/
typedef struct _THREAD_STACK_INFO{
	ULONG ulAddress;

	ULONG ulStack1;
	ULONG ulStack2;
	ULONG ulStack3;
	ULONG ulStack4;

}THREAD_STACK_INFO,*PTHREAD_STACK_INFO;

typedef struct _THREAD_STACK {          //THREAD_STACK
	ULONG ulCount;
	ULONG ulKthread;     //ring3打印显示需要用到
	ULONG ulKernelStack; //ring3打印显示需要用到
	ULONG KernelStack;   //ring3打印显示需要用到
	THREAD_STACK_INFO StackInfo[1];

} THREAD_STACK, *PTHREAD_STACK;

PTHREAD_STACK ThreadStack;

//--------------------------------------

extern WCHAR lpwzEthread[50];

ULONG GetKernelInfo(char *lpKernelName,ULONG *ulBase,ULONG *ulSize);
VOID FixPDBModule();
VOID FormatStack(ULONG ulStack,WCHAR *lpwzFormatOut);
BOOL bIsWorking = FALSE;
HWND hWndOfDebug;

#define	READ_PDB_MAX_COLUMN	2

wchar_t	ReadPDBStr[READ_PDB_MAX_COLUMN][260]  = {_T("地址"),_T("调用函数")};										 
int		ReadPDBWidth[READ_PDB_MAX_COLUMN]= {70,250};

// CStackThread 对话框

IMPLEMENT_DYNAMIC(CStackThread, CDialogEx)

CStackThread::CStackThread(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStackThread::IDD, pParent)
	, m_strStackThread(_T(""))
{

}

CStackThread::~CStackThread()
{
}

void CStackThread::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_strStackThread);
	DDX_Control(pDX, IDC_LIST1, m_ListCtrl);
}


BEGIN_MESSAGE_MAP(CStackThread, CDialogEx)
	ON_BN_CLICKED(IDC_ReadPDB, &CStackThread::OnBnThreadStackByPDB)
END_MESSAGE_MAP()


// CStackThread 消息处理程序


BOOL CStackThread::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	LONG lStyle;
	lStyle = GetWindowLong(m_ListCtrl.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_ListCtrl.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_ListCtrl.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER;//选中某行使整行高亮（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_ListCtrl.SetExtendedStyle(dwStyle); //设置扩展风格

	hWndOfDebug = m_hWnd;

	bIsWorking = FALSE;

	for(int Index = 0; Index < READ_PDB_MAX_COLUMN; Index++)
	{
		m_ListCtrl.InsertColumn(Index, ReadPDBStr[Index] ,LVCFMT_LEFT,ReadPDBWidth[Index]);
	}

	// TODO:  在此添加额外的初始化
	//m_strStackThread.Format(_T("我在CStackThread中，全有问题都解决好了"));
	ULONG ulBase;
	ULONG ulSize;
	char lpszKernelPath[260] = {0};
	DWORD dwReadByte;
	int i =0;
	m_strStackThread=L"";
	CString m_str;

	//先得到一次线程列表
	SystemThreadTemp = (PSYSTEM_THREAD_INFO)VirtualAlloc(0, sizeof(SYSTEM_THREAD_INFO)*257,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (SystemThreadTemp)
	{
		memset(SystemThreadTemp,0,sizeof(SYSTEM_THREAD_INFO)*257);
		ReadFile((HANDLE)LIST_SYSTEM_THREAD,SystemThreadTemp, sizeof(SYSTEM_THREAD_INFO)*257,&dwReadByte,0);

		for ( i=0;i< (int)SystemThreadTemp->ulCount;i++)
		{
			WCHAR lpwzBuffer[50] = {0};
			memset(lpwzBuffer,0,50);
			wsprintfW(lpwzBuffer,L"0x%08X",SystemThreadTemp->SystemThreadInfo[i].KernelThread);
			if (_wcsnicmp(lpwzBuffer,lpwzEthread,wcslen(lpwzEthread)) == 0)
			{
				ReadFile((HANDLE)INIT_THREAD_STACK,0,SystemThreadTemp->SystemThreadInfo[i].KernelThread,&dwReadByte,0);

				//释放内存
				VirtualFree(SystemThreadTemp,sizeof(SYSTEM_THREAD_INFO)*257,MEM_RESERVE | MEM_COMMIT);
				break;
			}
		}
	}

	if (GetKernelInfo(lpszKernelPath,&ulBase,&ulSize))
	{
		if (ThreadStack)
			VirtualFree(ThreadStack,ulSize+1024,MEM_RESERVE | MEM_COMMIT);

		ThreadStack = (PTHREAD_STACK)VirtualAlloc(0,ulSize+1024,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
		if (ThreadStack)
		{
			memset(ThreadStack,0,ulSize+1024);

			ReadFile((HANDLE)LIST_THREAD_STACK,ThreadStack,ulSize+1024,&dwReadByte,0);
			if (ThreadStack->ulCount > 10)
			{
				WCHAR *lpForMat = L"lkd> dt_kthread 0x%08X\r\n"
					              L"     ...\r\n"
								  L"     +0x%x KernelStack      : 0x%08x Void\r\n\r\n"
								  L"lkd> dd 0x%08x\r\n"; 

				WCHAR lpBuffer[260] = {0};
				wsprintfW(lpBuffer,lpForMat,ThreadStack->ulKthread,ThreadStack->ulKernelStack,ThreadStack->KernelStack,ThreadStack->KernelStack);
				//m_strStackThread.Format(lpBuffer);
				m_str.Format(lpBuffer);
				m_strStackThread+=m_str;
				UpdateData(false);//这个用来刷新数据，可以加到循环外
			}else{
				m_str.Format(L"堆栈不可用!\r\n");
				m_strStackThread+=m_str;
				UpdateData(false);//这个用来刷新数据，可以加到循环外
				VirtualFree(ThreadStack,ulSize+1024,MEM_RESERVE | MEM_COMMIT);
				return FALSE;
			}
			for (i=0;i< (int)ThreadStack->ulCount;i++)
			{
				WCHAR *lpForMat1 = L"0x%08x  %08x %08x %08x %08x\r\n";
				WCHAR lpBuffer1[260] = {0};

				wsprintfW(lpBuffer1,lpForMat1,
					ThreadStack->StackInfo[i].ulAddress,
					ThreadStack->StackInfo[i].ulStack1,
					ThreadStack->StackInfo[i].ulStack2,
					ThreadStack->StackInfo[i].ulStack3,
					ThreadStack->StackInfo[i].ulStack4);
				m_str.Format(lpBuffer1);
				m_strStackThread+=m_str;
				UpdateData(false);//这个用来刷新数据，可以加到循环外
			}
			//VirtualFree(ThreadStack,ulSize+1024,MEM_RESERVE | MEM_COMMIT);
		}
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
//获取函数到一块数据表准备解析
VOID GetPDBFuncInfo()
{
	FixPDBModule();
}
//开始解析ulStack的内存地址
VOID FormatStackAddr(ULONG ulStack,WCHAR *lpwzFormatOut)
{
	FormatStack(ulStack,lpwzFormatOut);
}
DWORD WINAPI ThreadStackByPDB(CListCtrl *m_List)
{
	if (ThreadStack->ulCount < 10){
		SetDlgItemTextW(hWndOfDebug,IDC_StackDebug,L"无法完成解析!");
		return 0;
	}

	m_List->DeleteAllItems();

	bIsWorking = 5;

	SetDlgItemTextW(hWndOfDebug,IDC_StackDebug,L"正在解析堆栈，请稍后...");
	
	GetPDBFuncInfo();

	int x=0;
	for (int i=0;i<ThreadStack->ulCount;i++)
	{
		WCHAR lpwzAddr[250] = {0};
		WCHAR lpwzStack1[250] = {0};
		WCHAR lpwzStack2[250] = {0};
		WCHAR lpwzStack3[250] = {0};
		WCHAR lpwzStack4[250] = {0};

		memset(lpwzAddr,0,sizeof(lpwzAddr));
		memset(lpwzStack1,0,sizeof(lpwzStack1));
		memset(lpwzStack2,0,sizeof(lpwzStack2));
		memset(lpwzStack3,0,sizeof(lpwzStack3));
		memset(lpwzStack4,0,sizeof(lpwzStack4));

		wsprintfW(lpwzAddr,L"0x%08x",ThreadStack->StackInfo[i].ulAddress);

		if (ThreadStack->StackInfo[i].ulStack1 > 0x80000000){
			//wsprintfW(lpwzStack1,L"0x%08x",ThreadStack->StackInfo[i].ulStack1);
			FormatStackAddr(ThreadStack->StackInfo[i].ulStack1,lpwzStack1);
		}else
			wsprintfW(lpwzStack1,L"-",ThreadStack->StackInfo[i].ulStack1);

		if (ThreadStack->StackInfo[i].ulStack2 > 0x80000000){
			//wsprintfW(lpwzStack2,L"0x%08x",ThreadStack->StackInfo[i].ulStack2);
			FormatStackAddr(ThreadStack->StackInfo[i].ulStack2,lpwzStack2);
		}else
			wsprintfW(lpwzStack2,L"-",ThreadStack->StackInfo[i].ulStack2);

		if (ThreadStack->StackInfo[i].ulStack3 > 0x80000000){
			//wsprintfW(lpwzStack3,L"0x%08x",ThreadStack->StackInfo[i].ulStack3);
			FormatStackAddr(ThreadStack->StackInfo[i].ulStack3,lpwzStack3);
		}else
			wsprintfW(lpwzStack3,L"-",ThreadStack->StackInfo[i].ulStack3);

		if (ThreadStack->StackInfo[i].ulStack4 > 0x80000000){
			//wsprintfW(lpwzStack4,L"0x%08x",ThreadStack->StackInfo[i].ulStack4);
			FormatStackAddr(ThreadStack->StackInfo[i].ulStack4,lpwzStack4);
		}else
			wsprintfW(lpwzStack4,L"-",ThreadStack->StackInfo[i].ulStack4);

		m_List->InsertItem(i+x,lpwzAddr,RGB(77,77,77));
		m_List->SetItemText(i+x,1,lpwzStack1);

		x++;
		m_List->InsertItem(i+x,L"-",RGB(77,77,77));
		m_List->SetItemText(i+x,1,lpwzStack2);

		x++;
		m_List->InsertItem(i+x,L"-",RGB(77,77,77));
		m_List->SetItemText(i+x,1,lpwzStack3);

		x++;
		m_List->InsertItem(i+x,L"-",RGB(77,77,77));
		m_List->SetItemText(i+x,1,lpwzStack4);
	}
	bIsWorking = FALSE;
	SetDlgItemTextW(hWndOfDebug,IDC_StackDebug,L"解析完毕...");
	return 0;
}
void CStackThread::OnBnThreadStackByPDB()
{
	//ThreadStackByPDB(&m_ListCtrl);
	if (bIsWorking == 5){
		MessageBoxW(L"当前解析正在进行，请稍后操作...",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadStackByPDB,&m_ListCtrl, 0,NULL);
}