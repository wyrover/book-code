//--------------------------------------------------------------
// #define WIN32_LEAN_AND_MEAN
// h파일 충돌 처리 
//----------------------------------------------------------------

#define WIN32_LEAN_AND_MEAN // windows.h 에서 자주 사용하지 않은 것은 컴파일에서
							// 제외 한다. winsock2.h 와의 충돌을 막아 준다.
#include <stdio.h>
#include <windows.h>	
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

void main()
{
	WSADATA wsadata;

	if ( WSAStartup( MAKEWORD(2,2), &wsadata) != 0 )
	{
		printf("Can't Initialize Socket !\n");
		return ;
	}
	//--------------------------------------------

	//--------------------------------------------
	WSACleanup();
}

// 여기까지의 코드가 모든 Network 프로그램의 기본 코드 입니다. 잘 보관해 두세요.
// 앞으로 이 코드를 복사해서 사용하겠습니다.








