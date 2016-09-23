#include "stdafx.h"
#include "AntiSpy.h"
#include "ProcessManager.h"
#include "ProcessModule.h"
#include "ProcessHandle.h"
#include "ProcessWind.h"
#include "ProcessMemory.h"
#include "ProcessThread.h"
#include "afxdialogex.h"

//获得文件厂商所需头文件

#include <WinVer.h>
#include <Strsafe.h>
#pragma comment(lib,"Version.lib")


CStatusBar  m_wndStatusBar;
	static UINT indicators[] =
{
	IDR_STATUSBAR_STRING
};

CProcessManager* g_dlg;
ULONG g_ProcessCount = 0;       
ULONG g_HideProcessCount = 0;
ULONG g_OpenProcessFailedCount = 0;
BOOL  bSort = FALSE;
IMPLEMENT_DYNAMIC(CProcessManager, CDialog)

CProcessManager::CProcessManager(CWnd* pParent)
	: CDialog(CProcessManager::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_NOR);
}

CProcessManager::~CProcessManager()
{
	m_Vector.clear();
}

void CProcessManager::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_List);
}

BEGIN_MESSAGE_MAP(CProcessManager, CDialog)
	ON_NOTIFY(NM_RCLICK, IDC_LIST, &CProcessManager::OnNMRClickList)
	ON_COMMAND(ID_PROCESS_MODULE, &CProcessManager::OnProcessModule)
	ON_WM_CTLCOLOR()
	ON_NOTIFY (NM_CUSTOMDRAW, IDC_LIST, OnCustomdrawMyList )
	ON_COMMAND(ID_PROCESS_REFRESH, &CProcessManager::OnProcessRefresh)
	ON_COMMAND(ID_PROCESS_LOCATION_FILE, &CProcessManager::OnProcessLocationFile)
	ON_COMMAND(ID_PROCESS_COPY_PROCESSNAME, &CProcessManager::OnProcessCopyProcessname)
	ON_COMMAND(ID_PROCESS_COPY_PROCESS_IMAGE_NAME, &CProcessManager::OnProcessCopyProcessImageName)
	ON_COMMAND(ID_PROCESS_COPY_EPROCESS, &CProcessManager::OnProcessCopyEprocess)
	ON_COMMAND(ID_PROCESS_HANDLE, &CProcessManager::OnProcessHandle)
	ON_NOTIFY(HDN_ITEMCLICK, 0, &CProcessManager::OnItemclickList)
	ON_COMMAND(ID_PROCESS_CLOSE_PROCESS, &CProcessManager::OnProcessCloseProcess)
	ON_COMMAND(ID_PROCESS_CLOSE_FORCED, &CProcessManager::OnProcessCloseForced)
	ON_COMMAND(ID_PROCESS_CLOSE_DELETE, &CProcessManager::OnProcessCloseDelete)
	ON_COMMAND(ID_PROCESS_WIND, &CProcessManager::OnProcessWind)
	ON_COMMAND(ID_PROCESS_THREAD, &CProcessManager::OnProcessThread)
	ON_COMMAND(ID_PROCESS_MEMORY, &CProcessManager::OnProcessMemory)
END_MESSAGE_MAP()

VOID CProcessManager::InitControl()
{
	m_List.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_List.InsertColumn(0,L"映像名称",LVCFMT_LEFT,110);
	m_List.InsertColumn(1,L"进程ID",LVCFMT_CENTER,70);
	m_List.InsertColumn(2,L"父进程ID",LVCFMT_CENTER,70);
	m_List.InsertColumn(3,L"映像路径",LVCFMT_LEFT,310);
	m_List.InsertColumn(4,L"EPROCESS",LVCFMT_CENTER,90);
	m_List.InsertColumn(5,L"应用层访问状态",LVCFMT_CENTER,120);
	m_List.InsertColumn(6,L"文件厂商",LVCFMT_LEFT,150);

	SetWindowLong(m_List.m_hWnd ,GWL_EXSTYLE,WS_EX_CLIENTEDGE);
	m_List.SetExtendedStyle(LVS_EX_GRIDLINES);                   			  
	::SendMessage(m_List.m_hWnd, LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT, LVS_EX_FULLROWSELECT);

	m_ControlImageList.Create(16, 16, ILC_COLOR32 | ILC_MASK, 2, 2);
	ListView_SetImageList(m_List.m_hWnd, m_ControlImageList.GetSafeHandle(), LVSIL_SMALL);

	
}

void CProcessManager::OnItemclickList(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLISTVIEW *pLV = (NMLISTVIEW *) pNMHDR;

	g_dlg = this;

	if(bSort==FALSE)
	{
		bSort = TRUE;
		m_List.SortItems(SortFuncByIncrease, pLV->iItem);
	}
	else
	{
		bSort = FALSE;
		m_List.SortItems(SortFuncByDecrease, pLV->iItem);
	}


	*pResult = 0;
}


int CALLBACK SortFuncByIncrease(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString strItem1, strItem2;
	LVFINDINFO info;
	int nIndex;
	

	info.flags  = LVFI_PARAM;

	info.lParam = lParam1;

	if ( (nIndex = g_dlg->m_List.FindItem(&info))!=-1)
	{
		strItem1 = g_dlg->m_List.GetItemText(nIndex,(int)lParamSort);
	}

	info.lParam = lParam2;

	if ( (nIndex = g_dlg->m_List.FindItem(&info))!=-1)
	{
		strItem2 = g_dlg->m_List.GetItemText(nIndex,(int)lParamSort);
	}

	switch(lParamSort)
	{
	case 0:
		{
			return strItem1.Compare(strItem2);
			break;
		}
	case 1:
		{
			int r1=_ttoi(strItem1);
			int r2=_ttoi(strItem2);

			if (r1==r2)
			{
				return 0;
			}
			else
			{
				if (r1>r2)
				{
					return 1;
				}
				else
				{
					return -1;
				}
			}
			break;
		}
	case 2:
		{
			int r1=_ttoi(strItem1);
			int r2=_ttoi(strItem2);

			if (r1==r2)
			{
				return 0;
			}
			else
			{
				if (r1>r2)
				{
					return 1;
				}
				else
				{
					return -1;
				}
			}
			break;
		}
	case 3:
		{
			return strItem1.Compare(strItem2);
			break;
		}
	case 4:
		{
			return strItem1.Compare(strItem2);
			break;
		}
	case 6:
		{
			return strItem1.Compare(strItem2);
			break;
		}
	}
}


int CALLBACK SortFuncByDecrease(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
	CString strItem1, strItem2;
	LVFINDINFO info;
	int nIndex;


	info.flags  = LVFI_PARAM;

	info.lParam = lParam1;

	if ( (nIndex = g_dlg->m_List.FindItem(&info))!=-1)
	{
		strItem1 = g_dlg->m_List.GetItemText(nIndex,(int)lParamSort);
	}

	info.lParam = lParam2;

	if ( (nIndex = g_dlg->m_List.FindItem(&info))!=-1)
	{
		strItem2 = g_dlg->m_List.GetItemText(nIndex,(int)lParamSort);
	}

	switch(lParamSort)
	{
	case 0:
		{
			return strItem1.Compare(strItem2)*-1;
			break;
		}
	case 1:
		{
			int r1=_ttoi(strItem1);
			int r2=_ttoi(strItem2);

			if (r1==r2)
			{
				return 0;
			}
			else
			{
				if (r1>r2)
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}
			break;
		}
	case 2:
		{
			int r1=_ttoi(strItem1);
			int r2=_ttoi(strItem2);

			if (r1==r2)
			{
				return 0;
			}
			else
			{
				if (r1>r2)
				{
					return -1;
				}
				else
				{
					return 1;
				}
			}
			break;
		}
	case 3:
		{
			return strItem1.Compare(strItem2)*-1;
			break;
		}
	case 4:
		{
			return strItem1.Compare(strItem2)*-1;
			break;
		}
	case 6:
		{
			return strItem1.Compare(strItem2)*-1;
			break;
		}
	}
}

void CProcessManager::OnCustomdrawMyList ( NMHDR* pNMHDR, LRESULT* pResult )
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );

	if ( CDDS_PREPAINT == pLVCD->nmcd.dwDrawStage )
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if ((CDDS_ITEMPREPAINT) ==pLVCD->nmcd.dwDrawStage )
	{
		COLORREF crText;

		crText = RGB(0,0,0);

		CString str = m_List.GetItemText(pLVCD->nmcd.dwItemSpec,6);

		if(wcscmp(str.GetBuffer(),L"Microsoft Corporation")!=0)
		{
			crText = RGB(0,0,255);
		}

		if (str.GetLength()==0)
		{
			crText = RGB(0,0,0);
		}

		for(vector <PROCESS_INFO>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )

		{
			if(wcscmp((*Iter).wzProcessName,m_List.GetItemText(pLVCD->nmcd.dwItemSpec,0))==0)
			{
				if((*Iter).bHide==TRUE)
				{
					crText = RGB(255,0,0);
				}
			}
		}

		pLVCD->clrText = crText;	

		*pResult = CDRF_DODEFAULT;
	}
}

BOOL CProcessManager::OnInitDialog()
{
	CDialog::OnInitDialog();


	InitControl();

	ULONG_PTR  ulControlCode =FORCE_ENUM_PROCESS;
	SendIoControlCode(ulControlCode);


	CreatStatusBar();
	
	return TRUE;  
}

void CProcessManager::CreatStatusBar(void)
{
	RECT	rect;
	GetClientRect(&rect);

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators)/sizeof(UINT)))
	{
		return ;      
	}

	m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_STRETCH, NULL);
	
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0); //显示状态栏
	

	CRect rc;
	rc.top=rect.bottom-22;
	rc.left=0;
	rc.right=rect.right;
	rc.bottom=rect.bottom;

	m_wndStatusBar.MoveWindow(rc);           

	CString strStatusMsg;
	strStatusMsg.Format(L"进程：%d ， 隐藏进程：%d ， 应用层不可访问进程：%d",g_ProcessCount,g_HideProcessCount,1);
	m_wndStatusBar.SetPaneText(0,strStatusMsg);  
}



BOOL CProcessManager::SendIoControlCode(ULONG_PTR ulControlCode)
{
	if (ulControlCode==FORCE_ENUM_PROCESS)
	{
		EnumProcess();

		if (m_Vector.empty())
		{
			return FALSE;
		}


		g_ProcessCount = m_Vector.size();

		if(g_ProcessCount!=0)
		{
			for(int i=0;i<g_ProcessCount;i++)
			{
				m_ControlImageList.Remove(0);
			}
		}
	
		for (vector <PROCESS_INFO>::iterator Iter = m_Vector.begin( ); 
			Iter != m_Vector.end( ); 
			Iter++ )
		{
			HICON hIcon = GetProcessIcon((*Iter).wzProcessFileName);

			AddItemToControlList(*Iter,m_ControlImageList.Add(hIcon));
					
		}
	}
	else if(ulControlCode==KILL_PROCESS)
	{
		KillProcess();
	}
	else if(ulControlCode==KILL_PROCESS_FORCED)
	{
		KillProcessByForce();
	}
	else if(ulControlCode==KILL_PROCESS_AND_DELETE_FILE)
	{
		KillProcessAndDeleteFile();
	}
	return TRUE;
}

VOID CProcessManager::KillProcessAndDeleteFile()
{
	CString   csTemp;
	BOOL      bRet = FALSE;
	DWORD     dwReturnSize = 0;

	struct _DELETE_DATA_
	{
		ULONG_PTR ulProcessId;
		WCHAR     wzImagePath[MAX_PATH];
	}Data;

	memset(&Data,0,sizeof(Data));

	POSITION pos = m_List.GetFirstSelectedItemPosition();  

	while(pos) 
	{
		int	nItem = m_List.GetNextSelectedItem(pos); 

		csTemp = m_List.GetItemText(nItem,1);

		Data.ulProcessId = _ttol(csTemp);

		csTemp = m_List.GetItemText(nItem,3);

		memcpy(Data.wzImagePath,csTemp.GetBuffer(),(csTemp.GetLength()+1)*sizeof(WCHAR));

		bRet = DeviceIoControl(g_hDevice,CTL_KILLPROCESS_AND_FILE,
			&Data,
			sizeof(_DELETE_DATA_),
			NULL,
			0,
			&dwReturnSize,
			NULL);

		if(bRet)
		{
			m_List.DeleteItem(nItem);
		}
	}

}

VOID CProcessManager::KillProcessByForce()
{
	CString   csTemp;
	ULONG_PTR ulProcessId = 0;
	BOOL      bRet = FALSE;
	BOOL      bOk =  FALSE;
	DWORD     dwReturnSize = 0;

	POSITION pos = m_List.GetFirstSelectedItemPosition();  

	while(pos) 
	{
		int	nItem = m_List.GetNextSelectedItem(pos); 

		csTemp = m_List.GetItemText(nItem,1);

		ulProcessId = _ttol(csTemp);

		bRet = DeviceIoControl(g_hDevice,CTL_FORCE_KILL_PROCESS,
			&ulProcessId,
			sizeof(ULONG_PTR),
			NULL,
			0,
			&dwReturnSize,
			NULL);

		if(bRet)
		{
			m_List.DeleteItem(nItem);
		}
	}
}

VOID CProcessManager::KillProcess()
{
	CString   csTemp;
	ULONG_PTR ulProcessId = 0;
	BOOL      bRet = FALSE;
	BOOL      bOk =  FALSE;
	DWORD     dwReturnSize = 0;

	POSITION pos = m_List.GetFirstSelectedItemPosition();  

	while(pos) 
	{
		int	nItem = m_List.GetNextSelectedItem(pos); 

		csTemp = m_List.GetItemText(nItem,1);

		ulProcessId = _ttol(csTemp);

		bRet = DeviceIoControl(g_hDevice,CTL_KILLPROCESS,
			&ulProcessId,
			sizeof(ULONG_PTR),
			NULL,
			0,
			&dwReturnSize,
			NULL);

		if(bRet)
		{
			m_List.DeleteItem(nItem);
		}
	}

}

BOOL CProcessManager::EnableDebugPri()
{
	HANDLE hToken;
	TOKEN_PRIVILEGES pTP;
	LUID uID;

	if (!OpenProcessToken(GetCurrentProcess(),
		TOKEN_ADJUST_PRIVILEGES|TOKEN_QUERY,&hToken))   
	{
		printf("OpenProcessToken is Error\n");

		return false;
	}

	if (!LookupPrivilegeValue(NULL,SE_DEBUG_NAME,&uID))   //调式
	{
		printf("LookupPrivilegeValue is Error\n");

		return false;
	}

	pTP.PrivilegeCount = 1;
	pTP.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	pTP.Privileges[0].Luid = uID;

	if (!AdjustTokenPrivileges(hToken,false,&pTP,sizeof(TOKEN_PRIVILEGES),NULL,NULL))
	{
		printf("AdjuestTokenPrivileges is Error\n");
		return  false;
	}
	return true;
}



VOID CProcessManager::EnumProcess()
{
    g_ProcessCount = 0;   
	g_HideProcessCount = 0;
	g_OpenProcessFailedCount = 0;

	ULONG       i = 0;
	ULONG       j = 0;
	BOOL        bRet = FALSE;
	DWORD       dwReturn = 0;
	ULONG       ulProcessCount = 0;
	ULONG_PTR   ulCount = 0x1000;
	DWORD       ulReturnSize = 0;      
	PRTL_PROCESS_INFORMATION PProcessInfor = NULL;

	BOOL bOk = FALSE;

	m_Vector.clear();

	m_List.DeleteAllItems();

	do 
	{
		ULONG_PTR ulSize = 0;

		if (PProcessInfor)
		{
			free(PProcessInfor);
			PProcessInfor = NULL;
		}

		ulSize = sizeof(RTL_PROCESS_INFORMATION) + ulCount * sizeof(PROCESS_INFO);

		PProcessInfor = (PRTL_PROCESS_INFORMATION)malloc(ulSize);
		if (PProcessInfor==NULL)
		{
			break;
		}

		memset(PProcessInfor,0,ulSize);

		bRet = DeviceIoControl(g_hDevice,IOCTL_ENUMPROCESSINFOR,
			NULL,
			0,
			PProcessInfor,
			ulSize,
			&ulReturnSize,
			NULL);

		ulCount = PProcessInfor->NumberOfProcess + 1000;

	} while (bRet == FALSE && GetLastError() == ERROR_INSUFFICIENT_BUFFER);

	if (bRet && PProcessInfor)
	{
		for (ULONG i = 0; i < PProcessInfor->NumberOfProcess; i++)
		{
			CString strTemp; 
			FixPath(PProcessInfor->Process[i].wzProcessFileName);
			strTemp = GetFileCompanyName(PProcessInfor->Process[i].wzProcessFileName);

			wcscpy(PProcessInfor->Process[i].wzCompanyName,strTemp.GetBuffer());

			m_Vector.push_back(PProcessInfor->Process[i]);
		}
		bOk = TRUE;
	}

	if (PProcessInfor)
	{
		free(PProcessInfor);
		PProcessInfor = NULL;
	}

	FindTheHideProcess();

	return ;
}

VOID CProcessManager::FindTheHideProcess()
{
	ULONG       i = 0;
	HANDLE      hProcess = NULL;
	
	BOOL        bRet = FALSE;
	DWORD       dwReturn = 0;

	int         iHideProcessCount = 0;

	EnableDebugPri();
	
	for ( i= 4;i<100000;i+=4)      
	{
		BOOL        bHide = TRUE;
		hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,i);   
		if (hProcess!=0)   
		{
			CloseHandle(hProcess);

			for (vector <PROCESS_INFO>::iterator Iter = m_Vector.begin( ); 
				Iter != m_Vector.end( ); 
				Iter++ )
			{
				if (i==Iter->ulProcessID)
				{
					bHide = FALSE;
				}
			}

			if(bHide==TRUE)
			{
				PROCESS_INFO ProcessInfor = {0} ;

				bRet = DeviceIoControl(g_hDevice,IOCTL_ENUMHIDEPROCESSINFOR,
					&i,
					sizeof(ULONG),
					&ProcessInfor,
					sizeof(PROCESS_INFO),
					&dwReturn,
					NULL);

				if(bRet&&wcslen(ProcessInfor.wzProcessName)!=0)
				{
					CString strTemp; 
					FixPath(ProcessInfor.wzProcessFileName);
					strTemp = GetFileCompanyName(ProcessInfor.wzProcessFileName);

					wcscpy(ProcessInfor.wzCompanyName,strTemp.GetBuffer());

					m_Vector.push_back(ProcessInfor);

					iHideProcessCount++;
					
				}
			}	
		}
	}              
	g_HideProcessCount = iHideProcessCount;	
}

VOID CProcessManager::AddItemToControlList(PROCESS_INFO ProcessInfor,ULONG ulIndex)
{
	CString strProcessID, strEProcess, strProcessParentID;
	strProcessID.Format(L"%d", ProcessInfor.ulProcessID);
	strEProcess.Format(L"0x%p", ProcessInfor.ulEProcess);
	strProcessParentID.Format(L"%d", ProcessInfor.ulProcessParentID);

	int n = m_List.InsertItem(m_List.GetItemCount(),  ProcessInfor.wzProcessName,ulIndex);

	if(ProcessInfor.bHide==TRUE)
	{
		m_List.SetItemData(n,1);
	}

	HANDLE hProcess = OpenProcess(PROCESS_QUERY_INFORMATION,FALSE,ProcessInfor.ulProcessID);   
	if (hProcess!=0) 
	{
		m_List.SetItemText(n, 5, L"-");
	}
	else
	{
		m_List.SetItemText(n, 5, L"拒绝");
		g_OpenProcessFailedCount++;
		
	}


	LV_ITEM lvitem;
	lvitem.mask=LVIF_PARAM;
	lvitem.iItem = n;       
	lvitem.lParam= n;
	m_List.SetItem(&lvitem);
	
	m_List.SetItemText(n, 1, strProcessID);
	m_List.SetItemText(n, 2, strProcessParentID);
	m_List.SetItemText(n, 3, ProcessInfor.wzProcessFileName);
	m_List.SetItemText(n, 4, strEProcess);
	m_List.SetItemText(n, 6, ProcessInfor.wzCompanyName);
}

HICON CProcessManager::GetProcessIcon(WCHAR* wzProcessPath)
{
	if (!PathFileExists(wzProcessPath))
	{
		return m_hIcon;
	}

	SHFILEINFO shInfo;
	memset(&shInfo, 0, sizeof(shInfo));
	SHGetFileInfo(wzProcessPath, 0, &shInfo, sizeof(SHFILEINFO), 
		SHGFI_ICON |  SHGFI_SMALLICON );

	m_ControlImageList.Add(shInfo.hIcon);

	return shInfo.hIcon;
	
}


CString CProcessManager::GetFileCompanyName(CString strPath)
{
	CString strCompanyName = 0;;

	if (strPath.IsEmpty())
	{
		return NULL;
	}
	if (!PathFileExists(strPath))
	{	
		return strCompanyName;
	}

	struct LANGANDCODEPAGE {
		WORD wLanguage;
		WORD wCodePage;
	} *lpTranslate;

	LPWSTR lpstrFilename = (LPWSTR)(LPCWSTR)strPath;  
	DWORD dwHandle = 0;
	DWORD dwVerInfoSize = GetFileVersionInfoSize(lpstrFilename, &dwHandle);

	 // Dos FileHead    
	//[][][][][][][][]   [][][][]

	if (dwVerInfoSize)
	{
		LPVOID Buffer = LocalAlloc(LMEM_ZEROINIT, dwVerInfoSize);  //1852

		if (Buffer)
		{
			if (GetFileVersionInfo(lpstrFilename, dwHandle, dwVerInfoSize, Buffer))
			{
				UINT cbTranslate = 0;

				if ( VerQueryValue(Buffer, L"\\VarFileInfo\\Translation", (LPVOID*) &lpTranslate, &cbTranslate))
				{                
					LPCWSTR lpwszBlock = 0;          
					UINT    cbSizeBuf  = 0;
					WCHAR   wzSubBlock[MAX_PATH] = {0};  //260

					if ((cbTranslate/sizeof(struct LANGANDCODEPAGE)) > 0)   
					{
						StringCchPrintf(wzSubBlock, sizeof(wzSubBlock)/sizeof(WCHAR), 
							L"\\StringFileInfo\\%04x%04x\\CompanyName", lpTranslate[0].wLanguage, lpTranslate[0].wCodePage); 
					}

					//"\\VarFileInfo\\Translation   

					if ( VerQueryValue(Buffer, wzSubBlock, (LPVOID*)&lpwszBlock, &cbSizeBuf))
					{
						WCHAR wzCompanyName[MAX_PATH] = {0};

						StringCchCopy(wzCompanyName, MAX_PATH/sizeof(WCHAR), (LPCWSTR)lpwszBlock);   
						strCompanyName = wzCompanyName;
					}
				}
			}

			LocalFree(Buffer);
		}
	}

	return strCompanyName;
}


VOID CProcessManager::FixPath(WCHAR* wzImagePathName)
{
	WCHAR wzBuffer[512] = {0};
	WCHAR wzDrive[26][50] = {0};
	int BlockCount = 0;
	DWORD dwNum = GetLogicalDriveStrings(512,wzBuffer);   //c:\nulld:\nulle:\nullf:\nullnull

	if (dwNum!=0)
	{
		WCHAR* Temp = wzBuffer;
		while (*Temp!=0)
		{
			wcscpy(wzDrive[BlockCount++],Temp);
			Temp=_tcschr(Temp,0)+1;
		}
	}

	for(int i=0;i<BlockCount;i++)
	{
		WCHAR wzTemp[MAX_PATH] = L"\\Device\\HarddiskVolume";
		WCHAR  wzNum[2] = {0};
		 _itow( i,wzNum, 10 );
		wcscat(wzTemp,wzNum);

		if(wcsstr(wzImagePathName,wzTemp)!=NULL)
		{			
			wcscat(wzDrive[i],wzImagePathName+wcslen(wzTemp)+1);
			memset(wzImagePathName,0,wcslen(wzImagePathName)*sizeof(WCHAR));
			wcscpy(wzImagePathName,wzDrive[i]);
			break;
		}

	}

}

void CProcessManager::OnNMRClickList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	

	CMenu	popup;
	popup.LoadMenu(IDR_PROCESS_MENU);               
	CMenu*	pM = popup.GetSubMenu(0);                 
	CPoint	p;
	GetCursorPos(&p);
	int	count = pM->GetMenuItemCount();

	if (m_List.GetSelectedCount() == 0)         
	{ 
		for (int i = 0;i<count;i++)
		{
			if(i!=2)
			{
				pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);          
			}
		}

	}

	pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);

	*pResult = 0;
}

void CProcessManager::OnProcessModule()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  

	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		swscanf(m_List.GetItemText(nItem,1),L"%d",&m_ProcessID);
		m_ProcessName = m_List.GetItemText(nItem,0);

		CProcessModule Dlg(this);
		Dlg.DoModal();
	} 
	
}


void CProcessManager::OnProcessCloseProcess()
{
	ULONG_PTR ulControlCode = KILL_PROCESS;
	SendIoControlCode(ulControlCode);
}


void CProcessManager::OnProcessCloseForced()
{
	ULONG_PTR ulControlCode = KILL_PROCESS_FORCED;
	SendIoControlCode(ulControlCode);
}

void CProcessManager::OnProcessCloseDelete()
{
	ULONG_PTR ulControlCode = KILL_PROCESS_AND_DELETE_FILE;
	SendIoControlCode(ulControlCode);
}

void CProcessManager::OnProcessRefresh()
{
	ULONG_PTR  ulControlCode =FORCE_ENUM_PROCESS;
	SendIoControlCode(ulControlCode);

	CString strStatusMsg;
	strStatusMsg.Format(L"进程：%d ， 隐藏进程：%d ， 应用层不可访问进程：%d",g_ProcessCount,g_HideProcessCount,g_OpenProcessFailedCount);
	m_wndStatusBar.SetPaneText(0,strStatusMsg);  
}

void CProcessManager::OnProcessLocationFile()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  
	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		LocationExplorer(m_List.GetItemText(nItem,3));
	} 
}

VOID CProcessManager::LocationExplorer(CString strPath)
{
	if (!strPath.IsEmpty() && PathFileExists(strPath))
	{
		CString strCmd;
		strCmd.Format(L"/select,%s", strPath);
		ShellExecuteW(NULL, L"open", L"explorer.exe", strCmd, NULL, SW_SHOW);
	}
}

void CProcessManager::OnProcessCopyProcessname()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  
	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		CopyData(m_List.GetItemText(nItem,0));
	} 
}

void CProcessManager::OnProcessCopyProcessImageName()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  
	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		CopyData(m_List.GetItemText(nItem,3));
	} 
}

void CProcessManager::OnProcessCopyEprocess()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  
	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		CopyData(m_List.GetItemText(nItem,4));
	} 
}

void CProcessManager::CopyData(CString strhSelectData)
{
	if(OpenClipboard())   
	{   
		HGLOBAL   ClipBuffer;   
		CHAR*     Buffer;  
		DWORD     dwLen = 0;


		dwLen = WideCharToMultiByte(CP_ACP,NULL,strhSelectData.GetBuffer(),-1,NULL,0,NULL,FALSE); 

		char*	szBuffer = new char[dwLen];

		WideCharToMultiByte(CP_OEMCP,NULL,strhSelectData.GetBuffer(),-1,szBuffer,dwLen,NULL,FALSE);  

		EmptyClipboard();  


		ClipBuffer   =   GlobalAlloc(GMEM_DDESHARE,(strhSelectData.GetLength()+1)*sizeof(WCHAR));   
		Buffer		 =   (char*)GlobalLock(ClipBuffer);   
		strcpy(Buffer, szBuffer);

		GlobalUnlock(ClipBuffer);  
		SetClipboardData(CF_TEXT,ClipBuffer);   
		CloseClipboard();   

		if(szBuffer!=NULL)
		{
			delete szBuffer;
			szBuffer = NULL;
		}
	}   
}

void CProcessManager::OnProcessHandle()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  

	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 

		swscanf(m_List.GetItemText(nItem,1),L"%d",&m_ProcessID);

		m_ProcessName = m_List.GetItemText(nItem,0);

		CProcessHandle Dlg(this);
		Dlg.DoModal();
	} 
}



void CProcessManager::OnProcessWind()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  

	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 

		swscanf(m_List.GetItemText(nItem,1),L"%d",&m_ProcessID);

		m_ProcessName = m_List.GetItemText(nItem,0);

		CProcessWind Dlg(this);
		Dlg.DoModal();
	} 
}


void CProcessManager::OnProcessThread()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  

	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		swscanf(m_List.GetItemText(nItem,1),L"%d",&m_ProcessID);
		m_ProcessName = m_List.GetItemText(nItem,0);

		CProcessThread Dlg(this);
		Dlg.DoModal();
	} 
	
}


void CProcessManager::OnProcessMemory()
{
	POSITION Position = m_List.GetFirstSelectedItemPosition();  
	while(Position) 
	{
		int nItem = m_List.GetNextSelectedItem(Position); 
		swscanf(m_List.GetItemText(nItem,1),L"%d",&m_ProcessID);
		m_ProcessName = m_List.GetItemText(nItem,0);
		CProcessMemory Dlg(this);
		Dlg.DoModal();
	} 

}
