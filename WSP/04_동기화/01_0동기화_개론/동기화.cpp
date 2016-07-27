// 동기화

#include <windows.h>
#include <iostream>
using namespace std;

BOOL bWait = TRUE;

void WorkFunc()
{
	for( int i=0; i<10000000; i++);	// 시간 지연 루틴
}
DWORD WINAPI Func(PVOID p)
{
	cout << "==> Func Start" << endl;
	for( int i=0; i<40; i++)
	{
		WorkFunc();
		cout << "gogo........................" << endl;
	}
	cout << "==> Func End" << endl;
	bWait = FALSE;
	return 0;
}
void main()
{
	DWORD tid;
	HANDLE hThread = CreateThread(NULL, 0, Func, 0, NORMAL_PRIORITY_CLASS, &tid);
	for( int i=0; i<20; i++)
	{
		WorkFunc();
		cout << "main  " << endl;
	}
	while( bWait )
	{

	}
	CloseHandle(hThread);
}




