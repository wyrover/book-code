#include <windows.h>
#include <stdio.h>

void main()
{
	char buf[256] = {0};

	HWND hwnd = FindWindow( 0, "B");
	if ( hwnd == 0 )
	{
		printf("B 윈도우를 먼저 실행해 주세요\n");	return;
	}
	//-----------------------------------
	while ( 1 )
	{
		printf("B에게 전달한 메세지를 입력하세요 >> ");
		gets( buf); // 1줄입력 ?

		// 원격지로 Pointer를 전달하기 위한 메세지 - WM_COPYDATA
		COPYDATASTRUCT cds;
		cds.cbData = strlen(buf)+1;  // 전달한 data 크기
		cds.dwData = 1;				// 식별자 
		cds.lpData = buf;			// 전달할 Pointer

		SendMessage( hwnd, WM_COPYDATA, 0,  (LPARAM)&cds);
	}
}









