#include <stdio.h>
#include <windows.h>

void DLLInject(DWORD pid, char* path )
{
	// process ID를 가지고 Process 핸들을 얻는다.
	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );

	//---------------------------------------------
	// Kernel32.dll의 주소를 구한다.
	HMODULE hDll = GetModuleHandle( "Kernel32.dll");

	PTHREAD_START_ROUTINE f = (PTHREAD_START_ROUTINE)
							GetProcAddress( hDll, "LoadLibraryA" );
	
	// PTHREAD_START_ROUTINE 는 미리 정의된 스레드 함수모양의 함수포인터 type

	// NateOn의 가상주소 공간을 할당한다.
	void* p = VirtualAllocEx( hProcess, 0, strlen(path)+1, 
									MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);

	DWORD len;
	WriteProcessMemory( hProcess, p, path, strlen(path)+1, &len );

	//-----------------------------------------
	// 상대 프로세스에 새로운 스레드를 생성한다.(Win2000 이상 부터 지원)
	HANDLE hThread = CreateRemoteThread( hProcess, 0, 0,
										 f, p, // 함수, 인자  
										 0,  0);
	CloseHandle( hProcess );
	CloseHandle( hThread );
}




void main()
{
	HWND hwnd = FindWindow( 0, "지뢰 찾기");
	if ( hwnd == 0 )
	{
		printf("NateOn 을 먼저 실행하세요\n");
		return;
	}
	DWORD pid;
	DWORD tid = GetWindowThreadProcessId( hwnd, &pid );

	DLLInject( pid, "D:\\speedhack.dll");	// <<================
}






