#include <windows.h>
#include <stdio.h>
#include <conio.h>
void main()
{
	const int size = 1024*1024*100;	// 100M

	char* base = (char*)VirtualAlloc( (void*)0x0, // 원하는 가상주소(64k 배수)
								size * 15,		  // 크기
								MEM_RESERVE,	  // 예약만
								PAGE_READWRITE ); // 보호속성

	printf("예약된 주소 : %p\n", base );
//	*base = 'a'; // runtime error - 아직 물리 공간이 없다.
	//--------------------------------------------------
	char* p = base;
	for ( int i = 0; i < 15; ++i )
	{
		getch();
		void* temp = VirtualAlloc( p, size, MEM_COMMIT, PAGE_READWRITE);

		printf("확정된 주소 : %p\n", temp );
		p = p + size;
	}
	VirtualFree( p, size *15, MEM_DECOMMIT ); // 다시 예약 상태로.
	VirtualFree( p, size *15, MEM_FREE );	  // 다시 Free 상태로.
}
