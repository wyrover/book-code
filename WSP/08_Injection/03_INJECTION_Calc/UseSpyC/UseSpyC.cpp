// console

#include <windows.h>
#include <iostream>
using namespace std;


void main()
{
	HWND hwnd = FindWindow("SciCalc", "계산기");
	if( hwnd == 0 )
	{
		cout << "계산기가 없다. " << endl;
		return;
	}

	// 계산기의 Process ID
	DWORD pid;
	GetWindowThreadProcessId(hwnd, &pid);

	// ID -> Handle
	HANDLE hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

	// kernel32.dll의 위치를 얻는다.
	HMODULE hdll = GetModuleHandle("kernel32.dll");

	// Kernel32 안에 실제 LoadLibrary 어드레스를 얻음.
	// LoadLibrary => dll을 로드하는 함수
	PTHREAD_START_ROUTINE func =		
			(PTHREAD_START_ROUTINE)GetProcAddress(hdll, "LoadLibraryA");

	//------------------------------------------------------------
	char file[] = "SpyC.dll";

	char *p = (char*)VirtualAllocEx(hProcess, 0, sizeof(file), 
		MEM_RESERVE | MEM_COMMIT , PAGE_READWRITE);

	if ( p == 0 )
	{
		cout << "메모리 할당 실패 " << endl;
		CloseHandle(hProcess);
		return;
	}

	// 할당 받은 메모리에 문자열 기록, 다른 프로세스에 문자열을 씀
	DWORD len;
	WriteProcessMemory( hProcess, p, file, sizeof(file), &len);

	//-------------------------------------------------------------
	// 해당 프로세스에 스레드 생성
	HANDLE hThread = CreateRemoteThread(hProcess, NULL, 0, 
		func,	// 함수
		p,		// 파라미터
		0, 0);

	CloseHandle(hThread);
	CloseHandle(hProcess);
}


















