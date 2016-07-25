#include <windows.h>

HHOOK g_hMouse=NULL;
HHOOK g_hKeyboard=NULL;

#pragma data_seg("MySec")
HWND g_hWnd=NULL;
#pragma data_seg()

//#pragma comment(linker,"/section:MySec,RWS")
/*HINSTANCE g_hInst;

BOOL WINAPI DllMain(
  HINSTANCE hinstDLL,  // handle to the DLL module
  DWORD fdwReason,     // reason for calling function
  LPVOID lpvReserved   // reserved
)
{
	g_hInst=hinstDLL;
}*/

LRESULT CALLBACK MouseProc(
  int nCode,      // hook code
  WPARAM wParam,  // message identifier
  LPARAM lParam   // mouse coordinates
)
{
	return 1;
}

LRESULT CALLBACK KeyboardProc(
  int code,       // hook code
  WPARAM wParam,  // virtual-key code
  LPARAM lParam   // keystroke-message information
)
{
	if(VK_F2==wParam)
	{
		SendMessage(g_hWnd,WM_CLOSE,0,0);
		UnhookWindowsHookEx(g_hMouse);
		UnhookWindowsHookEx(g_hKeyboard);
	}
	return 1;
}

void SetHook(HWND hwnd)
{
	g_hWnd=hwnd;
	g_hMouse=SetWindowsHookEx(WH_MOUSE,MouseProc,GetModuleHandle("Hook"),0);
	g_hKeyboard=SetWindowsHookEx(WH_KEYBOARD,KeyboardProc,GetModuleHandle("Hook"),0);
}