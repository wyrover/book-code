#include <stdio.h>
#include <windows.h>

void main()
{
	// 프로세스의 기본 힙의 핸들을 구한다.
	HANDLE h1 = GetProcessHeap();
	HANDLE h2 = HeapCreate( 0, 1024*1024, 0); // flag, 초기 크기, 최대 크기

	// 힙에서 메모리 할당
	void* p1 = HeapAlloc( h1, HEAP_ZERO_MEMORY, 100);
	void* p2 = HeapAlloc( h2, HEAP_ZERO_MEMORY, 100);

	printf("할당한 메모리 주소 : %p\n", p1 );
	printf("할당한 메모리 주소 : %p\n", p2 );

	HeapFree( h1, 0, p1 );
	HeapFree( h2, 0, p2 );

	// 생성한 힙 파괴(기본힙은 파괴하지 않는다.)
	HeapDestroy( h2 );
}
// 결국은 malloc() 과 비슷한 용도로 사용하면 된다.!










