// main.cpp



#include "std.h"
#pragma comment(lib, "comctl32.lib")

HWND hMainList;
HWND hBtnRefresh, hBtnTerminateProcess, hBtnSpy;
HWND hStaticLog;

HANDLE	hThreadList[20];	// 생성된 thread의 핸들
int		ThreadCount;

HANDLE	hExitEvent;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{

	switch(msg)
	{
	case WM_CREATE:		return OnCreate(hwnd, wParam, lParam);

	case WM_CTLCOLOREDIT: return OnCtrlColorEdit(hwnd, wParam, lParam);

	case WM_COMMAND:	return OnCommand(hwnd, wParam, lParam);

	case WM_SYSCOMMAND:	return OnSysComand(hwnd, msg, wParam, lParam);

	case WM_TRAYMESSAGE:return OnTrayMessage(hwnd, wParam, lParam);

	case WM_DESTROY:	return OnDestory();
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}

////////////////////////////////////////////////////////////
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, 
				   PSTR lpCmdLine, int nShowCmd)
{

	WNDCLASS wc;

	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;

	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.hCursor = LoadCursor(0, IDC_ARROW);
	wc.hIcon = LoadIcon(0, IDI_APPLICATION);
	wc.hInstance = hInstance;

	wc.lpfnWndProc = WndProc; 

	wc.lpszClassName = "TEST"; 
	wc.lpszMenuName = MAKEINTRESOURCE(IDR_MAINMENU);  
	wc.style = 0; 
	
	RegisterClass(&wc); 
	
	HWND hwnd = CreateWindow("TEST", 
					"작업관리자 실습 Ver1.0", 
		WS_OVERLAPPEDWINDOW , 
		CW_USEDEFAULT,0,530, 590, 0, 0, 
			hInstance, 0); 

	ShowWindow(hwnd, nShowCmd);
	UpdateWindow(hwnd); 

	MSG msg;
	while ( GetMessage(&msg, 0, 0, 0) )
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

