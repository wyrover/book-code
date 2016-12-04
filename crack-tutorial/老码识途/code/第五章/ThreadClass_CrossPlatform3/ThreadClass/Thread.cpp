#include "Thread.h"

TThreadAPIBinder TThread::FAPIBinder;

TThread::TThread(Runnable * runnable)
{
	//FListener = NULL;
	FRunnable = runnable;
	if (runnable != NULL)
	{
		runnable->SetThread(this);
	}
	
	FThreadID = 0;
	FHandle = 0;
};

void * TThread::Wrapper(void * arg)
{
	TThread * Thd;
	int i;
	Thd = ((TThread *) arg);
	unsigned long ret = Thd->FRunnable->execute();
	Thd->FReturnValue = ret;
	FAPIBinder.ExitFromThread(ret);
	return (void *) (Thd->FReturnValue);
};

bool TThread::Run()
{   
	int error;
	FHandle = FAPIBinder.MakeThread(&FThreadID, Wrapper, this, &error);
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
		FAPIBinder.Cleanup((unsigned long)FHandle);
	}
	if (FRunnable != NULL)
		delete FRunnable;
};

void TThread::WaitFor()
{
	int Result;
	void * P;
	if (!FSuccessMake)
		return;
	P = &Result;
	FAPIBinder.WaitFor((unsigned long)FHandle);
};

int TThread::GetThreadID()
{
	return FThreadID;
};

HANDLE TThread::GetHandle()
{
	return FHandle;
};

void TThread::Stop()
{
	FRunnable->stop();
}

void Runnable::stop()
{
	isStop = true; 
}
