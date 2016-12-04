#include "Thread.h"
#include <windows.h>


TThread::TThread()
{  FListener = NULL;
FThreadID = 0;
FHandle = 0;
};

TThread::TThread(TThreadListener * Listener)
{  FListener = Listener;  
FThreadID = 0;
FHandle = 0;
};

unsigned long __stdcall TThread::Wrapper(void * arg)
{  
	TThread * Thd;
	Thd = ((TThread *) arg);
	Thd->Execute();
	if (Thd->FListener != NULL)
		Thd->FListener->NotifyThreadFinished(Thd); 
	ExitThread(Thd->FReturnValue);
	return Thd->FReturnValue;
};

bool TThread::Run()
{  
	FHandle = CreateThread(NULL, 0, Wrapper, this, 0, &FThreadID);
	if (FHandle == 0)
		FThreadID = 0;
	return FHandle != 0; 
};

TThread::~TThread()
{ 
	if (FThreadID != 0)  { 
		WaitFor(); 
		CloseHandle(FHandle);
	}
	if (FListener != NULL)    
		delete FListener;
};

void TThread::WaitFor(){ 
	if (FThreadID == 0)    return; 
	WaitForSingleObject(FHandle, INFINITE);
};

int TThread::GetThreadID()
{  
	return FThreadID;
};

HANDLE TThread::GetHandle()
{
	return FHandle;
};
