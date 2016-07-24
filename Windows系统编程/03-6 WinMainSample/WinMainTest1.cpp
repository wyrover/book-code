#include "windows.h"
#include "stdio.h"
#include "resource.h"

LRESULT CALLBACK MyProc(HWND hWnd, UINT Msg, 
						WPARAM wParam,LPARAM lParam );

int PASCAL WinMain(HINSTANCE hInstance,
				   HINSTANCE hPrevInstance,
				   LPSTR lpCmdLine,
				   int nCmdShow)
{
	
	WNDCLASS wndClass;
	
	wndClass.style=0;
	wndClass.lpfnWndProc	=(WNDPROC)MyProc;
	wndClass.cbClsExtra		=0;
	wndClass.cbWndExtra		=0; 
    wndClass.hInstance		=hInstance;
	wndClass.hIcon			=LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1)); 
	wndClass.hCursor		=LoadCursor(hInstance,
										MAKEINTRESOURCE(IDC_CURSOR1)); 
//	wndClass.hCursor		=LoadCursor(NULL,IDC_NO);

	LOGBRUSH lgbr;
	lgbr.lbStyle	=BS_SOLID;
	lgbr.lbColor	=RGB(0,192,192);
	lgbr.lbHatch	=0;
	wndClass.hbrBackground	=CreateBrushIndirect(&lgbr);
//	wndClass.hbrBackground	=(HBRUSH) (COLOR_ACTIVECAPTION);
//	wndClass.hbrBackground	=(HBRUSH)GetStockObject(BLACK_BRUSH);

	wndClass.lpszMenuName	=NULL; 
    wndClass.lpszClassName	="MyProc";

	RegisterClass(&wndClass);

	HWND hWnd;
	hWnd=CreateWindow("MyProc","Windows内核编程与实例",
		WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX &~WS_MINIMIZEBOX,
		CW_USEDEFAULT,0,
		CW_USEDEFAULT,0,
		NULL,NULL,hInstance,NULL);
	ShowWindow(hWnd,nCmdShow);
	UpdateWindow(hWnd);

	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	}
	return true;
}


LRESULT CALLBACK MyProc( HWND hWnd,UINT Msg,
						WPARAM wParam, LPARAM lParam)
{
	switch(Msg)
	{
	case WM_CHAR:
		char str[10];
		sprintf(str,"char is %c",wParam);
	//	sprintf(str,"char is %d",wParam);
		MessageBox(hWnd,str,"MyProc",MB_OKCANCEL);
		break;

	case WM_LBUTTONDOWN:
		MessageBox(hWnd,"mouse click","MyProc",MB_OK);
		HDC hDC;
		hDC=GetWindowDC(hWnd);
		TextOut(hDC,20,100,"你信不信？",strlen("你信不信？"));
		ReleaseDC(hWnd,hDC);
		break;

	case WM_COMMAND:
		;
		break;

	case WM_CLOSE:
		if(IDOK==MessageBox(NULL,"你真的要走吗？","喂,喂!",MB_OKCANCEL))
		{
			DestroyWindow(hWnd);
		}
		break;	

	case WM_MOVING:
		RedrawWindow(hWnd,NULL,NULL,
					RDW_ERASE|RDW_INVALIDATE |RDW_ERASENOW );
		break;
	
	case WM_PAINT:
		PAINTSTRUCT ps;
		ps.fErase =0;
		hDC=BeginPaint(hWnd,&ps);
		TextOut(hDC,20,20,"知识就是力量:）",strlen("知识就是力量:）"));
		EndPaint(hWnd,&ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return(DefWindowProc(hWnd,Msg, wParam,lParam ));

	}
	return (0);
}

