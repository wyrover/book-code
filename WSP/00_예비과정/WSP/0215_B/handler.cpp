//handler.cpp
#include "std.h"

struct DATA
{
	char message[30];
	char nickname[20];
	int  h, m, s;
};

int  count;
HWND hNickNameList;

int OnCopyData(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	COPYDATASTRUCT* p = (COPYDATASTRUCT*)lParam;

	if ( p->dwData == 1 ) // 닉테임(접속)
	{
		count++;
		SetDlgItemInt(hDlg, IDC_EDIT1, count, FALSE);
		SendMessage(hNickNameList, LB_ADDSTRING, 0, (LPARAM)p->lpData);		
	}
	else if( p->dwData == 2)
	{
		DATA *pData = (DATA*)p->lpData;
		MessageBox(hDlg, pData->message, pData->nickname, MB_OK);

	}
	return true;
}

int OnInitDialog(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	count = 0;			
	hNickNameList = GetDlgItem(hDlg, IDC_LIST2);
	return true;
}

int OnCommand(HWND hDlg, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD( wParam ) )
	{
	
	case IDCANCEL: EndDialog(hDlg, IDCANCEL);	return 0;
	}
	return true;
}