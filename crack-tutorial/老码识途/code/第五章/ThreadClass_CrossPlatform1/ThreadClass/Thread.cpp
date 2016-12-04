#include "Thread.h"
#ifdef WIN32
#include <windows.h>

#elif defined(LINUX)
#include <pthread.h>
#else
#error	"Please fix os ver"
#endif

#include <stdio.h>

TThread::TThread()
{
	FListener = NULL;
	FThreadID = 0;
	FHandle = 0;
};

TThread::TThread(TThreadListener * Listener)
{
	FListener = Listener;
	FThreadID = 0;
	FHandle = 0;
};

#ifdef WIN32
unsigned long __stdcall TThread::Wrapper(void * arg)
#elif defined(LINUX)
void * TThread::Wrapper(void * arg);
#else
#error	"Please fix os ver"
#endif

{
	TThread * Thd;
	int i;
	Thd = ((TThread *) arg);
	Thd->Execute();
	if (Thd->FListener != NULL)
		Thd->FListener->NotifyThreadFinished(Thd);
#ifdef WIN32
	ExitThread(Thd->FReturnValue);
	return Thd->FReturnValue;
#elif defined(LINUX)
	pthread_exit(&Thd->FReturnValue);
	return (void *) (Thd->FReturnValue);
#else
#error	"Please fix os ver"
#endif

};

bool TThread::Run()
{
#ifdef LINUX
	FSuccessMake = pthread_create(&FThreadID, NULL, Wrapper, this) != 0;
#elif defined(WIN32)
	FHandle = CreateThread(NULL, 0, Wrapper, this, 0, &FThreadID);
	FSuccessMake = FHandle != 0;
	if (FHandle == 0)
		FThreadID = 0;
#else
#error	"Please fix os ver"
#endif

	return FSuccessMake; 
};

	TThread::~TThread()
	{
		if (FSuccessMake)
		{
			WaitFor();
#ifdef LINUX
			pthread_detach(FThreadID);
#elif defined(WIN32)
			CloseHandle(FHandle);
#else
#error	"Please fix os ver"
#endif

		}
		if (FListener != NULL)
			delete FListener;
};

	void TThread::WaitFor()
	{
		int Result;
		void * P;
		if (!FSuccessMake)
			return;
		P = &Result;
#ifdef WIN32
		WaitForSingleObject(FHandle, INFINITE);
#elif defined(LINUX)
		pthread_join(FThreadID, &P);    
#else
#error	"Please fix os ver"
#endif

	};

	int TThread::GetThreadID()
	{
		return FThreadID;
	};

	HANDLE TThread::GetHandle()
	{
		return FHandle;
	};
