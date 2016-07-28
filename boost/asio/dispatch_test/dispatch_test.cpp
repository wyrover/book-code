#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <iostream>

void func(int i)
{
	std::cout << "func called, i = " << i << std::endl;
}

void run_dispatch_and_post(boost::asio::io_service *svc)
{
	for (int i = 0; i < 10; i += 2)
	{
		svc->dispatch(boost::bind(func, i));
		svc->post(boost::bind(func, i + 1));
	}
}

int main()
{
	boost::asio::io_service svc;
	svc.post(boost::bind(run_dispatch_and_post, &svc));
	svc.run();
	return 0;
}

