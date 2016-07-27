#include <windows.h>
#include <tlhelp32.h>
#include "resource.h"

//-------------------------------------------
// 스레드에 전달되는 데이터를 가지고 있는 구조체
//-------------------------------------------
typedef struct _TDATA
{
	HWND	hDlg;			// 주 스레드가 만든 Dialog핸들
	char	exename[256];	// process 이름
	DWORD	pid;			// process ID
}TDATA;


//------------------------------------------------------
// ComboBox 에 현재의 process 목록을 저장하는 함수 
//-------------------------------------------------------
void Refresh( HWND hCombo )
{
	// 1. ComboBox 초기화(비워주기)
	SendMessage(hCombo, CB_RESETCONTENT, 0, 0);

	// 2. ComboBox 채우기 
	HANDLE	hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPPROCESS, 0);

	PROCESSENTRY32 ppe;
	BOOL	b     = Process32First(hSnap, &ppe);
	while( b )
	{
		// LPARAM : 집어넣을 process명.
		int index = SendMessage(hCombo, CB_ADDSTRING, 0, (LPARAM)ppe.szExeFile);

		SendMessage(hCombo, CB_SETITEMDATA, index, (LPARAM)ppe.th32ProcessID);

		b = Process32Next(hSnap, &ppe);
	}
	CloseHandle(hSnap);

	// 3. ComboBox 데이터 선택 
	SendMessage(hCombo, CB_SETCURSEL, 1, 0);
}
//---------------------------------------------
// Thread Function...
//---------------------------------------------
DWORD WINAPI Watch( void *p)
{
	TDATA *pData	= (TDATA*)p;
	DWORD	pid		= pData->pid;	// process의 ID
	//------------------------------
	HANDLE	hProcess = OpenProcess(PROCESS_ALL_ACCESS, 0, pid);
	if( hProcess == 0 )
	{
		MessageBox(0, "프로세스의 핸들을 얻을 수 없다.", "", MB_OK);
		return 0;
	}
	
	//----- Event 관련 구현 추가 ---------------------------------
	HANDLE	hEvent		= CreateEvent(0, TRUE, 0, "Exit");	//-> Open.(동일한 객체의 핸들)
	HANDLE	h[2]		= { hProcess, hEvent };

	DWORD ret = WaitForMultipleObjects( 2, h, FALSE, INFINITE);	// 부분 시그널

	if( ret		== WAIT_OBJECT_0)	// 프로세스가 시그널
		MessageBox(0, "프로세스 종료", pData->exename, MB_OK);

	else if( ret == WAIT_OBJECT_0 + 1 )	// event 시그널
		MessageBox(0, "event Signal,스레드 종료", "", MB_OK);

	CloseHandle( hEvent);
	
	//------------------------------------------------------------
/*
	DWORD ret = WaitForSingleObject(hProcess, INFINITE);
	// 해당 프로세스가 signal
	if( ret == WAIT_OBJECT_0 )	
		MessageBox(0, "프로세스가 종료되었다", pData->exename, MB_OK);
*/
	CloseHandle(hProcess);
	delete pData;
	
	return 0;
}


// DlgProc --------------------------------------------
BOOL CALLBACK DlgProc(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	static HWND		hCombo;				// comboBox의 윈도우 핸들

	static HANDLE	hThreadList[256];	// 생성된 thread의 핸들
	static int		nCount = 0;			// 생성된 thread의 갯수 

	//-------------------------------------------------------------------
	static HANDLE	hExit;				// 동기화 이벤트 객체(수동)
	//-------------------------------------------------------------------

	switch( msg )
	{
	case WM_INITDIALOG:
		hExit				= CreateEvent(0, TRUE, 0, "Exit");	//수동 이벤트 객체 생성
		hCombo				= GetDlgItem(hDlg, IDC_COMBO1);
		Refresh(hCombo);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		// watch버튼 클릭시 호출되는 메시지 핸들러.
		case IDC_BTN_WATCH:
			{
				//현재 선택된 인덱스 리턴..
				int n		= SendMessage(hCombo, CB_GETCURSEL,0, 0);
				// wParam(index)로 전달된 index가 가지고 있는 data 리턴.
				DWORD pid	= SendMessage(hCombo, CB_GETITEMDATA, n, 0);

				// 스레드로 전달할 구조체 정보 저장
				TDATA *pData	= new TDATA;
				pData->hDlg		= hDlg;
				pData->pid		= pid;
				GetWindowText(hCombo, pData->exename, 256);
				//-----------------------------------------------------

				// 스레드 생성 및 데이터 전달
				DWORD tid;
				hThreadList[nCount++] = CreateThread(0, 0, Watch, 
							(void*)pData, 0, &tid);

				// 증가된 카운트 출력
				SetDlgItemInt(hDlg, IDC_STATIC_COUNT, nCount, 0);
						

			}
			break;
		case IDC_BTN_REFRESH:
			{
				Refresh(hCombo);
			}
			break;
		case IDCANCEL:

			SetEvent(hExit);		// 대기중인 모든 스레드를 깨운다..

			// 모든 생성된 스레드가 종료될때까지 대기하자..
			WaitForMultipleObjects( nCount, hThreadList, TRUE, INFINITE);

			for( int i=0; i< nCount; i++)
			{
				CloseHandle( hThreadList[i]);
			}
			EndDialog(hDlg, IDCANCEL);
			break;
		}
		return TRUE;
	}
	return FALSE;
}

int WINAPI WinMain( HINSTANCE hInst, HINSTANCE , LPSTR lpCmdLine,
						int nShowCmd)
{
	DialogBox(hInst, MAKEINTRESOURCE(IDD_DIALOG1), 0, DlgProc);
	return 0;
}