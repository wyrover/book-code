// Semaphre
#include <windows.h>
#include <stdio.h>

void main()
{
	HANDLE hSemaphore = CreateSemaphore( 0, // 보안
										3,  // count 초기값
										3,	// 최대 count
										"s"); // 이름
	printf("세마포어를 대기합니다.\n");

	WaitForSingleObject( hSemaphore, INFINITE ); //++count

	printf("세마 포어를 획득\n");
	MessageBox(0, "Release??","", MB_OK);

	LONG old;
	ReleaseSemaphore( hSemaphore, 1, &old ); // count -= 2nd parameter

	CloseHandle( hSemaphore );
}

// 메세지 Box 의 OK를 누르지 말고.. 4번 이상 실행해 보세요..













/*
count 값을 가지고 있다.(접근 가능한 갯수)

프로그램의 인스턴스 갯수를 제어하는 데 유용 
다운로드 할 때 최대 다운로드 숫자를 제어하는 데 사용
*/
/*
#include <windows.h>
#include <iostream>
using namespace std;



void main()
{
	HANDLE	h = CreateSemaphore( NULL, 3, 3, // 초기 count, 최대 count
					"sem");		// 공유할 이름

	cout << "세마포어를 기다린다." << endl;
	WaitForSingleObject(h, INFINITE);

	cout << "DownLoading.............." << endl;
	for( int i=0; i<100; i++)
	{
		cout << ".";
		for( int j=0; j< 10000000; j++);
	}
	MessageBox(NULL, "End DownLoad", "", MB_OK);
	long old;
	ReleaseSemaphore(h, 1, &old);		// count 1증가
	CloseHandle(h);
}
*/