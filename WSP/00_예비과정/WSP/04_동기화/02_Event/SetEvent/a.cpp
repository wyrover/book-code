#include <iostream>
#include <windows.h>
using namespace std;

// Event : 스레드간 통신을 사용한 동기화.
void main()
{
	HANDLE h = CreateEvent( 0,     // 보안
							FALSE, // 리셋의 종류 ( TRUE: manual, FALSE : auto)
							FALSE, // 초기 Signal 상태
							"e");

	while ( 1 )
	{
		int n;	cin  >> n;

		// Event를 Signal 한다.
		SetEvent( h );
	}


	CloseHandle( h );
}


