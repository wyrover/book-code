#include "ThreadAPIBinder.h"

#ifdef WIN32
  
#include <windows.h>

unsigned long __stdcall WinWrapper(void * arg){
	struct ThreadWrapperArg* theArg;
	unsigned long ReturnValue;
	theArg = (struct ThreadWrapperArg*) arg;
	ReturnValue = (unsigned long)theArg->StartAddress(theArg->arg);
	free(theArg);
	return ReturnValue;
};

HANDLE TThreadAPIBinder::MakeThread(unsigned long * ThreadID, void * (* StartAddress)(void *) , void * arg, int *error){
	struct ThreadWrapperArg* pWrapperArg;
	HANDLE handle;
	//allocate memory , to prevent from thread access local memory of MakeThread. it will be freed.
	pWrapperArg = (struct ThreadWrapperArg*) malloc(sizeof(struct ThreadWrapperArg));
	pWrapperArg->StartAddress = StartAddress;
	pWrapperArg->arg = arg;
	handle = CreateThread(NULL, 0, WinWrapper, pWrapperArg, 0, ThreadID);
	if (handle == NULL) 
		* error = GetLastError();
	else
		* error = 0;
	return handle;
};

long TThreadAPIBinder::WaitFor(unsigned long Handle)
{
	return WaitForSingleObject((HANDLE) Handle, INFINITE);
};

void TThreadAPIBinder::ExitFromThread(unsigned long ExitCode)
{
	ExitThread(ExitCode);
};

void TThreadAPIBinder::Cleanup(unsigned long Handle)
{ 
	CloseHandle((HANDLE) Handle); 
};

#endif