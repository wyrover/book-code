// CProcessSearch.cpp : 实现文件
//

#include "stdafx.h"
#include "A-Protect.h"
#include "CProcessSearch.h"
#include "Afxdialogex.h"
#include "A-ProtectView.h"

HWND hWndStatusOut;
BOOL bStopSearch = FALSE;

BOOL PrintfDosPath(__in LPCTSTR lpwzNtFullPath,__out LPCTSTR lpwzDosFullPath);
BOOL VerifyEmbeddedSignature( LPCWSTR lpFileName );

typedef struct _SAFESYSTEM_PROCESS_INFORMATION {          //SAFESYSTEM_PROCESS_INFORMATION
	int   IntHideType;
	ULONG ulPid;
	ULONG EProcess;
	WCHAR lpwzFullProcessPath[256];
	ULONG ulInheritedFromProcessId;  //父进程
	ULONG ulKernelOpen;
}SAFESYSTEM_PROCESS_INFORMATION, *PSAFESYSTEM_PROCESS_INFORMATION;

typedef struct _PROCESSINFO {          //PROCESSINFO
	ULONG ulCount;
	SAFESYSTEM_PROCESS_INFORMATION ProcessInfo[1];
} PROCESSINFO, *PPROCESSINFO;
//---------------------------------------------------------------------------------------
//dll
//---------------------------------------------------------------------------------------
typedef struct _SearchDllModule_INFORMATION {
	ULONG ulBase;
	WCHAR lpwzDllModule[256];
} SearchDllModule_INFORMATION, *PSearchDllModule_INFORMATION;

typedef struct _DllModuleINFO {          //DLL
	ULONG ulCount;
	SearchDllModule_INFORMATION DllInfo[1];
} DllModuleINFO, *PDllModuleINFO;
//---------------------------------------------------------------------------------------
//had been searched~把所有已经搜索过的dll模块路径放到一个内存，只要发现有重复，就返回下一位~
//---------------------------------------------------------------------------------------
typedef struct _BEEN_SEARCHED_DLL_INFORMATION {
	WCHAR lpwzDllModule[256];
} BEEN_SEARCHED_DLL_INFORMATION, *PBEEN_SEARCHED_DLL_INFORMATION;

typedef struct _BEENSEARCHEDDLLINFO {
	ULONG ulCount;
	BEEN_SEARCHED_DLL_INFORMATION DllInfo[1];
} BEENSEARCHEDDLLINFO, *PBEENSEARCHEDDLLINFO;

//---------------------------------------------------------------------------------------
WCHAR SearchDLLString[260] = {0};

BOOL NtFilePathToDosFilePath(__in LPCTSTR lpwzNtFilePath,__out LPCTSTR lpwzDosFilePath);

//***************************************************************************
//process
//***************************************************************************
#define	 SEARCH_DLL_MAX_COLUMN	7

wchar_t	SearchDllStr[SEARCH_DLL_MAX_COLUMN][260]  = {_T("序号"),_T("DLL名"),_T("DLL路径"),	_T("进程路径"),		
	_T("进程EPROCESS"),_T("进程PID"),_T("MD5/数字签名")};										 
int		SearchDllWidth[SEARCH_DLL_MAX_COLUMN]= {40,50,170,200, 80,70 ,70};

IMPLEMENT_DYNAMIC(CCProcessSearch, CDialogEx)

	CCProcessSearch::CCProcessSearch(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCProcessSearch::IDD, pParent)
	, m_strFindInfo(_T(""))
{
}

CCProcessSearch::~CCProcessSearch()
{
}

void CCProcessSearch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_FINDINFO, m_strFindInfo);
	DDX_Control(pDX, IDC_LIST1, m_listFindInfo);
}


BEGIN_MESSAGE_MAP(CCProcessSearch, CDialogEx)
	ON_BN_CLICKED(ID_BTN_SEARCH, &CCProcessSearch::OnBnClickedBtnSearch)
	ON_BN_CLICKED(ID_STOP_SEARCH, &CCProcessSearch::OnBnClickedStopSearch)
	
	//ON_WM_TIMER()
END_MESSAGE_MAP()


// CCProcessSearch 消息处理程序


BOOL CCProcessSearch::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	LONG lStyle;
	lStyle = GetWindowLong(m_listFindInfo.m_hWnd, GWL_STYLE);//获取当前窗口style
	lStyle &= ~LVS_TYPEMASK; //清除显示方式位
	lStyle |= LVS_REPORT; //设置style
	SetWindowLong(m_listFindInfo.m_hWnd, GWL_STYLE, lStyle);//设置style

	DWORD dwStyle = m_listFindInfo.GetExtendedStyle();
	dwStyle |= LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES|LVS_EX_DOUBLEBUFFER;//选中某行使整行高亮（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_GRIDLINES;//网格线（只适用与report风格的listctrl）
	//dwStyle |= LVS_EX_CHECKBOXES;//item前生成checkbox控件
	m_listFindInfo.SetExtendedStyle(dwStyle); //设置扩展风格

	hWndStatusOut = m_hWnd;

	switch(idFrom)
	{
	case 1:
		//对来自“某个DLL模块”的初始化
		SetWindowText(_T("在进程中查找DLL模块"));
		for(int Index = 0; Index < SEARCH_DLL_MAX_COLUMN; Index++)
		{
			m_listFindInfo.InsertColumn(Index, SearchDllStr[Index] ,LVCFMT_LEFT, SearchDllWidth[Index]);
		}
		break;
	case 2:
		SetWindowText(_T("查找未通过数字签名的DLL模块"));
		for(int Index = 0; Index < SEARCH_DLL_MAX_COLUMN; Index++)
		{
			m_listFindInfo.InsertColumn(Index, SearchDllStr[Index] ,LVCFMT_LEFT, SearchDllWidth[Index]);
		}
		((CEdit*)GetDlgItem(IDC_EDIT_FINDINFO))->SetReadOnly(TRUE);

		// 		CWnd* pWndParent = GetParent();
		// 		CWnd* pWnd = pWndParent->GetDlgItem(IDC_EDIT_FINDINFO);
		// 		pWnd->EnableWindow(FALSE);
		break;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}
extern HANDLE  g_hThreadEvent;
extern BOOL bIsPhysicalCheck;
extern WCHAR PhysicalFile[260];
extern void SaveToFile(CHAR *lpszBuffer,WCHAR *lpwzFilePath);
//体检函数，用多线程吧
void DllModuleInfoForPhysical(CMyList *m_list)
{
	DWORD dwReadByte;
	int i=0,x=0,y=0,been=0;
	int count=0;
	BOOL bHadBeenSearched = FALSE;
	WCHAR lpwzDLLPath[260];
	WCHAR lpwzMd5[260] = {0};
	WCHAR NumBer[10] = {0};
	WCHAR lpwzFullPath[260] = {0};
	WCHAR lpwzFullProcPath[260] = {0};
	WCHAR lpwzEprocess[260] = {0};
	WCHAR lpwzPid[10] = {0};
	WCHAR lpwzTrue[260] = {0};
	WCHAR lpwzBase[260] = {0};

	PBEENSEARCHEDDLLINFO BeenSearchedDllModule;
	PDllModuleINFO SearchDllModule;
	PPROCESSINFO ProcessInfo;

	if (bIsPhysicalCheck){
		SaveToFile("\r\n\r\n[---DLL模块---]\r\n",PhysicalFile);
	}
	BeenSearchedDllModule = (PBEENSEARCHEDDLLINFO)VirtualAlloc(0,(sizeof(BEENSEARCHEDDLLINFO)+sizeof(BEEN_SEARCHED_DLL_INFORMATION))*1024,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!BeenSearchedDllModule)
	{
		return;
	}
	SearchDllModule = (PDllModuleINFO)VirtualAlloc(0,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!SearchDllModule)
	{
		return;
	}
	//先得到一次所有的进程列表
	ProcessInfo = (PPROCESSINFO)VirtualAlloc(0, sizeof(PROCESSINFO)*900,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (ProcessInfo)
	{
		memset(ProcessInfo,0,sizeof(PROCESSINFO)*900);
		ReadFile((HANDLE)LIST_PROCESS,ProcessInfo,sizeof(PROCESSINFO)*900,&dwReadByte,0);
		for ( i=0;i<ProcessInfo->ulCount;i++)
		{
			//过滤下A盾自己~
			if (GetCurrentProcessId() == ProcessInfo->ProcessInfo[i].ulPid)
				continue;

			Sleep(10);
			//读取第一个进程的所有Dll模块
			ReadFile((HANDLE)INIT_PROCESS_LIST_PROCESS_MODULE,0,ProcessInfo->ProcessInfo[i].EProcess,&dwReadByte,0);

			//清空这个内容，继续下一个
			memset(SearchDllModule,0,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810);
			ReadFile((HANDLE)LIST_PROCESS_MODULE,SearchDllModule,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810,&dwReadByte,0);
			for (x=0;x<SearchDllModule->ulCount;x++)
			{
				Sleep(10);

				memset(NumBer,0,sizeof(NumBer));
				wsprintfW(NumBer,L"%d",count);

				memset(lpwzEprocess,0,sizeof(lpwzEprocess));
				wsprintfW(lpwzEprocess,L"0x%08X",ProcessInfo->ProcessInfo[i].EProcess);

				memset(lpwzPid,0,sizeof(lpwzPid));
				wsprintfW(lpwzPid,L"%d",ProcessInfo->ProcessInfo[i].ulPid);

				bHadBeenSearched = FALSE;
				for (int y=0;y<BeenSearchedDllModule->ulCount;y++)
				{
					if (_wcsnicmp(BeenSearchedDllModule->DllInfo[y].lpwzDllModule,SearchDllModule->DllInfo[x].lpwzDllModule,wcslen(SearchDllModule->DllInfo[x].lpwzDllModule)) == 0)
					{
						bHadBeenSearched = TRUE;
						break;
					}
				}
				if (bHadBeenSearched)
					continue;

				memset(lpwzDLLPath,0,sizeof(lpwzDLLPath));

				if (wcsstr(SearchDllModule->DllInfo[x].lpwzDllModule,L"\\Device\\") != NULL)
				{
					//开始处理dos路径
					NtFilePathToDosFilePath(SearchDllModule->DllInfo[x].lpwzDllModule,lpwzDLLPath);
					goto Next;
				}
				if (*(SearchDllModule->DllInfo[x].lpwzDllModule) == '\\')
				{
					//获取dll路径
					PrintfDosPath(SearchDllModule->DllInfo[x].lpwzDllModule,lpwzDLLPath);
					goto Next;
				}
				if (!wcslen(lpwzDLLPath)){
					wcsncat(lpwzDLLPath,SearchDllModule->DllInfo[x].lpwzDllModule,wcslen(SearchDllModule->DllInfo[x].lpwzDllModule));
				}
Next:
				if (!VerifyEmbeddedSignature(lpwzDLLPath))
				{
					memset(lpwzFullProcPath,0,sizeof(lpwzFullProcPath));
					//处理进程路径
					if (wcsstr(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,L"\\Device\\") != NULL)
					{
						//开始处理dos路径
						NtFilePathToDosFilePath(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,lpwzFullProcPath);
					}else
					{
						wcsncat(lpwzFullProcPath,ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,wcslen(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath));
					}
					memset(lpwzBase,0,sizeof(lpwzBase));
				   wsprintfW(lpwzBase,L"0x%08X",SearchDllModule->DllInfo[x].ulBase);

					//这里是一键体检的数据，不需要插入界面了
					if (bIsPhysicalCheck){

						WCHAR lpwzSaveBuffer[1024] ={0};
						CHAR lpszSaveBuffer[2024] ={0};
						memset(lpwzSaveBuffer,0,sizeof(lpwzSaveBuffer));
						memset(lpszSaveBuffer,0,sizeof(lpszSaveBuffer));

						wsprintfW(lpwzSaveBuffer,L"          --> 发现未识别DLL模块:Base:%ws | DLL路径:%ws | DLL所在进程:%ws | 进程PID:%ws | EPROCESS:%ws\r\n",
							lpwzBase,lpwzDLLPath,lpwzFullProcPath,lpwzPid,lpwzEprocess);

						m_list->InsertItem(count,L"未识别DLL模块",RGB(77,77,77));
						m_list->SetItemText(count,1,lpwzSaveBuffer);

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

					count++;

				}
				memset(BeenSearchedDllModule->DllInfo[been].lpwzDllModule,0,sizeof(BeenSearchedDllModule));
				wcscat(BeenSearchedDllModule->DllInfo[been].lpwzDllModule,SearchDllModule->DllInfo[x].lpwzDllModule);
				BeenSearchedDllModule->ulCount = been;
				been++;
				//**********************************************
			}
		}
		//释放内存
		VirtualFree(ProcessInfo,sizeof(PROCESSINFO)*900,MEM_RESERVE | MEM_COMMIT);
	}
	//释放内存
	VirtualFree(SearchDllModule,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810,MEM_RESERVE | MEM_COMMIT);
	VirtualFree(BeenSearchedDllModule,(sizeof(BEENSEARCHEDDLLINFO)+sizeof(BEEN_SEARCHED_DLL_INFORMATION))*1024,MEM_RESERVE | MEM_COMMIT);
	SetEvent(g_hThreadEvent); //触发事件
}
void CCProcessSearch::SearchDllModuleInfo(CMyList *m_listInfo,int Type)
{
	DWORD dwReadByte;
	int i=0,x=0,y=0,been=0;
	int count=0;
	BOOL bHadBeenSearched = FALSE;
	WCHAR lpwzDLLPath[260];
	WCHAR lpwzMd5[260] = {0};
	WCHAR NumBer[10] = {0};
	WCHAR lpwzFullPath[260] = {0};
	WCHAR lpwzFullProcPath[260] = {0};
	WCHAR lpwzEprocess[260] = {0};
	WCHAR lpwzPid[10] = {0};
	WCHAR lpwzTrue[260] = {0};

	PBEENSEARCHEDDLLINFO BeenSearchedDllModule;
	PDllModuleINFO SearchDllModule;
	PPROCESSINFO ProcessInfo;

	BeenSearchedDllModule = (PBEENSEARCHEDDLLINFO)VirtualAlloc(0,(sizeof(BEENSEARCHEDDLLINFO)+sizeof(BEEN_SEARCHED_DLL_INFORMATION))*1024,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!BeenSearchedDllModule)
	{
		return;
	}
	SearchDllModule = (PDllModuleINFO)VirtualAlloc(0,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (!SearchDllModule)
	{
		return;
	}
	//先得到一次所有的进程列表
	ProcessInfo = (PPROCESSINFO)VirtualAlloc(0, sizeof(PROCESSINFO)*900,MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if (ProcessInfo)
	{
		memset(ProcessInfo,0,sizeof(PROCESSINFO)*900);
		ReadFile((HANDLE)LIST_PROCESS,ProcessInfo,sizeof(PROCESSINFO)*900,&dwReadByte,0);
		for ( i=0;i<ProcessInfo->ulCount;i++)
		{
			if (!bStopSearch){
				SetDlgItemTextA(hWndStatusOut,IDC_SearchStatus,"手动停止成功!");
				goto _Stop;
			}
			//过滤下A盾自己~
			if (GetCurrentProcessId() == ProcessInfo->ProcessInfo[i].ulPid)
				continue;

			Sleep(10);
			//读取第一个进程的所有Dll模块
			ReadFile((HANDLE)INIT_PROCESS_LIST_PROCESS_MODULE,0,ProcessInfo->ProcessInfo[i].EProcess,&dwReadByte,0);

			//清空这个内容，继续下一个
			memset(SearchDllModule,0,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810);
			ReadFile((HANDLE)LIST_PROCESS_MODULE,SearchDllModule,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810,&dwReadByte,0);
			for (x=0;x<SearchDllModule->ulCount;x++)
			{
				if (!bStopSearch){
					SetDlgItemTextA(hWndStatusOut,IDC_SearchStatus,"正在停止...");
					break;
				}
				Sleep(10);
				CHAR lpszTextOut[100];
				memset(lpszTextOut,0,sizeof(lpszTextOut));
				wsprintfA(lpszTextOut,"共有 %d 个进程，正在扫描第 %d 个进程的 第 [%d -> %d] 个DLL模块，目前共 %d 个DLL模块符合条件，请稍后...",
					ProcessInfo->ulCount,
					i,
					SearchDllModule->ulCount,
					x,
					count);

				SetDlgItemTextA(hWndStatusOut,IDC_SearchStatus,lpszTextOut);

				memset(NumBer,0,sizeof(NumBer));
				wsprintfW(NumBer,L"%d",count);

				memset(lpwzEprocess,0,sizeof(lpwzEprocess));
				wsprintfW(lpwzEprocess,L"0x%08X",ProcessInfo->ProcessInfo[i].EProcess);

				memset(lpwzPid,0,sizeof(lpwzPid));
				wsprintfW(lpwzPid,L"%d",ProcessInfo->ProcessInfo[i].ulPid);

				if (Type == 0){
					if (wcsstr(SearchDllModule->DllInfo[x].lpwzDllModule,SearchDLLString) != 0)
					{
						if (wcsstr(SearchDllModule->DllInfo[x].lpwzDllModule,L"\\Device\\") != NULL)
						{
							//开始处理dos路径
							NtFilePathToDosFilePath(SearchDllModule->DllInfo[x].lpwzDllModule,lpwzDLLPath);
						}
						memset(lpwzFullProcPath,0,sizeof(lpwzFullProcPath));
						//处理进程路径
						if (wcsstr(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,L"\\Device\\") != NULL)
						{
							//开始处理dos路径
							NtFilePathToDosFilePath(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,lpwzFullProcPath);
						}else
						{
							wcsncat(lpwzFullProcPath,ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,wcslen(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath));
						}
						m_listInfo->InsertItem(count,NumBer);
						m_listInfo->SetItemText(count,1,SearchDLLString);
						m_listInfo->SetItemText(count,2,lpwzDLLPath);
						m_listInfo->SetItemText(count,3,lpwzFullProcPath);
						m_listInfo->SetItemText(count,4,lpwzEprocess);
						m_listInfo->SetItemText(count,5,lpwzPid);
						m_listInfo->SetItemText(count,6,L"-");
						count++;
					}
				}else if (Type == 1)
				{
					bHadBeenSearched = FALSE;
					for (int y=0;y<BeenSearchedDllModule->ulCount;y++)
					{
						if (_wcsnicmp(BeenSearchedDllModule->DllInfo[y].lpwzDllModule,SearchDllModule->DllInfo[x].lpwzDllModule,wcslen(SearchDllModule->DllInfo[x].lpwzDllModule)) == 0)
						{
							bHadBeenSearched = TRUE;
							break;
						}
					}
					if (bHadBeenSearched)
						continue;

					memset(lpwzDLLPath,0,sizeof(lpwzDLLPath));

					if (wcsstr(SearchDllModule->DllInfo[x].lpwzDllModule,L"\\Device\\") != NULL)
					{
						//开始处理dos路径
						NtFilePathToDosFilePath(SearchDllModule->DllInfo[x].lpwzDllModule,lpwzDLLPath);
						goto Next;
					}
					if (*(SearchDllModule->DllInfo[x].lpwzDllModule) == '\\')
					{
						//获取dll路径
						PrintfDosPath(SearchDllModule->DllInfo[x].lpwzDllModule,lpwzDLLPath);
						goto Next;
					}
					if (!wcslen(lpwzDLLPath)){
						wcsncat(lpwzDLLPath,SearchDllModule->DllInfo[x].lpwzDllModule,wcslen(SearchDllModule->DllInfo[x].lpwzDllModule));
					}
Next:
					if (!VerifyEmbeddedSignature(lpwzDLLPath))
					{
						memset(lpwzFullProcPath,0,sizeof(lpwzFullProcPath));
						//处理进程路径
						if (wcsstr(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,L"\\Device\\") != NULL)
						{
							//开始处理dos路径
							NtFilePathToDosFilePath(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,lpwzFullProcPath);
						}else
						{
							wcsncat(lpwzFullProcPath,ProcessInfo->ProcessInfo[i].lpwzFullProcessPath,wcslen(ProcessInfo->ProcessInfo[i].lpwzFullProcessPath));
						}
						m_listInfo->InsertItem(count,NumBer);
						m_listInfo->SetItemText(count,1,L"-");
						m_listInfo->SetItemText(count,2,lpwzDLLPath);
						m_listInfo->SetItemText(count,3,lpwzFullProcPath);
						m_listInfo->SetItemText(count,4,lpwzEprocess);
						m_listInfo->SetItemText(count,5,lpwzPid);
						m_listInfo->SetItemText(count,6,L"-");
						count++;

// 						WCHAR lpwzTextOut[100];
// 						memset(lpwzTextOut,0,sizeof(lpwzTextOut));
// 						wsprintfW(lpwzTextOut,L"[%d]%ws",been,lpwzDLLPath);
// 						OutputDebugStringW(lpwzTextOut);

					}
					memset(BeenSearchedDllModule->DllInfo[been].lpwzDllModule,0,sizeof(BeenSearchedDllModule));
					wcscat(BeenSearchedDllModule->DllInfo[been].lpwzDllModule,SearchDllModule->DllInfo[x].lpwzDllModule);
					BeenSearchedDllModule->ulCount = been;
					been++;
					//**********************************************
				}

			}
		}
_Stop:
		CHAR lpszTextOut[100];
		memset(lpszTextOut,0,sizeof(lpszTextOut));
		wsprintfA(lpszTextOut,"扫描完毕，共有 %d 个数据符合要求",count);
		SetDlgItemTextA(hWndStatusOut,IDC_SearchStatus,lpszTextOut);
		bStopSearch = FALSE;

		//释放内存
		VirtualFree(ProcessInfo,sizeof(PROCESSINFO)*900,MEM_RESERVE | MEM_COMMIT);
	}
	//释放内存
	VirtualFree(SearchDllModule,(sizeof(DllModuleINFO)+sizeof(SearchDllModule_INFORMATION))*810,MEM_RESERVE | MEM_COMMIT);
	VirtualFree(BeenSearchedDllModule,(sizeof(BEENSEARCHEDDLLINFO)+sizeof(BEEN_SEARCHED_DLL_INFORMATION))*1024,MEM_RESERVE | MEM_COMMIT);
}
DWORD WINAPI SearchDllModuleInfoFunction0(CMyList *m_list)
{
	CCProcessSearch Search;
	Search.SearchDllModuleInfo(m_list,0);
	return 0;
}
DWORD WINAPI SearchDllModuleInfoFunction1(CMyList *m_list)
{
	CCProcessSearch Search;
	Search.SearchDllModuleInfo(m_list,1);
	return 0;
}
void CCProcessSearch::OnBnClickedStopSearch()
{
	bStopSearch = FALSE;
}
void CCProcessSearch::OnBnClickedBtnSearch()
{
	// TODO: 在此添加控件通知处理程序代码
	static int i=0;
	UpdateData(true);

	if (bStopSearch == 5){
		MessageBoxW(L"请先停止本次扫描！",L"A盾电脑防护",MB_ICONWARNING);
		return;
	}

	bStopSearch = 5;
	switch(idFrom)
	{
	case 1:
		memset(SearchDLLString,0,sizeof(SearchDLLString));
		wcscat(SearchDLLString,m_strFindInfo);
		if (!wcslen(SearchDLLString)){
			break;
		}
		m_listFindInfo.DeleteAllItems();
		CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SearchDllModuleInfoFunction0,&m_listFindInfo,0,NULL));
		//SearchDllModuleInfo(&m_listFindInfo,0); 
		//SetTimer(1,100,NULL);
		break;
	case 2:
		m_listFindInfo.DeleteAllItems();
		CloseHandle(CreateThread(NULL,0,(LPTHREAD_START_ROUTINE)SearchDllModuleInfoFunction1,&m_listFindInfo,0,NULL));
		//SetTimer(2,100,NULL);
		break;
	}
}

/*void CCProcessSearch::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch(nIDEvent)
	{
	case 1:
		memset(SearchDLLString,0,sizeof(SearchDLLString));
		wcscat(SearchDLLString,m_strFindInfo);
		if (!wcslen(SearchDLLString)){
			break;
		}
		m_listFindInfo.DeleteAllItems();
		SearchDllModuleInfo(&m_listFindInfo,0); //查找某个dll
		KillTimer(1);
		break;
	case 2:
		m_listFindInfo.DeleteAllItems();
		SearchDllModuleInfo(&m_listFindInfo,1); //查找没通过数字签名的
		KillTimer(2);
		break;
	}
	CDialogEx::OnTimer(nIDEvent);
}*/
