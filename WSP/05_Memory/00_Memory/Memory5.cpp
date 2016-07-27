// 공유 메모리 

#include <iostream>
using namespace std;

/*
모든 exe와 DLL 파일 이미지는 여러 부분의 집합으로 구성되어 있다.
이를 Section이라 함..

.exe : 프로그램 컴파일시 모든 코드들
.bass : 초기화되지 않은 데이터들
.data : 초기화된 데이터(전역변수)
.rdata : 읽기 전용 data

  => 이렇게 섹션으로 데이터와 코드를 분리시키고, 
     섹션마다의 특징들을 가지고 있다.

  => 이 외에도 사용자 섹션을 만들수 있다.=> 공유할 목적으로...
*/

#pragma data_seg("AAA")
int a = 0;					// 초기화 된 데이터 ( AAA secion )
int b;						// 초기화 되지 않은 데이터 ( .bss section ) 
#pragma data_seg()

#pragma comment(linker, "/SECTION:AAA,RWS")		
// Section 영역에 Read, write, shared 속성을 줌..


void main()
{
	a++;
	b++;
	cout << a << endl;
	cout << b << endl;

	int b;
	cin >> b;
}





















