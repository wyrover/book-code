

#define _WIN32_WINNT 0x0500
#define WINVER      0x0500
#include <windows.h>
#include <iostream.h>
#include <aclapi.h>


void main()
{

	HANDLE hToken;
	OpenProcessToken( GetCurrentProcess(), 
		TOKEN_ADJUST_PRIVILEGES, &hToken );
	LUID luid;

	// 특권의 이름(SE_DEBUG_NAME)  ==> LUID로 변환 
	// - 특권 : 문자열로 정의되어 있고 LUID라는 값으로 사용함 
	LookupPrivilegeValue( ".", SE_DEBUG_NAME, &luid );
	

	// 특권 설정을 변경시킬때 사용되는 구조체 
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1;
	tp.Privileges[0].Luid = luid;
	tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
	
	// 프로세스의 액세스 토큰에 특권 부여.
//	AdjustTokenPrivileges( hToken, 0, &tp, sizeof(tp), 0, 0);

	HWND hwnd = FindWindow( "SciCalc", "계산기");
	DWORD pid;
	DWORD tid = GetWindowThreadProcessId( hwnd, &pid);




	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );
	
	if (hProcess  )
	{
		BOOL b = TerminateProcess( hProcess, 0);
		
		if ( b ) 
			cout <<"Terminate" << endl;
		else
			cout <<"Faile"<< endl;
	}
	else
		cout <<"Fail To Get Handle" << endl;
}
