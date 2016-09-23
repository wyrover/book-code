// SelectKernelModuleHook.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "SelectKernelModuleHook.h"
#include "afxdialogex.h"


// CSelectKernelModuleHook 对话框

HWND skmhWnd;
#define   SKERNEL_HOOK_MAX_COLUMN 9

wchar_t	SHookStr[SKERNEL_HOOK_MAX_COLUMN][260]  = {_T("被挂钩地址"),_T("原始地址"),		_T("函数"),	_T("hook跳转地址"),	_T("Hook模块"),_T("原始模块"),		
	_T("模块基址"),	_T("模块大小"),	_T("Hook类型")};										 
int		SHookWidth[SKERNEL_HOOK_MAX_COLUMN]= {80,80,  100, 90, 180,  160, 80, 80,80};


IMPLEMENT_DYNAMIC(CSelectKernelModuleHook, CDialogEx)

CSelectKernelModuleHook::CSelectKernelModuleHook(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSelectKernelModuleHook::IDD, pParent)
{

}

CSelectKernelModuleHook::~CSelectKernelModuleHook()
{
}

void CSelectKernelModuleHook::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_SKMHOOKList);
}


BEGIN_MESSAGE_MAP(CSelectKernelModuleHook, CDialogEx)
END_MESSAGE_MAP()


// CSelectKernelModuleHook 消息处理程序
extern VOID QueryKernelHook(HWND m_hWnd,ULONG ID,CMyList *m_list);

DWORD WINAPI SQueryKernelHookFunction(CMyList *m_ListCtrl)
{
	QueryKernelHook(skmhWnd,IDC_DebugStatus,m_ListCtrl);
	return 0;
}

BOOL CSelectKernelModuleHook::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	LONG lStyle;
    lStyle = GetWindowLong(m_SKMHOOKList.m_hWnd, GWL_STYLE);//获取当前窗口style
    lStyle &= ~LVS_TYPEMASK; //清除显示方式位
    lStyle |= LVS_REPORT; //设置style
    SetWindowLong(m_SKMHOOKList.m_hWnd, GWL_STYLE, lStyle);//设置style

    DWORD dwStyle = m_SKMHOOKList.GetExtendedStyle();
	//选中某行使整行高亮（只适用与report风格的listctrl）LVS_EX_DOUBLEBUFFER双缓冲区，解决闪烁问题
    dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER;
    m_SKMHOOKList.SetExtendedStyle(dwStyle); //设置扩展风格
	m_SKMHOOKList.SetExtendedStyle(m_SKMHOOKList.GetExtendedStyle()|LVS_EX_SUBITEMIMAGES);

	skmhWnd=m_hWnd;

	for(int Index = 0; Index < SKERNEL_HOOK_MAX_COLUMN; Index++)
	{
		m_SKMHOOKList.InsertColumn(Index, SHookStr[Index] ,LVCFMT_CENTER, SHookWidth[Index]);
	}

	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)SQueryKernelHookFunction,&m_SKMHOOKList, 0,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
