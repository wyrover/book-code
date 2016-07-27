// 모듈 열거 하기.
#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
using namespace std;

// 특권을 부여하는 함수.
void EnablePri( char* name, BOOL bEnable ) // 특권이름, 가능여부
{
	HANDLE hToken;
	OpenProcessToken( GetCurrentProcess(), // 프로세스 핸들
					  TOKEN_ADJUST_PRIVILEGES, 
					  &hToken );
	// 특권 이름으로 부터 특권 ID( 64비트)를 얻는다.
	LUID luid;
	LookupPrivilegeValue( ".", // PC 이름
						  name, // 특권이름
						  &luid); // luid를 담을 변수.
	TOKEN_PRIVILEGES tp;
	tp.PrivilegeCount = 1; // 갯수
	tp.Privileges[0].Luid = luid; // 특권 번호
	tp.Privileges[0].Attributes = bEnable ? SE_PRIVILEGE_ENABLED : 0;

	AdjustTokenPrivileges( hToken, FALSE , &tp, sizeof(tp), 0, 0);
}

void main( int argc, char** argv)
{
	// 시스템 프로세스에 접근할수있는 특권 부여
	EnablePri( SE_DEBUG_NAME, TRUE );

//	if ( argc != 2 )
//	{
//		cout << "usage : mds <pid>" << endl;
//		return ;
//	}
	//----------------------------------
	DWORD pid = 936;//atoi(argv[1]);
	HANDLE hSnap = CreateToolhelp32Snapshot( TH32CS_SNAPMODULE, pid );

	MODULEENTRY32 me;
	BOOL b = Module32First( hSnap, &me );
	while ( b )
	{
		cout << (void*)me.modBaseAddr << " : " << (char*)me.szModule << endl;

		b = Module32Next( hSnap, &me);
	}
	CloseHandle( hSnap );
}


































