// ErrorHandling
// 함수 호출 후 리턴값 확인 작업
// 성공과 실패 및 이유 확인 작업 

#include <windows.h>
#include <iostream>
using namespace std;

// 내맘대로 에러 코드 정의
#define ERROR_MYERROR 100 

// 사용자 정의 함수 작성시
BOOL foo()
{
	SetLastError(ERROR_MYERROR);
	return FALSE;
}

void main()
{

	// 1. 함수 호출을 통한 확인
/*
	HWND hwnd = CreateWindow(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
	if( hwnd ==  0 )
	{
		DWORD err = GetLastError();
		cout << "error : " << err << endl;

		char msg[256];
		FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, 0, 
			err, 0, msg, 256, 0);

		cout << msg << endl;
	}
*/
	// 1. GetLastError() -> FormatMessage
	// 2. ErrorLookup 툴 사용
	// 3. Debuging 모드 => @err,hr ( watch 창)

	// CreateWindow()	실패시 0을 리턴.
	// CreateProcess()  실패시 -1을 리턴
	// CreateFile()     실패시 INVALID_HANDLE_VALUE(-1)를 리턴
	// 성공시 모두 HANDLE을 리턴..


	// 2. 나만의 에러 처리 번호 정의 
	if( foo() == FALSE)
	{
		DWORD e = GetLastError();
		cout << "사용자 정의 에러 :" << e << endl;
	}
	

}










