// ThreadClass.cpp : 定义控制台应用程序的入口点。
//

#include "Thread.h"
#include <stdio.h>
#include <windows.h>

class TPrintRunnable : public Runnable{
protected:
	virtual unsigned long execute(){
		Sleep(5000);
		printf("hello I am in a thread\n");		
		return 1;
	};	
};



void main()
{
	TThread * Thd;
	Thd = new TThread(new TPrintRunnable());
	printf("please input\n");
	Thd->Run();
	getchar();
	printf("I am in main thread\n");
	getchar();
	delete Thd;//we don't use waitfor, because we wait it in destructor.
}

