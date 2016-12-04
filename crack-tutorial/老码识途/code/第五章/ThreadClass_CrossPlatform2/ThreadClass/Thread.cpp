#include "Thread.h"
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

void * TThread::Wrapper(void * arg)
{
	TThread * Thd;
	int i;
	Thd = ((TThread *) arg);
	Thd->Execute();
	if (Thd->FListener != NULL)
		Thd->FListener->NotifyThreadFinished(Thd);
	TThreadAPIBinder::ExitFromThread(Thd->FReturnValue);
	return (void *) (Thd->FReturnValue);
};

bool TThread::Run()
{   
	int error;
	//双重包装保证MakeThread的Wrapper参数类型不随系统不同而不同
	FHandle = TThreadAPIBinder::MakeThread(&FThreadID, Wrapper, this, &error);
	FSuccessMake = error == 0;
	if (!FSuccessMake)
		FThreadID = 0;
	return FSuccessMake; 
};

TThread::~TThread()
{
	if (FSuccessMake)
	{
		WaitFor();
		TThreadAPIBinder::Cleanup((unsigned long)FHandle);
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
	TThreadAPIBinder::WaitFor((unsigned long)FHandle);
};

int TThread::GetThreadID()
{
	return FThreadID;
};

HANDLE TThread::GetHandle()
{
	return FHandle;
};

