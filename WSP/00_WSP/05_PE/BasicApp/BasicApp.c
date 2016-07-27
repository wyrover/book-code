/*******************************************************************************
 * 파  일 : BasicApp.C
 * 종  류 : C 본체
 * 정  의 : WinMain - Win32 Entry Point.
 *          WndProc - Windows Procedure.
 * 내  용 : 가장 원초적인 윈도우즈 GUI 어플리케이션의 정의...
 * ----------------------------------------------------------------------------
 * 저  자 : Yi HoDong
 * 날  짜 : 2001.11.06
 * 버  젼 : 1.0
 * 회  사 : YHD Works Co. (http://www.yhdworks.com)
 ******************************************************************************/
#include <windows.h>

TCHAR g_szAppName[] = "Basic GUI Application";

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

/*******************************************************************************
 * 종  류 : WIN32 엔트리 포인트 홤수
 * 소  속 : 없음
 * 이  름 : WinMain
 * 인  수 : HINSTANCE hInstance [IN] : 본 프로세스의 인스턴스 핸들(시작번지)
 *          HINSTANCE hPrevInst [IN] : 언제나 0, 무시(16비트와의 호환을 위해)
 *          PSTR      szCmdLine [IN] : 커맨드 라인 패러미터
 *          int       iCmdShow  [IN] : 윈도우가 보여지는 형태
 * 반  환 : int : 
 * 내  용 : WIN32 엔트리 포인트 홤수
 *          콘솔 프로그램(main)과는 다르게 GUI에서는 WinMain을 엔트리 포인트
 *          함수로 인식한다.
 * ----------------------------------------------------------------------------
 * 저  자 : Yi HoDong
 * 날  짜 : 2001.08.06
 * 버  젼 : 1.0
 * 회  사 : YHD Works Co. (http://www.yhdworks.com)
 ******************************************************************************/
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInst,
                   PSTR szCmdLine, int iCmdShow)
{
	HWND         hWnd;
	MSG          msg;
	WNDCLASS     wndclass;


	////////////////////////////////////////////////////////////////////////////
	//윈도우즈 클래스 등록
	////////////////////////////////////////////////////////////////////////////
	wndclass.style         = CS_HREDRAW|CS_VREDRAW;
	wndclass.lpfnWndProc   = WndProc;
	wndclass.cbClsExtra    = 0;
	wndclass.cbWndExtra    = 0;
	wndclass.hInstance     = hInstance;
	wndclass.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor       = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName  = NULL;
	wndclass.lpszClassName = g_szAppName;
	if(!RegisterClass(&wndclass))
	{
		MessageBox(NULL, "Window Class Registeration Failure!!!",
				  g_szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	//윈도우즈 생성& 갱신
	////////////////////////////////////////////////////////////////////////////
	hWnd = CreateWindow(g_szAppName,			// window class name
						g_szAppName,			// window caption
						WS_OVERLAPPEDWINDOW,	// window style
						CW_USEDEFAULT,			// initial x position
						CW_USEDEFAULT,			// initial y position
						400,					// initial x size
						150,					// initial y size
						NULL,					// parent window handle
						NULL,					// window menu handle
						hInstance,				// program instance handle
						NULL) ;					// creation parameters
	if(!hWnd)
	{
		MessageBox(NULL, "Window Creation Failure!!!",
				  g_szAppName, MB_ICONERROR) ;
		return 0 ;
	}
	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);
	////////////////////////////////////////////////////////////////////////////


	////////////////////////////////////////////////////////////////////////////
	//메시지 루프
	////////////////////////////////////////////////////////////////////////////
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	////////////////////////////////////////////////////////////////////////////

	return msg.wParam;
}
////////////////////////////////////////////////////////////////////////////////


/*******************************************************************************
 * 종  류 : 윈도우즈 프로시저 함수(콜백 함수)
 * 소  속 : 없음
 * 이  름 : WndProc
 * 인  수 : HWND   hWnd   [IN] : 메시지를 받을 윈도우 핸들
 *           UINT   uMsg   [IN] : 메시지
 *           WPARAM wParam [IN] : 인자1
 *           LPARAM lParam [IN] : 인자2
 * 반  환 : LRESULT : 
 * 내  용 : 윈도우즈 프로시저
 *           WINDOWCLASS의 lpfnWndProc 필드에 이 함수의 번지를 등록해야.
 *           윈도우의 동작을 이 함수 내에서 컨트롤한다.
 * ----------------------------------------------------------------------------
 * 저  자 : Yi HoDong
 * 날  짜 : 2001.08.06
 * 버  젼 : 1.0
 * 회  사 : YHD Works Co. (http://www.yhdworks.com)
 ******************************************************************************/
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	static char szOutBuf[] = "The Most Simple Windows GUI Program by YHD.";
	HDC         hDC;
	PAINTSTRUCT ps;
	RECT        rc;

	switch(uMsg)
	{
		case WM_PAINT:
			hDC = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rc);
			DrawText(hDC, 
					 szOutBuf, 
					 lstrlen(szOutBuf), 
					 &rc, 
					 DT_SINGLELINE|DT_CENTER|DT_VCENTER);
			EndPaint (hWnd, &ps) ;
		return 0;

		case WM_DESTROY:
			PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
////////////////////////////////////////////////////////////////////////////////