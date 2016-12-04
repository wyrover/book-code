#ifndef XXX_Thread_H
#define XXX_Thread_H

#include <malloc.h>
#include "ThreadAPIBinder.h"

class TThreadListener;


class TThread
{
private:	
	unsigned long FThreadID;
	bool FSuccessMake;
	HANDLE FHandle;
	TThreadListener * FListener;
	static void * Wrapper(void * arg);
protected:
	int FReturnValue;
	virtual void Execute() = 0;
public:
	TThread();
	TThread(TThreadListener * Listener);
	~TThread();
	bool Run();
	void WaitFor();
	int GetThreadID();
	HANDLE GetHandle();
	int getReturnValue(){return FReturnValue;};
};

class TThreadListener
{
public:
	virtual void NotifyThreadFinished(TThread * Sender) = 0;
};




#endif
