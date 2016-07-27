#include <stdio.h>
#include <windows.h>
#include <conio.h>

DWORD WINAPI foo( void* p)
{
	printf("foo\n");Sleep(5000);
	return 0;					// DllMain() 호출->스레드 종료
}
void main()
{
	getch();
	HMODULE hDll = LoadLibrary("W.dll"); // DllMain()호출.
	if ( hDll == 0)
	{
		printf("DLL을 복사해 오세요\n");
		return;
	}
	getch();
	HANDLE h = CreateThread( 0, 0, foo, 0, 0,0); // DllMain()호출
	WaitForSingleObject( h, INFINITE);

	getch();
	FreeLibrary( hDll );	// DllMain()호출 -> 프로세스에서 해제.
}















