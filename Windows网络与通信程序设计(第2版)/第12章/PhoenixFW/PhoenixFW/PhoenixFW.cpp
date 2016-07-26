// PhoenixFW.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PhoenixFW.h"
#include "PhoenixFWDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPhoenixApp

BEGIN_MESSAGE_MAP(CPhoenixApp, CWinApp)
	//{{AFX_MSG_MAP(CPhoenixApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPhoenixApp construction

CPhoenixApp::CPhoenixApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

CPhoenixApp theApp;

CPRuleFile g_RuleFile;				// 管理规则文件
CPIOControl *g_pIoControl = NULL;	// 管理DLL共享数据

BOOL CPhoenixApp::InitInstance()
{
	// 运行一次
	TCHAR szModule[] = L"PhoenixFW";
	m_hSemaphore = ::CreateSemaphore(NULL, 0, 1, szModule);
	if(::GetLastError() == ERROR_ALREADY_EXISTS)
	{
		AfxMessageBox(L" 已经有一个实例在运行！");
		return FALSE;
	}

	// 加载过滤文件
	if(!g_RuleFile.LoadRules())
	{
		AfxMessageBox(L" 加载配置文件出错！");
		return FALSE;
	}

	// 创建DLL I/O控制对象，加载DLL模块
	g_pIoControl = new CPIOControl;
	// 应用文件中的数据，设置应用层和核心层过滤规则
	ApplyFileData();

	//////////////////////////////

	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	CMainDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}
	return FALSE;
}

int CPhoenixApp::ExitInstance() 
{	
	if(g_pIoControl != NULL)
	{
		g_pIoControl->SetWorkMode(PF_PASS_ALL);
		g_pIoControl->SetPhoenixInstance(NULL, L"");
		delete g_pIoControl;
	}
	IMClearRules();
	::CloseHandle(m_hSemaphore);
	return CWinApp::ExitInstance();
}

BOOL CPhoenixApp::SetAutoStart(BOOL bStart)
{
	// 根键、子键名称、和到子键的句柄
	HKEY hRoot = HKEY_LOCAL_MACHINE;
    TCHAR *szSubKey = L"Software\\Microsoft\\Windows\\CurrentVersion\\Run";
	HKEY hKey;

	// 打开指定子键
	DWORD dwDisposition = REG_OPENED_EXISTING_KEY;	// 如果不存在不创建
	LONG lRet = ::RegCreateKeyEx(hRoot, szSubKey, 0, NULL, 
			REG_OPTION_NON_VOLATILE, KEY_ALL_ACCESS, NULL, &hKey, &dwDisposition);
	if(lRet != ERROR_SUCCESS)
		return FALSE;

	if(bStart)
	{
		// 得到当前执行文件的文件名（包含路径）
		char szModule[MAX_PATH] ;
		::GetModuleFileNameA(NULL, szModule, MAX_PATH);
		// 创建一个新的键值，设置键值数据为文件名
		lRet = ::RegSetValueExA(hKey, "PhoenixFW", 0, REG_SZ, (BYTE*)szModule, strlen(szModule));
	}
	else
	{
		// 删除本程序的键值
		lRet = ::RegDeleteValueA(hKey, "PhoenixFW");
	}

	// 关闭子键句柄
	::RegCloseKey(hKey);
	return lRet == ERROR_SUCCESS;
}

BOOL CPhoenixApp::ApplyFileData()
{
	// 设置工作模式
	g_pIoControl->SetWorkMode(g_RuleFile.m_header.ucLspWorkMode);

	// 设置应用层规则文件
	g_pIoControl->SetRuleFile(&g_RuleFile.m_header, g_RuleFile.m_pLspRules);

	// 设置核心层规则文件
	IMClearRules();
	if(g_RuleFile.m_header.ucKerWorkMode == IM_START_FILTER)
	{
		if(!IMSetRules(g_RuleFile.m_pKerRules, g_RuleFile.m_header.ulKerRuleCount))
		{
			AfxMessageBox(L" 设置核心层规则出错！\n");
			return FALSE;
		}
	}
	return TRUE;
}