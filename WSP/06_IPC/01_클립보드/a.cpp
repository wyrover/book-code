#include <windows.h>
#include <stdio.h>

void main()
{
	char buf[4096] = { 0 };

	if ( OpenClipboard( 0 ) )
	{
		// 클립 보드에 있는 메모리 핸들을 얻어 온다.
		HANDLE hData = GetClipboardData( CF_TEXT );

		char* p = (char*)GlobalLock( hData );
	
		strcpy( buf, p ); // 자신의 버퍼에 옮겨 담는다.
		
		GlobalUnlock( hData );
		
		CloseClipboard( );
	}

	printf("%s\n", buf );
}













/*
void main()
{
	char buf[256] = { 0 };

	while ( 1 )
	{
		printf("클립 보드로 전송할 메세지를 입력하세요 >> ");
		gets( buf );

		//==============================================
		// 클립 보드로 전송할 이동가능한 메모리를 할당
		HANDLE hData = GlobalAlloc( GMEM_MOVEABLE, 256 );

		// 핸들 기반의 이동 가능 메모리를 포인터로 변경한다.
		char* pData = (char*)GlobalLock( hData );
		strcpy( pData, buf );
		GlobalUnlock( hData);
		//=======================================
		// 클립 보드를 Open하고 메모리를 전송한다.
		if ( OpenClipboard( 0 ) )   // 인자는 클립보드를 Open 하는 윈도우 핸들.
		{
			EmptyClipboard();  // 기존의 clipboard를 비운다.
			SetClipboardData( CF_TEXT, hData ); 
			CloseClipboard();
		}
	}
}
*/












