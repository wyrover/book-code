//---------------------------------------------------
//  SetFilePointer 사용

//  대부분의 운영체제에서는 열린 파일마다 읽기/쓰기를 위한
//  현재 위치 정보를 유지한다.
//  Windows에서는 이를 파일 포인터라고 한다.
//  즉 다음에 읽기/쓰기를 할 대상 데이터의 파일 선두로부터의 
//  오프셋이다.
//  지금 막 열린 파일의 파일 포인터는 파일의 선두에 위치한다.

//  그리고 ReadFile이나 WriteFile을 호출하면 실제로 읽고 쓴
//  만큼 파일 포인터가 파일의 뒤쪽으로 이동한다.
//  ReadFile을 계속 호출해서 연속적으로 읽을 수 있는 이유도 
//  이 때문이다.

//  파일 포인터는 데이터가 있는 범위라면 SetFilePointer API
//  를 사용해서 자유롭게 이동할 수 있고, 이를 탐색(SEEK)라 한다.

//  파일의 특정 위치에서 읽고 쓰려면 그 위치로 파일 포인터를 
//  옮기고 나서 ReadFile / WriteFile 을 호출하면 된다.

//  DWORD SetFilePointer(		반환값 : 이동후 파일포인터의 하위 
//										 32bit위치
//   HANDLE hFile,				 파일 핸들
//   LONG lDistanceToMove,		 이동할 상대 거리의 하위 32bit
//   PLOING lpDistanceToMoveHigh, 이동할 상대 거리의 상위 32bit를 
//								  넣은 변수의 주소, 이동후의 상위 
//								  32bit가 반환됨
//   DWORD dwMoveMethod			  이동 기준 위치 
//---------------------------------------------------

#include <windows.h>
#include <stdio.h>
  
int main()
{
	// 읽기 모드로 새로 생성
	HANDLE hf = CreateFile("test.dat",
		GENERIC_WRITE | GENERIC_READ,
		0, NULL, CREATE_ALWAYS, 0, NULL);
	if(hf != INVALID_HANDLE_VALUE)
	{
		char *data = "테스트 용\r\n";
		char buf[256];
		DWORD written, read;

		// 데이터 기록
		WriteFile(hf, data, strlen(data), &written, NULL);

		// 파일 포인터 이동
		SetFilePointer(hf, 0, NULL, FILE_BEGIN);

		// 쓰여진 부분만 읽기
		ReadFile(hf, buf, sizeof(buf), &read, NULL);

		buf[read] = '?';

		// 읽은 데이터 출력
		printf("%s", buf);

		CloseHandle(hf);
		//마무리 중요 
		// CloseHandle() : 리턴값 TRUE 를 반드시 확인하자 .
	}
	return 0;
}
