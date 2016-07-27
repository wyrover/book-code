// ListView.cpp
#include "std.h"

HWND InitListView(HINSTANCE hInst, HWND hdlg)
{
	HWND hList;
	LVCOLUMN col={0};

	// 1. 리스트 컨트롤 생성 
	hList=CreateWindow(
		WC_LISTVIEW,NULL,WS_VISIBLE|WS_CHILD|WS_BORDER| \
		LVS_REPORT|LVS_SHOWSELALWAYS,
		10,10,560,300,hdlg,(HMENU)1010, hInst, NULL);

	// 2. 스타일 설정 
	ListView_SetExtendedListViewStyle(hList, 
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES); 

	// 3. 컬럼 설정 
	col.mask=LVCF_FMT | LVCF_WIDTH | LVCF_SUBITEM | LVCF_TEXT;
	col.fmt=LVCFMT_LEFT;

	col.cx=40, col.iSubItem=0, col.pszText="PID";
	ListView_InsertColumn(hList,0,&col);

	col.cx=100, col.iSubItem=0, col.pszText="이 름";
	ListView_InsertColumn(hList,1,&col);

	col.cx=420, col.iSubItem=0, col.pszText="전체 경로";
	ListView_InsertColumn(hList,2,&col); 

	return hList;
}


void __stdcall AttachReady(HWND hList)
{
	LVITEM li={0};

	if(!hList)
	{
		ListView_DeleteAllItems(hList); // 이전에 표시한 리스트뷰 행(아이템)들을 다 지워준다.
		return;
	}

	HANDLE hsnap_process, hsnap_module;
	PROCESSENTRY32 ppe={0};
	MODULEENTRY32 mme={0};
	ppe.dwSize=sizeof(ppe);
	mme.dwSize=sizeof(mme);
	li.mask=LVIF_TEXT | LVIF_IMAGE | LVIF_STATE; // 리스트뷰 사용할 행(아이템) 속성 선택
	hsnap_process = CreateToolhelp32Snapshot(TH32CS_SNAPALL, 0);
	Process32First(hsnap_process, &ppe); // 첨에 나오는 프로세스는 중복검사되므로
	Process32Next(hsnap_process, &ppe); // 한 번 재껴준다. 

	char temp[8]="";
	do{
		hsnap_module=CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, ppe.th32ProcessID);
		Module32First(hsnap_module, &mme);

		// //li.iItem=i; // 리스트뷰 행(아이템) 세팅안하면 알아서 최신 프로세스가 가장 위에 표시
		if(lstrcmp(ppe.szExeFile, mme.szExePath))
		{ 
			li.pszText=itoa(ppe.th32ProcessID, temp, 10); 
			ListView_InsertItem(hList, &li);
			ListView_SetItemText(hList, 0, 1, ppe.szExeFile);
			ListView_SetItemText(hList, 0, 2, mme.szExePath);
			//i++; // 세팅안하면 알아서 최신 프로세스가 가장 위에 표시
		}
		CloseHandle(hsnap_module);
	}while(Process32Next(hsnap_process, &ppe)); // 다음 프로세스 검사한다.

	CloseHandle(hsnap_process);
	
	// 처음 아이템 선택한 상태로
	PostMessage(hList,WM_LBUTTONDOWN,1,0x0017004c); 
	PostMessage(hList,WM_LBUTTONUP,0,0x0017004c);

}




void _stdcall SelectTarget(HWND hList)
{

	int target_pid=0;
	char process_name[30]="";
	char process_path[200]="";
	char pid_str[8]="";

	int num_selected=ListView_GetSelectedCount(hList);

	if(num_selected==0 || num_selected>1)
	{
		MessageBox(NULL,"올바르게 선택하세요","에러",MB_OK);
		return;
	}

	int index= ListView_GetNextItem (
		hList,-1,LVIS_SELECTED);

	ListView_GetItemText(hList, index, 0, pid_str, 8);
	target_pid=atoi(pid_str);
	ListView_GetItemText(hList, index, 2, process_path, 200);
	ListView_GetItemText(hList, index, 1, process_name, 30);
	MessageBox(NULL, process_path, process_name, MB_OK);

}



