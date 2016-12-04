#ifndef XXX_Thread_H
#define XXX_Thread_H

typedef void * HANDLE;

class TThreadListener;

class TThread
{
private:
	unsigned long FThreadID;
	HANDLE FHandle;
	bool FSuccessMake;
	TThreadListener * FListener;
#ifdef WIN32
	static unsigned long __stdcall Wrapper(void * arg);
#elif defined(LINUX)
	static void * Wrapper(void * arg);
#else
#error	"Please fix os ver"
#endif


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
