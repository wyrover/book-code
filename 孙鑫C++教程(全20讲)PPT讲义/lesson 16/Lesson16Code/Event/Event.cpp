#include <windows.h>
#include <iostream.h>

DWORD WINAPI Fun1Proc(
  LPVOID lpParameter   // thread data
);

DWORD WINAPI Fun2Proc(
  LPVOID lpParameter   // thread data
);

int tickets=100;
HANDLE g_hEvent;

void main()
{
	HANDLE hThread1;
	HANDLE hThread2;
	hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);
	hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);
	CloseHandle(hThread1);
	CloseHandle(hThread2);

	//g_hEvent=CreateEvent(NULL,FALSE,FALSE,NULL);
	g_hEvent=CreateEvent(NULL,FALSE,FALSE,"tickets");
	if(g_hEvent)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			cout<<"only instance can run!"<<endl;
			return;
		}
	}
	SetEvent(g_hEvent);

	Sleep(4000);
	CloseHandle(g_hEvent);
}

DWORD WINAPI Fun1Proc(
  LPVOID lpParameter   // thread data
)
{
	while(TRUE)
	{
		WaitForSingleObject(g_hEvent,INFINITE);
//		ResetEvent(g_hEvent);
		if(tickets>0)
		{
			Sleep(1);
			cout<<"thread1 sell ticket : "<<tickets--<<endl;
		}
		else
			break;
		SetEvent(g_hEvent);
	}
	
	return 0;
}

DWORD WINAPI Fun2Proc(
  LPVOID lpParameter   // thread data
)
{
	
	while(TRUE)
	{
		WaitForSingleObject(g_hEvent,INFINITE);
//		ResetEvent(g_hEvent);
		if(tickets>0)
		{
			Sleep(1);
			cout<<"thread2 sell ticket : "<<tickets--<<endl;
		}
		else
			break;
		SetEvent(g_hEvent);
	}
	
	return 0;
}