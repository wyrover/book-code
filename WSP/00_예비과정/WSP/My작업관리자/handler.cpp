// handler.cpp

#include "std.h"


//-------------------------------------------------------------------------
// 함수명 : OnCreate
// 기  능 : 자식 컨트롤 생성 및 초기화 
//-------------------------------------------------------------------------
int OnCreate(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	InitCommonControls();

	CreateChildControl(hwnd);

	ProcessList();

	hExitEvent	= CreateEvent(0, TRUE, 0, "Exit");	//수동 이벤트 객체 생성

	return 0;
}

int OnCtrlColorEdit(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	static HBRUSH hBrush = CreateSolidBrush( RGB(255,255,0));

	HDC hdc = (HDC)wParam; // Edit 가 사용할 dc
	HWND h  = (HWND)lParam;

	SetBkColor( hdc, RGB(190,190,190));

	return (LRESULT)GetStockObject(LTGRAY_BRUSH); // WM_CTLCOLORxxx 메세지를 처리할때
}

//-------------------------------------------------------------------------
// 함수명 : OnSysComand
// 기  능 : 종료 버튼 클릭시 트레이 아이콘 처리 
//-------------------------------------------------------------------------
int OnSysComand(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch( wParam & 0xFFF0 )	
	{
	case SC_MINIMIZE: return 	OnTrayIcon(hwnd);
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}


//-------------------------------------------------------------------------
// 함수명 : OnTrayMessage
// 기  능 : 트레이 아이콘에서 발생되는 사용자 정의 메시지 처리
//-------------------------------------------------------------------------
int OnTrayMessage(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	TrayMessage(hwnd, wParam, lParam);
	return 0;
}


//-------------------------------------------------------------------------
// 함수명 : OnDestroy
// 기  능 : 프로그램 종료 함수 호출
//-------------------------------------------------------------------------
int OnDestory()
{
	SetEvent(hExitEvent);		// 대기중인 모든 스레드를 깨운다..

	// 모든 생성된 스레드가 종료될때까지 대기하자..
	WaitForMultipleObjects( ThreadCount, hThreadList, TRUE, INFINITE);

	for( int i=0; i< ThreadCount; i++)
	{
		CloseHandle( hThreadList[i]);
	}

	PostQuitMessage(0);
	return 0;
}

//-------------------------------------------------------------------------
// 함수명 : OnCommand
// 기  능 : 메뉴 및 컨트롤의 NOTIFY 처리 
//-------------------------------------------------------------------------
int OnCommand(HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch( LOWORD(wParam))		// ID 조사
	{
	case ID_EXIT:	SendMessage(hwnd, WM_CLOSE, 0, 0);	break;
	case ID_INFO:	MessageBox(hwnd, "과제 정보 : WSP 실습\r\n구현 날짜 : 2009.1.20\r\n만  든  이 : CCM","프로그램 정보",  NULL);
	case ID_MENU_SHOW:	ShowWindow(hwnd, SW_SHOW);		break;

	case ID_MENU_NEWPROCESS: NewProcess(hwnd);			break;

	case IDC_BTNREFRESH:	ProcessList();				break;
	case IDC_BTNTERMINATEPROCESS: ProcessExit(hwnd);	break;
	case IDC_BTNSPY: WatchProcess(hwnd);				break;
	}
	return 0;
}