#include "ThreadAPIBinder.h"

#ifdef LINUX

#include <pthread.h>

HANDLE TThreadAPIBinder::MakeThread(unsigned long * ThreadID, void * (*   StartAddress)(void *) , void * arg, int *error)
{
	* error = pthread_create(ThreadID, NULL, StartAddress, arg);
	return (HANDLE)(* ThreadID);
};

long TThreadAPIBinder::WaitFor(unsigned long Handle)
{
	long Result;
	void * P;
	P = &Result;
	pthread_join(Handle, &P);
	return Result;
};

void TThreadAPIBinder::ExitFromThread(unsigned long ExitCode)
{
	pthread_exit((void *) ExitCode);
};

void TThreadAPIBinder::Cleanup(unsigned long Handle)
{
	pthread_detach(Handle); 
};

#endif