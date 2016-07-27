#include <windows.h> // user32.dll의 모든 함수 선언.

#include <stdio.h>

// DLL 사용하기
//#include "xyz.h"					// 관련 헤더 include
//#pragma comment(lib, "xyz.lib")		// 라이브러리 추가
void main()
{
	MessageBox(0,"A","",MB_OK);

//	int s = Add(10,20);			// DLL 함수 사용
//	printf("결과 : %d\n", s);

	void* p1 = GetModuleHandle( "xyz.dll");
	printf("xyz.dll 주소 : %p\n", p1);
//	printf("Add 주소 : %p\n",    Add);
}
// xyz.h 와 xyz.lib 만 복사 해 오세요. 아직 xyz.dll은 복사 하지 마세요

// 컴파일/link 하시면 실행 파일(exe)가 생성됩니다.

// 실행해 보세요 -> DLL을 찾을수 없다는 에러가 나옵니다. 확인해 보세요

// 이제 xyz.dll을 복사해오세요. 다시 실행해 보세요..






