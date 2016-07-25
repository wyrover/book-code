#include <windows.h>
#include <iostream.h>

DWORD WINAPI Fun1Proc(
  LPVOID lpParameter   // thread data
);

DWORD WINAPI Fun2Proc(
  LPVOID lpParameter   // thread data
);
int index=0;
int tickets=100;
HANDLE hMutex;
void main()
{
	HANDLE hThread1;
	HANDLE hThread2;
	hThread1=CreateThread(NULL,0,Fun1Proc,NULL,0,NULL);
	hThread2=CreateThread(NULL,0,Fun2Proc,NULL,0,NULL);
	CloseHandle(hThread1);
	CloseHandle(hThread2);

//	hMutex=CreateMutex(NULL,TRUE,NULL);
	hMutex=CreateMutex(NULL,TRUE,"tickets");
	if(hMutex)
	{
		if(ERROR_ALREADY_EXISTS==GetLastError())
		{
			cout<<"only one instance can run!"<<endl;
			return;
		}
	}
	WaitForSingleObject(hMutex,INFINITE);
	ReleaseMutex(hMutex);
	ReleaseMutex(hMutex);

	Sleep(4000);
}

DWORD WINAPI Fun1Proc(
  LPVOID lpParameter   // thread data
)
{
/*	while(TRUE)
	{
		WaitForSingleObject(hMutex,INFINITE);
		if(tickets>0)
		{
			Sleep(1);
			cout<<"thread1 sell ticket : "<<tickets--<<endl;
		}
		else
			break;
		ReleaseMutex(hMutex);
	}
*/
	WaitForSingleObject(hMutex,INFINITE);
	cout<<"thread1 is running"<<endl;

	return 0;
}

DWORD WINAPI Fun2Proc(
  LPVOID lpParameter   // thread data
)
{
/*	while(TRUE)
	{
		WaitForSingleObject(hMutex,INFINITE);
		if(tickets>0)
		{
			Sleep(1);
			cout<<"thread2 sell ticket : "<<tickets--<<endl;
		}
		else
			break;
		ReleaseMutex(hMutex);
	}

*/
	WaitForSingleObject(hMutex,INFINITE);
	cout<<"thread2 is running"<<endl;
	return 0;
}