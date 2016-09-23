// protector.cpp : Defines the entry point for the application.
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





void setup()
{
char namebuff[256]; 
//get path to ths .sys.file
GetModuleFileName(0,namebuff,256);
DWORD  a=strlen(namebuff);
  while(1)
  {
	  if(namebuff[a]=='\\')break;
	  a--;
  }
  a++;
  strcpy(&namebuff[a], "protector.sys");
   
   //create service
   SC_HANDLE man=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
  SC_HANDLE t=CreateService(man,"protectorservice","protectorservice",SERVICE_START|SERVICE_STOP,SERVICE_KERNEL_DRIVER,SERVICE_DEMAND_START,SERVICE_ERROR_NORMAL,namebuff,0,0,0,0,0);
 StartService(t,0,0);
CloseServiceHandle(t);


}




void cleanup()
{
SC_HANDLE man=OpenSCManager(0,0,SC_MANAGER_ALL_ACCESS);
SERVICE_STATUS stat;
SC_HANDLE t=OpenService(man,"protectorservice",SERVICE_ALL_ACCESS);
ControlService(t,SERVICE_CONTROL_STOP,&stat);   
DeleteService(t);
}

HANDLE device;char outputbuff[256]; char * strings[256]; DWORD stringcount;

void thread()
{
DWORD a,x; char msgbuff[512];

while(1)
{
memmove(&a,&outputbuff[0],4);

//if nothing is there, Sleep() 10 ms and check again
if(!a){Sleep(10);continue;}


// looks like our permission is asked. If the file in question is already in the white list,
///give a positive response
char*name=(char*)&outputbuff[8];
for(x=0;x<stringcount;x++)
{
	if(!stricmp(name,strings[x])){a=1;goto skip;}
}


// ask user's permission to run the program
strcpy(msgbuff, "Do you want to run ");
strcat(msgbuff,&outputbuff[8]);

// if user's reply is positive, add the program to the white list
if(IDYES==MessageBox(0, msgbuff,"WARNING",MB_YESNO|MB_ICONQUESTION|0x00200000L))
{a=1; strings[stringcount]=_strdup(name);stringcount++;}
else a=0;

// write response to the buffer, and driver will get it
skip:memmove(&outputbuff[4],&a,4);

//tell the driver to go ahead
a=0;
memmove(&outputbuff[0],&a,4);
}

}

void go()
{
	setup();
DWORD controlbuff[64];DWORD dw;

//create processing thread
CreateThread(0,0,(LPTHREAD_START_ROUTINE)thread,0,0,&dw);

//open device
device=CreateFile("\\\\.\\PROTECTOR",GENERIC_READ|GENERIC_WRITE,0,0,OPEN_EXISTING,FILE_ATTRIBUTE_SYSTEM,0);

// get index of NtCreateSection, and pass it to the driver, along with the
//address of output buffer
DWORD * addr=(DWORD *)(1+(DWORD)GetProcAddress(GetModuleHandle("ntdll.dll"),"NtCreateSection"));
ZeroMemory(outputbuff,256);
controlbuff[0]=addr[0];
controlbuff[1]=(DWORD)&outputbuff[0];
DeviceIoControl(device,1000,controlbuff,256,controlbuff,256,&dw,0);


}








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
	LoadString(hInstance, IDC_PROTECTOR, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_PROTECTOR);

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
	wcex.hIcon			= LoadIcon(hInstance, (LPCTSTR)IDI_PROTECTOR);
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= (LPCSTR)IDC_PROTECTOR;
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

go();
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
	
			break;
		case WM_DESTROY:
			 CloseHandle(device);
			  cleanup();
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
