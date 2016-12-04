
#include <windows.h>


LRESULT CALLBACK WndProc (HWND, UINT, WPARAM, LPARAM) ;

int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance,
					PSTR szCmdLine, int iCmdShow)
{
	TCHAR szAppName[] = TEXT ("testbutton") ;
	HWND         hwnd ;
	MSG          msg ;
	WNDCLASS     wndclass ;

	wndclass.style         = CS_HREDRAW | CS_VREDRAW ;
	wndclass.lpfnWndProc   = WndProc ;
	wndclass.cbClsExtra    = 0 ;
	wndclass.cbWndExtra    = 0 ;
	wndclass.hInstance     = hInstance ;
	wndclass.hIcon         = LoadIcon (NULL, IDI_APPLICATION) ;
	wndclass.hCursor       = LoadCursor (NULL, IDC_ARROW) ;
	wndclass.hbrBackground = (HBRUSH) GetStockObject (WHITE_BRUSH) ;
	wndclass.lpszMenuName  = NULL ;
	wndclass.lpszClassName = szAppName ;

	if (!RegisterClass (&wndclass))
	{
		MessageBox (NULL, TEXT ("This program requires Windows NT!"),
			szAppName, MB_ICONERROR) ;
		return 0 ;
	}

	hwnd = CreateWindow (szAppName, TEXT ("Button Look"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL) ;

	ShowWindow (hwnd, iCmdShow) ;
	UpdateWindow (hwnd) ;

	while (GetMessage (&msg, NULL, 0, 0))
	{
		TranslateMessage (&msg) ;
		DispatchMessage (&msg) ;
	}
	return msg.wParam ;
}

HWND b1, b2;

WNDPROC oldBtnProc;

LRESULT CALLBACK WndProcBtn(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	//必须在自己处理前先调用前面的处理函数，如果之后调用则可能出现问题。
	LRESULT result = CallWindowProcA(oldBtnProc, hwnd, message, wParam, lParam);
	if (message == WM_LBUTTONUP)
	{
		if (b1 == hwnd)
		{			
			MessageBoxA(NULL, "button1 left up",NULL, NULL);
		}
		else if (b2 == hwnd)
		{			
			MessageBoxA(NULL, "button2 left up",NULL, NULL);
		}	
	}	
	return result;
}

LRESULT CALLBACK WndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_CREATE :
		b1 = CreateWindowA( "BUTTON", 
			"button1",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 10,
			60, 40,
			hwnd, 0,
			((LPCREATESTRUCT) lParam)->hInstance, NULL) ;		
		oldBtnProc = (WNDPROC)SetWindowLong(b1, GWL_WNDPROC, (LONG)WndProcBtn);
		b2 = CreateWindowA( "BUTTON", 
			"button2",
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 70,
			60, 40,
			hwnd, 0,
			((LPCREATESTRUCT) lParam)->hInstance, NULL) ;		
		SetWindowLong(b2, GWL_WNDPROC, (LONG)WndProcBtn);
		return 0 ;
	
	case WM_LBUTTONDOWN:
		MessageBoxA(NULL, "form left button",NULL, NULL);
		break;
/*
	case WM_COMMAND :
		if (b1 == (HWND)lParam)
		{			
			MessageBoxA(NULL, "button1 click",NULL, NULL);
		}
		else if (b2 == (HWND)lParam)
		{			
			MessageBoxA(NULL, "button2 click",NULL, NULL);
		}
		
		break ;
*/
	case WM_DESTROY :
		PostQuitMessage (0) ;
		return 0 ;
	}
	return DefWindowProc (hwnd, message, wParam, lParam) ;
}
