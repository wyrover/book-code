// ThreadClass.cpp : 定义控制台应用程序的入口点。
//

#include "thread.h"
#include <stdio.h>
#include <windows.h>

class TPrintThread : public TThread{
protected:
	void Execute() {
		printf("hello I am in a thread\n");
		FReturnValue = 1;
	};
public:
	TPrintThread(): TThread()
	{

	};
};



void main()
{
	TPrintThread * Thd;
	Thd = new TPrintThread();	
	Thd->Run();
	delete Thd;//we don't use waitfor, because we wait it in destructor.
}

