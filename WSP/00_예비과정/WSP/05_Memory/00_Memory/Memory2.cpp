// Memory 


#include <windows.h>
#include <iostream>
using namespace std;

/*
void main()
{
	// 메모리 할당 가능 함수(연산자) ; malloc, new, heapAlloc, VirtualAlloc
	// GlobalAlloc, LocalAlloc : win16 함수 

	BYTE *p  = (BYTE*)malloc(10);
	BYTE *p1 = new	BYTE[10];

	// 연속된 메모리 공간을 가짐... ( defult heap)
	cout << (void*)p  << endl;
	cout << (void*)p1 << endl;

	delete p;
	free(p1);
}
*/

/*
default Heap =>  예약 상태 
Heap ??		 =>  
*/

/*
void main()
{
	// default Heap의 핸들을 얻는 함수.
	HANDLE	hHeap1	= GetProcessHeap();

	// 새로운 힙 생성
	HANDLE	hHeap2	= HeapCreate( 0, // flag
								1024*1024, // 초기 크기 (1M - 확정)
								1024*1024*10);	// 최대 크기(예약)

	char* p	= (char*)HeapAlloc(hHeap1, // 핸들
								0,     // flag
								100);  // 크기 ( byte 단위 )

	char *p1 = (char*)HeapAlloc(hHeap1, 0, 100);
	char *p2 = (char*)HeapAlloc(hHeap1, 0, 400);

	// 메모리 초기화 ..... => 확정 0
	char *t  = (char*)HeapAlloc(hHeap2, HEAP_ZERO_MEMORY, 1024*1024);
//	char *t1 = (char*)HeapAlloc(hHeap2, HEAP_ZERO_MEMORY, 1024*1024);

	// 주소 출력
	cout << (void*)p << endl;
	cout << (void*)p1 << endl;
	cout << (void*)p2 << endl;
	cout << (void*)t  << endl;

	// 확정된 메모리 공간의 byte 리턴..
	DWORD lp = HeapSize(hHeap1, 0, p1);
	cout << "l " << lp << endl;
}
*/

class Node
{
public:
	static HANDLE	s_hHeap;
	static int		count;

	void *operator new(size_t s)		// new 재정의
	{
		if( s_hHeap == 0 )
		{
			s_hHeap = HeapCreate(0, 0, 0);
			if( s_hHeap == NULL)
				return NULL;
		}
		void *p = HeapAlloc( s_hHeap, 0, s);
		count++;
		return p;
	}
	void operator delete(void *p)		// delete 재정의
	{
		HeapFree( s_hHeap, 0, p);
		if( --count ==  0 )
			HeapDestroy(s_hHeap);
	}
};
HANDLE	Node::s_hHeap	= 0;
int		Node::count		= 0;

void main()
{
	Node *p1 = new Node;		Node *p2	= new Node;
	cout << p1 << endl;			cout << p2 << endl;
	delete p1;					delete p2;

}






















