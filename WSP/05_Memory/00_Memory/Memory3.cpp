// virtual Memory

#include <windows.h>
#include <iostream>
using namespace std;


void main()
{
	char *p = (char*)VirtualAlloc( NULL,			// 원하는 주소(64K 배수)
								1024*1024*50,		// 크기 (50MB)
								MEM_RESERVE,		// 예약
								PAGE_READWRITE);	// 보호 속성

	cout << (void*)p << endl;

	char *p1 = (char*)VirtualAlloc(p,				// 원하는 주소
									1024*1024*10,	// 크기(10MB)
									MEM_COMMIT,		// 확정
									PAGE_READWRITE);// 보호속성
	cout << (void*)p1 << endl;
	//-------------------------------------------------------

	char *temp = p;
	for( int i=0; i<10; i++)
	{
		int n; cin >> n;
		char *t = (char*)VirtualAlloc(temp, 1024*1024*10, MEM_COMMIT,
								PAGE_READWRITE);

		cout << "확정된 주소 : " << (void*)t << endl;
		temp += 1024 * 1024 * 10;
	}
	// commit ==> decommit
	VirtualFree( p, 1024*1024* 10, MEM_DECOMMIT);

	// free
	VirtualFree( p, 0, MEM_FREE);


}















