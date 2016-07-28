#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <iostream>

void func(int i)
{
	std::cout << "func called, i = " << i << "/"
		<< boost::this_thread::get_id() << std::endl;
}

void worker_thread(boost::asio::io_service *svc)
{
	svc->run();
}

int main()
{
	boost::asio::io_service svc;
	boost::asio::io_service::strand strand_one(svc), strand_two(svc);
	
	for (int i = 0; i < 5; ++i)
	{
		svc.post(strand_one.wrap(boost::bind(func, i)));
	}

	for (int i = 5; i< 10; ++i)
	{
		svc.post(strand_two.wrap(boost::bind(func, i)));
	}

	boost::thread_group threads;
	for (int i = 0; i < 3; ++i)
	{
		threads.create_thread(boost::bind(worker_thread, &svc));
	}

	// wait for all threads be created
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	threads.join_all();

	return 0;
}

