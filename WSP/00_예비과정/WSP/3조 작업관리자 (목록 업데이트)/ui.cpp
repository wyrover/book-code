//ui.cpp
#include "std.h"
	

void ui_InitList(HWND hDlg)
{
	HWND hList = GetDlgItem(hDlg,IDC_LIST);	

	LVCOLUMN COL;
		
	COL.mask=LVCF_FMT | LVCF_WIDTH | LVCF_TEXT |LVCF_SUBITEM;
	COL.fmt=LVCFMT_LEFT;
	COL.cx=150;
	COL.pszText="프로세스 이름";			// 첫번째 헤더
	COL.iSubItem=PNAME;
	ListView_InsertColumn(hList,PNAME,&COL);

	COL.pszText="PID";		// 두번째 헤더
	COL.iSubItem=PID;
	ListView_InsertColumn(hList,PID,&COL);

	COL.cx=130;
	COL.pszText="스레드";			// 세번째 헤더
	COL.iSubItem=PTHREED;
	ListView_InsertColumn(hList,PTHREED,&COL);

	ListView_SetExtendedListViewStyle(hList, LVS_EX_FULLROWSELECT |	LVS_EX_GRIDLINES);
	ui_ViewProcess(hList);

}


void ui_ViewProcess(HWND hList)
{

	HANDLE	hSnap = CreateToolhelp32Snapshot(
						TH32CS_SNAPPROCESS, 0);	// process ID // 0은 시스템적 모든 프로세스

	if( hSnap == 0 )
		return ;

	LVITEM LI;

	char Pid[100];
	char Thread[100];

	PROCESSENTRY32	ppe;

	BOOL	b = Process32First(hSnap, &ppe);

	while ( b )
	{
		int i = 0;

		sprintf(Pid, "%d", ppe.th32ProcessID);
		sprintf(Thread, "%d", ppe.cntThreads);

		LI.mask=LVIF_TEXT ;//| LVIF_IMAGE;
		LI.iItem=i;
		LI.iSubItem=0;
		LI.pszText=ppe.szExeFile;

		ListView_InsertItem(hList,&LI);
		ListView_SetItemText(hList,i,1,Pid);
		ListView_SetItemText(hList,i,2,Thread);
		b = Process32Next( hSnap, &ppe );

		i++;
	}
	CloseHandle(hSnap);	

}

/*
LVCOLUMN ui_ListAdd(HWND hList,TCHAR* str,HTREEITEM pos)
{
	LVITEM TI;

	return (LVCOLUMN)SendMessage(hList,LVM_INSERTITEM,0,(LPARAM)&TI);
}*/