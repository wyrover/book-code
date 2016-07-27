//---------------------------------------------------
//  동시 접근 제어 A
//  - Win32 API를 사용하여 파일조작시 얻는 장점 중 하나
//    
//  - 배타적 제어 : 자신이 조작중인 파일에 대해 다른 프로그램
//    의 접근을 허가할지 말지를 제어하는 것 

//  - CreateFile의 세번째 인자를 활용함 
//    0 : 닫기 전까지 그 파일을 다른 프로그램에서 사용 불가 
//    FILE_SHARE_READ : 다른 프로그램에서 읽기 허용
//    FILE_SHARE_WRITE : 다른 프로그램에서 쓰기 허용
//    OR 연산자로 조합 가능 

//  Sample 확인)
//   - 배타적 접근 에러 
//  - 읽기 허용 : 양쪽 모두 읽기 허용 처리 필요 
//---------------------------------------------------

#include <windows.h>
#include <stdio.h>

int main()
{
	
	HANDLE hf = CreateFile("c://test.dat", GENERIC_WRITE,
//		0, 
		FILE_SHARE_READ, 
		NULL, OPEN_ALWAYS, 0, NULL);

	if( hf != INVALID_HANDLE_VALUE)
	{
		// C라이브러리와 달리 API에는 모드의 구분이 없다.
		// 따라서 개행 처리는 \r\n으로 한다.
		char *data = "테스트 용\r\n";
		DWORD written;
		WriteFile(hf, data, strlen(data), &written, NULL);
		printf("Enter 키로 종료\n");
		getchar();
		CloseHandle(hf);
	}

	return 0;
}