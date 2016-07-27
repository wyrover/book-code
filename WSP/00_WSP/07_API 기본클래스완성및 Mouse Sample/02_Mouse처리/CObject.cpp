#include <windows.h>
#include "stdafx.h"
#include "resource.h"
#include "CObject.h"
#include "cview.h"
#include "CDocument.h"


extern CView app; // one global object

void CObject::InitInstance(HINSTANCE hInstance,PSTR szCmdLine,
						int iCmdShow) 
{
    wndclass.cbSize         = sizeof(wndclass);
    wndclass.style          = CS_HREDRAW|CS_VREDRAW;
    wndclass.lpfnWndProc    = WndProc;
    wndclass.cbClsExtra     = 0;
    wndclass.cbWndExtra     = 0;
    wndclass.hInstance      = hInstance;
    wndclass.hIcon          = LoadIcon(NULL,IDI_APPLICATION);
    wndclass.hCursor        = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground  = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName   = MAKEINTRESOURCE(IDR_MENU1);;
    wndclass.lpszClassName  = szAppName;
    wndclass.hIconSm        = LoadIcon(NULL,IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(szAppName,    //window class name
        szAppName,					 //window caption
        WS_OVERLAPPEDWINDOW,        //window style
        0,              //initial x position
        0,              //initial y position
        500,              //initial x size
        500,              //initial y size
        NULL,                       //parent window handle
        NULL,                       //window menu handle
        hInstance,                  //program instance handle
        NULL);                      //creation parameters
	//------------ Doc 개체 생성 --------------------
	pDoc = new CDocument();

    ShowWindow(hwnd,iCmdShow);
    UpdateWindow(hwnd);


}


void CObject::Run() 
{
	while (GetMessage(&msg,NULL,0,0)) 
	{
		if ( !app.PreTranslateMessage(hwnd, &msg) )
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
    }
}



WPARAM CObject::ExitInstance() 
{
	delete pDoc;

    return msg.wParam;
}

char CObject::szAppName[] = "NotePad";