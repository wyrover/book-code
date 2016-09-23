#include "stdafx.h"
#include "A-Protect.h"
#include "SubModule.h"
#include "Afxdialogex.h"

#define DLLMODULE_MAX_COLUMN 5

HWND hWnd;

wchar_t	DllModuleStr[DLLMODULE_MAX_COLUMN][260]  = {_T("基址"),	_T("大小"),			_T("DLL模块路径"),_T("系统服务启动"),_T("MD5/数字签名")};										 
int		DllModuleWidth[DLLMODULE_MAX_COLUMN]=	{100,80,200,100,140};

wchar_t	HandleStr[DLLMODULE_MAX_COLUMN][260]  = {_T("句柄类型代号"),_T("句柄类型"),	_T("进程所打开文件"),_T("句柄"),_T("句柄对象")};										 
int		HandleWidth[DLLMODULE_MAX_COLUMN]=	{100,70,300,70,70};

#define PROCESS_THREAD_MAX_COLUMN 10

wchar_t	ProcessThreadStr[PROCESS_THREAD_MAX_COLUMN][260]  = {_T("线程Id"),	_T("ETHREAD"),			_T("TEB"),	_T("线程入口"),		
	_T("HideDebug"),_T("优先级"),_T("切换次数"),_T("模块"),_T("线程状态"),_T("MD5/数字签名")};										 
int		ProcessThreadWidth[PROCESS_THREAD_MAX_COLUMN]=	{50, 70, 70, 80, 70,60,60,150,70,140};

void QueryProcessThread(HWND m_hWnd,ULONG ID,CMyList *m_list);
void KillProcessThread(HWND hWnd,CMyList *m_list,HANDLE ulCommand);

VOID EnumModule(HWND m_hWnd,ULONG ID,CMyList *m_list);
void UnLoadDLLModule(HWND m_hWnd,CMyList *m_list);
void UnLoadDLLModuleAndDelete(HWND m_hWnd,CMyList *m_list);
void CopyDLLDataToClipboard(HWND m_hWnd,CMyList *m_list);
void LookupDLLInServices(HWND m_hWnd,ULONG ID,CMyList *m_list,int Type);

VOID QueryProcessHandle(HWND m_hWnd,ULONG ID,CMyList *m_list);

IMPLEMENT_DYNAMIC(CSubModule, CDialogEx)

CSubModule::CSubModule(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSubModule::IDD, pParent)
{
	m_Subcase=0;
	m_SubItem=0;
}

CSubModule::~CSubModule()
{
}

void CSubModule::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SubList);
}


BEGIN_MESSAGE_MAP(CSubModule, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_CLOSE, &CSubModule::OnBnClickedBtnClose)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CSubModule::OnNMRClickList1)

	ON_COMMAND(ID_UnLoadDLLModule, &CSubModule::OnUnLoadDLLModule)
	ON_COMMAND(ID_UnLoadDLLModuleAndDeleteFile, &CSubModule::OnUnLoadDLLModuleAndDelete)
	ON_COMMAND(ID_CopyDLLDataToClipboard, &CSubModule::OnCopyDLLDataToClipboard)
	ON_COMMAND(ID_LookupDLLInServices, &CSubModule::OnLookupDLLInServices)
	ON_COMMAND(ID_TrustDllFile, &CSubModule::OnTrustDLLModule)

	ON_COMMAND(ID_ProcessThreadList, &CSubModule::OnProcessThreadList)
	ON_COMMAND(ID_KillSelectProcessThread, &CSubModule::OnKillProcessThread)
	ON_COMMAND(ID_SuspendSelectProcessThread, &CSubModule::OnSuspendProcessThread)
	ON_COMMAND(ID_ResumeSelectProcessThread, &CSubModule::OnResumeProcessThread)


	//ON_WM_TIMER()
END_MESSAGE_MAP()


// CProcessModule 消息处理程序


void CSubModule::OnBnClickedBtnClose()
{
	// TODO: 在此添加控件通知处理程序代码
	OnOK();
}
DWORD WINAPI QueryThreadFuntion(CMyList *m_SubList)
{
	QueryProcessThread(hWnd,IDC_DllDebugStatus,m_SubList);
	return 0;
}
DWORD WINAPI QueryHandleFuntion(CMyList *m_SubList)
{
	DWORD dwReadByte;

	ReadFile((HANDLE)KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);

	QueryProcessHandle(hWnd,IDC_DllDebugStatus,m_SubList);

	ReadFile((HANDLE)NO_KERNEL_SAFE_MODULE,0,0,&dwReadByte,0);
	return 0;
}
DWORD WINAPI QueryDllFuntion(CMyList *m_SubList)
{
	EnumModule(hWnd,IDC_DllDebugStatus,m_SubList);
	return 0;
}
BOOL CSubModule::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LONG lStyle;
    lStyle = GetWindowLong(m_SubList.m_hWnd, GWL_STYLE);//获取当前窗口style
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置style
    SetWindowLong(m_SubList.m_hWnd, GWL_STYLE, lStyle);//设置style

    DWORD dwStyle = m_SubList.GetExtendedStyle();
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER;//选中某行使整行高亮（只适用与report风格的listctrl）
    //dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
    //dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
    m_SubList.SetExtendedStyle(dwStyle); //设置扩展风格

	hWnd = m_hWnd;
	switch (m_Subcase)
	{
	case 1:
		for(int Index = 0; Index < DLLMODULE_MAX_COLUMN; Index++)
		{
			m_SubList.InsertColumn(Index, DllModuleStr[Index] ,LVCFMT_LEFT, DllModuleWidth[Index]);
		}

		SetWindowText(L"DLL模块");
		//EnumModule(m_hWnd,IDC_DllDebugStatus,&m_SubList);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryDllFuntion,&m_SubList, 0,NULL);
		//SetTimer(1,100,NULL);
		break;
	case 2:
		for(int Index = 0; Index < DLLMODULE_MAX_COLUMN; Index++)
		{
			m_SubList.InsertColumn(Index, HandleStr[Index] ,LVCFMT_LEFT, HandleWidth[Index]);
		}
		SetWindowText(L"Handle");
		//QueryProcessHandle(m_hWnd,IDC_DllDebugStatus,&m_SubList);
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryHandleFuntion,&m_SubList, 0,NULL);
		//SetTimer(2,100,NULL);
		break;
	case 3:
		for(int Index = 0; Index < PROCESS_THREAD_MAX_COLUMN; Index++)
		{
			m_SubList.InsertColumn(Index, ProcessThreadStr[Index] ,LVCFMT_LEFT, ProcessThreadWidth[Index]);
		}
		SetWindowText(L"Thread");
		CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryThreadFuntion,&m_SubList, 0,NULL);
		//SetTimer(3,100,NULL);
		break;
	default:
		return 0;
	}
// 	m_SubList.InsertColumn( 0, _T("ID"), LVCFMT_CENTER, 40 );//插入列
// 	m_SubList.InsertColumn( 1, _T("NAME"), LVCFMT_CENTER, 50 );

	/*CString strtemp;
	strtemp.Format(_T("单击的是第%d行第%d列"),
	m_Subcase, m_SubItem);
	MessageBox(strtemp);*/
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CSubModule::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	//右键菜单
	DWORD dwPos = GetMessagePos();
    CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
	CMenu menu;

	switch (m_SubItem)
	{
	case 1:
		menu.LoadMenu(IDR_DLLMODULE_MENU);
		break;
	case 3:
		menu.LoadMenu(IDR_PROCESSTHREAD_MENU);
		break;
	default:
		return;
	}
	CMenu* popup = menu.GetSubMenu(0);
    ASSERT( popup != NULL );
    popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
}
void CSubModule::OnUnLoadDLLModule()
{
	UnLoadDLLModule(m_hWnd,&m_SubList);
}
void CSubModule::OnUnLoadDLLModuleAndDelete()
{
	UnLoadDLLModuleAndDelete(m_hWnd,&m_SubList);
}
void CSubModule::OnCopyDLLDataToClipboard()
{
	CopyDLLDataToClipboard(m_hWnd,&m_SubList);
}
void CSubModule::OnLookupDLLInServices()
{
	LookupDLLInServices(m_hWnd,IDC_DllDebugStatus,&m_SubList,0);
}
void CSubModule::OnTrustDLLModule()
{
	LookupDLLInServices(m_hWnd,IDC_DllDebugStatus,&m_SubList,1);
}
//***********************************
void CSubModule::OnProcessThreadList()
{
	m_SubList.DeleteAllItems();
	//QueryProcessThread(m_hWnd,IDC_DllDebugStatus,&m_SubList);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryThreadFuntion,&m_SubList, 0,NULL);
}
void CSubModule::OnKillProcessThread()
{
	//KillProcessThreadFunction(m_hWnd,IDC_DllDebugStatus,&m_SubList);

	KillProcessThread(hWnd,&m_SubList,(HANDLE)KILL_SYSTEM_THREAD);

	m_SubList.DeleteAllItems();
	//QueryProcessThread(m_hWnd,IDC_DllDebugStatus,&m_SubList);
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)QueryThreadFuntion,&m_SubList, 0,NULL);
}
void CSubModule::OnSuspendProcessThread()
{
	KillProcessThread(m_hWnd,&m_SubList,(HANDLE)SUSPEND_THREAD);
}
void CSubModule::OnResumeProcessThread()
{
	KillProcessThread(m_hWnd,&m_SubList,(HANDLE)RESUME_THREAD);
}

/*void CSubModule::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		EnumModule(m_hWnd,IDC_DllDebugStatus,&m_SubList);
		KillTimer(1);
		break;
	case 2:
		QueryProcessHandle(m_hWnd,IDC_DllDebugStatus,&m_SubList);
		KillTimer(2);
		break;
	case 3:
		QueryProcessThread(hWnd,IDC_DllDebugStatus,&m_SubList);
		KillTimer(3);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}*/
