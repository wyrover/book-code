#include <boost/thread.hpp>
#include <iostream>

void foo()
{
	std::cout << "#" << boost::this_thread::get_id() << ": foo" << std::endl;
}

void bar(int a, int b)
{
	std::cout << "#" << boost::this_thread::get_id() << ": bar = " << a + b << std::endl;
}

int main()
{
	boost::thread thr1(foo);
	thr1.join();

	boost::thread thr2(bar, 1, 2);
	thr2.join();

	std::cout << "#" << boost::this_thread::get_id() << ": main thread" << std::endl;
	return 0;
}

