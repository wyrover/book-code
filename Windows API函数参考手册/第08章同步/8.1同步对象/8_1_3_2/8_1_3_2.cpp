// 8_1_3_2.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "resource.h"

#define MAX_LOADSTRING 100

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];								// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];								// The title bar text

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
	LoadString(hInstance, IDC_MY8_1_3_2, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_MY8_1_3_2);

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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_MY8_1_3_2);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_MY8_1_3_2;
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
DWORD WINAPI        Thread1(LPVOID param);
DWORD WINAPI        Thread2(LPVOID param);
//定义关键段对象
CRITICAL_SECTION cs;
//定义一个新的数据结构，该结构为链表中的节点
typedef struct LINK{
	int num;
	LINK *next;
}LINK,*PLINK;
PLINK pHead;

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;
	TCHAR szHello[MAX_LOADSTRING];
	LoadString(hInst, IDS_HELLO, szHello, MAX_LOADSTRING);

	switch (message) 
	{
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
			
			break;
	
		

//下面的程序段在程序的主消息循环中
case WM_CREATE:
		InitializeCriticalSection(&cs);
		//下面的代码创建一个单向链表
		int i;
		PLINK pLink,pTemp;
		pLink=new(LINK);
		pLink->next=NULL;
		pLink->num=10;
		pHead=pLink;
		//创建一个包含10个节点的链表
		for(i=0;i<10;i++){
			pTemp=new(LINK);
			pLink->next=pTemp;
			pTemp->num=i*10;
			pLink=pTemp;
		}
		
		break;
case WM_RBUTTONDOWN:
		HANDLE hThread1,hThread2;
		DWORD ThreadID;
		HDC hDCT;

		hDCT=GetDC(hWnd);
		hThread1=CreateThread(NULL,0,Thread1,hDCT,NULL,&ThreadID);
		hThread2=CreateThread(NULL,0,Thread2,hDCT,NULL,&ThreadID);
case WM_DESTROY:
		//在关闭程序时，删除关键段对象
		DeleteCriticalSection(&cs);
		PostQuitMessage(0);
		break;


	
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}
//下面定义了两个在链表上进行操作的函数
//Get函数获取链表的表头节点
PLINK Get()
{
		PLINK pTemp;
		if(pHead!=NULL){
			pTemp=pHead;
			pHead=pHead->next;
			pTemp->next=NULL;
			return pTemp;
		}
		return NULL;
}
//Put函数将pLink指定的节点放在链表的表头
VOID Put(PLINK pLink)
{
		pLink->next=pHead;
		pHead=pLink;
}
//下面为具体的线程函数
DWORD WINAPI  Thread1(LPVOID param)
{
	PLINK pLink;
		//链表的操作代码段为关键段，它不能被交叉调用，所以用关键段技术来保证其被串型调用
		EnterCriticalSection(&cs);
		pLink=Get();
		//实际应用中，可以使用该指针或在链表上进行更复杂的操作
		Put(pLink);
		MessageBox(NULL,"EnterCriticalSection--Thread1","OK",MB_OK);
		LeaveCriticalSection(&cs);
	return 1;
}

DWORD WINAPI  Thread2(LPVOID param)
{
		PLINK pLink;	
		EnterCriticalSection(&cs);
		pLink=Get();
		Put(pLink);
		MessageBox(NULL,"EnterCriticalSection--Thread2","OK",MB_OK);
		LeaveCriticalSection(&cs);
		return 1;
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
