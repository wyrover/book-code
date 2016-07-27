#include <stdio.h>
#include <windows.h>
#include <errno.h>
#include <conio.h>

// 함수 호출이 성공했을때 성공의 이유를 알기 위해서 
// GetLastError()를 호출할 때가 있다.
//  - 항상그런것은 아니다.
// 동시에 동일한 2개의 프로세스가 생성되는 것을 막는 방법.
void main()
{
	HANDLE hEvent = CreateEvent( 0, 0, 0,"e");
//	HANDLE hEvent = CreateEvent(0, 0, 0, "{0826602B-E8C6-4fd8-A497-6F514576AD48}");
	
	if ( hEvent != 0 ) // 성공..!!
	{
		DWORD e = GetLastError(); // 왜 성공했는지 이유를 알고 싶다.

		if ( e == ERROR_ALREADY_EXISTS )
		{
			printf("이미 존재하는 event 객체를 Open 했습니다.\n");
			printf("이미 응용 프로그램이 실행중입니다..\n");
			return;
		}
		else
		{
			printf("event 객체를 새롭게 생성했습니다.\n");
		}
	}
	getch(); // #include <conio.h> 추가해 주세요.
}




























