#include <stdio.h>
#include <windows.h>
#include <conio.h>

void main()
{
	DWORD pid   = 2156; // 목적 프로세스의 ID
	void* pAddr = (void*)0x0012FE80; // 목적 주소.

	HANDLE hProcess = OpenProcess( PROCESS_ALL_ACCESS, 0, pid );

	while ( 1 )
	{
		getch();

		// 다른 프로세스(hProcess)의 메모리(pAddr)을 읽어 온다.
		char buf[256] = {0};
		DWORD len;

		ReadProcessMemory( hProcess, pAddr, buf, 256, &len );

		printf("읽어온 data : %s\n", buf );
	}
}














