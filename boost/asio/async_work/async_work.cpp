#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <iostream>

void func(int i)
{
	std::cout << "func called, i = " << i << std::endl;
}

void worker_thread(boost::asio::io_service *svc)
{
	svc->run();
}

int main()
{
	boost::asio::io_service svc;

	for (int i = 0; i < 10; i++)
	{
		svc.post(boost::bind(func, i));
	}

	boost::thread_group threads;
	for (int i = 0; i < 3; i++)
	{
		threads.create_thread(boost::bind(worker_thread, &svc));
	}

	// wait for all threads to be created
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	threads.join_all();

	return 0;
}

