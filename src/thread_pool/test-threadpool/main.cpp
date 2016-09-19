#include "thread_pool.h"
#include <iostream>

struct test_inc
{
	void do_job(std::string str)
	{
		CHAR msg[100] = {0};
		sprintf(msg, "%d - %s", ::GetCurrentThreadId(), str.c_str());
		std::cout << msg << std::endl;
		::OutputDebugStringA(msg);

		::Sleep(100);
	}
};

int main()
{
	{
		thread_pool tp(10);
		test_inc t;

		for (int i = 1; i <= 1000; ++i) {
			char data[256] = {0};
			
			std::string message = "test";
			message.append(itoa(i, data, 10));
			tp.enqueue(boost::bind(&test_inc::do_job, &t, message));			
		}
	}

	::Sleep(10000);
    
    return 0;
}