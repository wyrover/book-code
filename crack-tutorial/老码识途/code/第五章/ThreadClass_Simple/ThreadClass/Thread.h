#ifndef XXX_Thread_H
#define XXX_Thread_H

typedef void * HANDLE;


class TThread{ 
private:
	unsigned long FThreadID;
	HANDLE FHandle;	
	static unsigned long __stdcall Wrapper(void * arg);
protected:
	int FReturnValue;    
	virtual void Execute() = 0;
public:    
	TThread(); 
	~TThread();    
	bool Run();    
	void WaitFor();    
	int GetThreadID();
	HANDLE GetHandle();
	int getReturnValue(){return FReturnValue;};
};

#endif