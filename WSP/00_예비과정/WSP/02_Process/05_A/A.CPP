#include <stdio.h>
#include <windows.h>

void main()
{
	HANDLE hFile = CreateFile( "a.txt", GENERIC_READ | GENERIC_WRITE,
								FILE_SHARE_READ | FILE_SHARE_WRITE, 
								0, // 보안
								CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);

	printf( "생성된 화일 핸들(Table Index) : %x\n", hFile );

	//--------------------------------------------------
	HWND hwnd = FindWindow( 0, "B");

	// B에게 핸들을 바로 전달한다.
//	SendMessage( hwnd, WM_USER+100, 0,  (LPARAM)hFile );

	// hwnd을 만든 프로세스의 ID를 구한다.
	DWORD pid;
	DWORD tid = GetWindowThreadProcessId( hwnd, &pid );

	// 프로세스 ID를 가지고 PROCESS 핸들을 얻는다.
	HANDLE h;
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );
	
	// A의 Table의 내용을 B의 Table 에 복사 해준다.
	DuplicateHandle( GetCurrentProcess(), hFile, // source
					 hProcess,            &h,    // target
					 0, 0, DUPLICATE_SAME_ACCESS);

	printf("B에 복사한 핸들(Table index) : %x\n", h );

	SendMessage( hwnd, WM_USER+100, 0, (LPARAM) h ); // ??? 채워 보세요...






	CloseHandle( hFile ); // 1. KOHT에서 항목삭제.
						 //  2. if ( --(hFile.참조개수) == 0) delete 화일KO;
}


		
// 다하신 분은 VC++ 하나 더 실행해서 "B"라는 이름의 Win32 Application 기본
// 코드 만드세요.

// 윈도우의 캡션바를 "B"로 변경하세요(CreateWindowEx() 3번째 인자 변경.
















