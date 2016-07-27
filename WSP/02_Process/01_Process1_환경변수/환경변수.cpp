// 환경변수

#include <iostream.h>
#include <windows.h>

void main()
{
	// Process 생성 함수 
//	WinExec("calc.exe", SW_SHOW);
//	ShellExecute(0, "open", "calc.exe", 0, 0, SW_SHOW);

	// 현재 프로세스의 현재 디렉토리 얻기
	char buf[256];
	GetCurrentDirectory(256, buf);
	cout << buf << endl;

	//---------------------------------------------

	// 환경 변수블록( 일종의 메모리 공간)에 값 넣기
	
	SetEnvironmentVariable("mon", "월요일");
	char temp[256];
	GetEnvironmentVariable("mon", temp, 256);
	cout << temp << endl;
	//-------------------------------------------------------
	cout << __argc  << endl;
	cout << __argv  << endl;
}


