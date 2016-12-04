#ifndef Threads_HDR_XXXX
#define Threads_HDR_XXXX


#ifdef WIN32
  #include <windows.h>
#elif defined(LINUX)
  #include <pthread.h>  
#else
#error	"Please fix os ver"
#endif
#include <malloc.h>

typedef void * HANDLE;

struct ThreadWrapperArg {
	void * (* StartAddress)(void *);
	void * arg;
};

class TThreadAPIBinder

#ifdef WIN32
{
private:
	static unsigned long __stdcall Wrapper(void * arg){
		struct ThreadWrapperArg* theArg;
		unsigned long ReturnValue;
		theArg = (struct ThreadWrapperArg*) arg;
		ReturnValue = (unsigned long)theArg->StartAddress(theArg->arg);
		free(theArg);
		return ReturnValue;
	};

public:
	static HANDLE MakeThread(unsigned long * ThreadID, void * (* StartAddress)(void *) , void * arg, int *error){
		struct ThreadWrapperArg* pWrapperArg;
		HANDLE handle;
		//allocate memory , to prevent from thread access local memory of MakeThread. it will be freed.
		pWrapperArg = (struct ThreadWrapperArg*) malloc(sizeof(struct ThreadWrapperArg));
		pWrapperArg->StartAddress = StartAddress;
		pWrapperArg->arg = arg;
		handle = CreateThread(NULL, 0, Wrapper, pWrapperArg, 0, ThreadID);
		if (handle == NULL) 
			* error = GetLastError();
		else
			* error = 0;
		return handle;
	};
	static long WaitFor(unsigned long Handle){ return WaitForSingleObject((HANDLE) Handle, INFINITE);};
	static void ExitFromThread(unsigned long ExitCode){ ExitThread(ExitCode);};
	static void Cleanup(unsigned long Handle){ CloseHandle((HANDLE) Handle); };

};
#elif defined(LINUX)
{
public:
	static HANDLE MakeThread(unsigned long * ThreadID, void * (* StartAddress)(void *) , void * arg, int *error){
		* error = pthread_create(ThreadID, NULL, StartAddress, arg);
		return (HANDLE)(* ThreadID);
	};
	static long WaitFor(unsigned long Handle){
		long Result;
		void * P;
		P = &Result;
		pthread_join(Handle, &P);
		return Result;
	};
	static void ExitFromThread(unsigned long ExitCode){ pthread_exit((void *) ExitCode);};
	static void Cleanup(unsigned long Handle){ pthread_detach(Handle); };

};
#else
#error	"Please fix os ver"
#endif

class TThread;

class Runnable
{
public:
	Runnable(){
		FThd = NULL;
		isStop = false; 
	}
	void SetThread(TThread * thd){
		FThd = thd;
	}

	virtual unsigned long execute() = 0;
	void stop();
protected:
	bool isStop;	
private:
	TThread * FThd;
};

class TThread
{
private:
	static TThreadAPIBinder FAPIBinder;
	unsigned long FThreadID;
	bool FSuccessMake;
	HANDLE FHandle;
	//TThreadListener * FListener;
	Runnable * FRunnable;
	static void * Wrapper(void * arg);
	int FReturnValue;
public:
	TThread(Runnable * runnable);    
	~TThread();
	void SetReturnValue(int value){
		FReturnValue = value;
	}
	bool Run();
	void Stop();
	void WaitFor();
	int GetThreadID();
	HANDLE GetHandle();
	int getReturnValue(){return FReturnValue;};
};

#endif

