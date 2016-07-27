//--------------------------------------------------------------
// Step1.  Align(정렬단위), Big endian & little endian
//----------------------------------------------------------------

#include <stdio.h>

// 컴파일러에게 정렬(Align)의 단위를 1byte 지정한다.
#pragma pack( 1 )

/*
struct DATA
{
	char c;
	int  n;
};

void main()
{
	printf("%d\n", sizeof( DATA ) ); // 실행하지 말고 예상해 보세요.
}
*/



#include <winsock2.h>
#pragma comment( lib, "ws2_32.lib") // 모든 socket 관련 함수는 ws2_32.dll에 있다.


void main()
{
//	int x = 0x12345678; // little endian 방식으로 저장 

	int x = htonl( 0x12345678);  // host방식의 data를 network 방식으로 변환
								// 즉 Big Endian으로 저장 

	char* p = (char*)&x;

	printf( "%x\n", *p);  // 12 ? 78 ?
}







