#include <iostream>
#include <windows.h>
using namespace std;

// Event : 스레드간 통신을 사용한 동기화.
void main()
{
	HANDLE h = CreateEvent( 0,     // 보안
							TRUE, // 리셋의 종류 ( TRUE: manual, FALSE : auto)
							FALSE, // 초기 Signal 상태
							"e");

	cout << "Event를 대기 합니다." << endl;

	WaitForSingleObject( h, INFINITE );
	cout << "Event 획득" << endl;

	WaitForSingleObject( h, INFINITE );
	cout << "Event 획득" << endl;

	CloseHandle( h );
}

//--------------------------------------------------------------
//
HANDLE h = CreateEvent( 0, FALSE, FALSE, "e"); // auto

HANDLE h1 = CreateEvent( 0, TRUE, FALSE, "e"); // 동일 이름이 존재 하면	
											// 1, 2, 3 의 인자는 무시 된다.

HANDLE h2 = CreateMutex( 0, 0, "e"); // ?? 실패.. 모든 KO는 동일 이름 공간 사용.

















