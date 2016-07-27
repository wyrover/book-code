// 네트워크 기본 코드 복사해 오세요
#define _WIN32_WINNT 0x0500
#define WINVER       0x0500

#define WIN32_LEAN_AND_MEAN
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>	
#pragma comment(lib, "ws2_32.lib")

struct MOUSE
{
	int message;
	POINTS point;
	WPARAM wParam;
	LPARAM lParam;
};

void main()
{
	WSADATA wsadata;

	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		printf("Can't Initialize Socket !\n");
		return ;
	}
	//--------------------------------------------
	SOCKET s = socket( AF_INET, SOCK_DGRAM, 0);

	SOCKADDR_IN addr;
	addr.sin_family			= AF_INET;
	addr.sin_port			= htons( 7000 );
	addr.sin_addr.s_addr    = INADDR_ANY;

	bind( s, (SOCKADDR*)&addr, sizeof(addr));

	while ( 1 )
	{
		SOCKADDR_IN c_addr;
		int sz = sizeof(c_addr);
		MOUSE mouse;
		
		recvfrom( s, (char*)&mouse, sizeof(mouse), 0, (SOCKADDR*)&addr, &sz);

		if ( mouse.message == WM_MOUSEMOVE )
		{
			SetCursorPos( mouse.point.x, mouse.point.y );
		}
		else if ( mouse.message == WM_LBUTTONDOWN || 
			      mouse.message == WM_LBUTTONUP)
		{
			INPUT input = {0};
			
			input.type = INPUT_MOUSE;

			input.mi.dx = mouse.point.x;
			input.mi.dy = mouse.point.y;

			if ( mouse.message == WM_LBUTTONDOWN )
				input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
			else
				input.mi.dwFlags = MOUSEEVENTF_LEFTUP;

			SendInput( 1, &input, sizeof(input));
		}
		else if ( mouse.message == WM_KEYDOWN || mouse.message == WM_KEYUP )
		{
			INPUT input = {0};

			input.type = INPUT_KEYBOARD;

			input.ki.wVk = mouse.wParam;
			input.ki.wScan = ( mouse.lParam & 0x00FF0000) << 16;

			if ( mouse.message == WM_KEYUP )
				input.ki.dwFlags = KEYEVENTF_KEYUP;

			SendInput( 1, &input, sizeof(input));
		}




	}
	//--------------------------------------------
	WSACleanup();
}

























