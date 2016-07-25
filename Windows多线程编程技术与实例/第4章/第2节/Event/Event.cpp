// Event.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];	// The title bar text


int iArray[10];
HANDLE hThreads[3];
HANDLE hEvent[2];
HANDLE hevent[2];

DWORD WINAPI WriteProc(LPVOID pParm);
DWORD WINAPI Read1Proc(LPVOID pParm);
DWORD WINAPI Read2Proc(LPVOID pParm);
							

// Foward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_EVENT, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_EVENT);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage is only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX); 

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= (WNDPROC)WndProc;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_EVENT);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_EVENT;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, (LPCTSTR)IDI_SMALL);

	return RegisterClassEx(&wcex);
}

//
//   FUNCTION: InitInstance(HANDLE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   HWND hWnd;

   hInst = hInstance; // Store instance handle in our global variable

   hWnd = CreateWindow(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, NULL, NULL, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, unsigned, WORD, LONG)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
		case WM_CREATE:
		 	hevent[0] = CreateEvent(NULL,FALSE,FALSE,NULL);
			hevent[1] = CreateEvent(NULL,FALSE,FALSE,NULL);
			hEvent[0] = CreateEvent(NULL,FALSE,TRUE,NULL);
			hEvent[1] = CreateEvent(NULL,FALSE,TRUE,NULL);
			hThreads[0] = CreateThread(NULL,0,WriteProc,NULL,0,NULL);	
			hThreads[1] = CreateThread(NULL,0,Read1Proc,hWnd,0,NULL);
			hThreads[2] = CreateThread(NULL,0,Read2Proc,hWnd,0,NULL);

		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code here...
			RECT rt;
			GetClientRect(hWnd, &rt);
			DrawText(hdc, szHello, strlen(szHello), &rt, DT_CENTER);
			EndPaint(hWnd, &ps);
			break;
		case WM_DESTROY:
			WaitForMultipleObjects(3,hThreads,TRUE,INFINITE);
			CloseHandle(hevent);
			CloseHandle(hEvent[0]);
			CloseHandle(hEvent[1]);

			CloseHandle(hThreads[0]);
			CloseHandle(hThreads[1]);
			CloseHandle(hThreads[2]);
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

// Mesage handler for about box.
LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_INITDIALOG:
				return TRUE;

		case WM_COMMAND:
			if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) 
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}


//thread1

DWORD WINAPI WriteProc(LPVOID pParm)
{
	DWORD dw;
	for (int j=0;j<10;j++)
	{
		dw = WaitForMultipleObjects(2,hEvent,TRUE,INFINITE);
//		ResetEvent(hEvent[0]);
//		ResetEvent(hEvent[1]);
		
		ResetEvent(hevent[0]);
		ResetEvent(hevent[1]);
	
		for (int i=0;i<10;i++)
			iArray[i]=j;
		SetEvent(hevent[0]);
 		SetEvent(hevent[1]);

		
		
	}
	return 0;
}

DWORD WINAPI Read1Proc(LPVOID pParm)
{
	int aArray[10];

	DWORD dw;
	for (int i=0;i<10;i++)
	{
		//wait for the hevent to become signaled 
		dw = WaitForSingleObject(hevent[0],INFINITE);
// 		ResetEvent(hevent[0]);
		
		ResetEvent(hEvent[0]);
		for (int k=0;k<10;k++)
		{
			aArray[k]=iArray[k];
		}		
		char str[255];
		str[0]=0;
		for (int j=0;j<10;j++)
		{
			int len=strlen(str);
			wsprintf(&str[len],"%d",aArray[j]);
		}
		::MessageBox(NULL,str,"Thread Read1 Proc",MB_OK);
		SetEvent(hEvent[0]);

	}
	return 0;
}

DWORD WINAPI Read2Proc(LPVOID pParm)
{
	int aArray[10];
	DWORD dw;
	for (int i=0;i<10;i++)
	{
		//wait for the hevent to become signaled 
		dw = WaitForSingleObject(hevent[1],INFINITE);
// 		ResetEvent(hevent[1]);
		
		
		ResetEvent(hEvent[1]);		
		for (int k=0;k<10;k++)
		{
			aArray[k]=iArray[k];
		}
		
		
		char str[255];
		str[0]=0;
		for (int j=0;j<10;j++)
		{
			int len=strlen(str);
			wsprintf(&str[len],"%d",aArray[j]);
		}
		
		::MessageBox(NULL,str,"Thread Read2 Proc",MB_OK);// (HWND)param			
		SetEvent(hEvent[1]);



		
	}
	return 0;
}