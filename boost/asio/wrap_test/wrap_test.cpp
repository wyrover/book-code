#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>
#include <iostream>

void dispatched_func_1()
{
	std::cout << "dispatched 1" << std::endl;
}

void dispatched_func_2()
{
	std::cout << "dispatched 2" << std::endl;
}

void test(boost::asio::io_service *svc, boost::function<void()> func)
{
	std::cout << "test" << std::endl;
	svc->dispatch(dispatched_func_1);
	func();
}

void service_run(boost::asio::io_service *svc)
{
	svc->run();
}

int main()
{
	boost::asio::io_service svc;

	test(&svc, svc.wrap(dispatched_func_2));
	boost::thread th(boost::bind(service_run, &svc));
	boost::this_thread::sleep(boost::posix_time::millisec(500));
	th.join();

	return 0;
}

