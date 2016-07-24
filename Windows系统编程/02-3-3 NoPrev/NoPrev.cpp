// NoPrev.cpp : Defines the entry point for the application.
//

#include "stdafx.h"

#define VK_X 0x58

int APIENTRY WinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPSTR     lpCmdLine,
                     int       nCmdShow)
{
 	// TODO: Place code here.
	HANDLE h = CreateMutex(NULL,TRUE,"No Previous Instance!");
	if (!h)
	{
		MessageBox(NULL,"Fail to create Mutex !","NoPrev",
			MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

	if (GetLastError()==ERROR_ALREADY_EXISTS)
	{
		MessageBox(NULL,
		"Previous Instance is running, the current is forbidden!",
		"NoPrev",MB_OK|MB_SYSTEMMODAL);
		return FALSE;
	}

	if (!RegisterHotKey(NULL,0x0001,
		MOD_CONTROL|MOD_SHIFT,VK_X))
	{
		MessageBox(NULL,"Fail to register hotkey Ctrl+Shift+X!",
			"NoPrev",MB_OK|MB_SYSTEMMODAL);
		return FALSE;	
	}

	MessageBox(NULL,
	"NoPrev is running!\n\nHotkey Ctrl+Shift+X will stop NoPrev.",
	"NoPrev",MB_OK|MB_SYSTEMMODAL);

	MSG msg;
	while(GetMessage(&msg,NULL,0,0))
	{
		switch(msg.message)
		{
		case WM_HOTKEY:
			if (int(msg.wParam)==0x0001)
				if(MessageBox(NULL,"Stop NoPrev?","NoPrev",
					MB_OK|MB_SYSTEMMODAL)==IDYES)
					return FALSE;
		}
	}

	//CloseHandle(h); 
	return TRUE;
}

