#include <windows.h>
#include <iostream.h>

DWORD WINAPI Fun1Proc(
  LPVOID lpParameter   // thread data
);

DWORD WINAPI Fun2Proc(
  LPVOID lpParameter   // thread data
);

int tickets=100;

CRITICAL_SECTION g_csA;
CRITICAL_SECTION g_csB;

void main()
{
	HANDLE hThread1;
	HANDLE hThread2;
	hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);
	hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);
	CloseHandle(hThread1);
	CloseHandle(hThread2);

	InitializeCriticalSection(&g_csA);
	InitializeCriticalSection(&g_csB);
	Sleep(4000);

	DeleteCriticalSection(&g_csA);
	DeleteCriticalSection(&g_csB);
}

DWORD WINAPI Fun1Proc(
  LPVOID lpParameter   // thread data
)
{
	while(TRUE)
	{
		EnterCriticalSection(&g_csA);
		Sleep(1);
		EnterCriticalSection(&g_csB);
		if(tickets>0)
		{
			Sleep(1);
			cout<<"thread1 sell ticket : "<<tickets--<<endl;
		}
		else
			break;
		LeaveCriticalSection(&g_csB);
		LeaveCriticalSection(&g_csA);
	}
	
	return 0;
}

DWORD WINAPI Fun2Proc(
  LPVOID lpParameter   // thread data
)
{
	
	while(TRUE)
	{
		EnterCriticalSection(&g_csB);
		Sleep(1);
		EnterCriticalSection(&g_csA);
		if(tickets>0)
		{
			Sleep(1);
			cout<<"thread2 sell ticket : "<<tickets--<<endl;
		}
		else
			break;
		LeaveCriticalSection(&g_csA);
		LeaveCriticalSection(&g_csB);
	}
	cout<<"thread2 is running!"<<endl;
	return 0;
}