/* poolwork.cpp */
#include <boost/asio.hpp>
#include <iostream>

int main(void)
{
	boost::asio::io_service io_svc;
	boost::asio::io_service::work work(io_svc);

	for(int i=0; i<5; i++)
	{
		io_svc.poll();
		std::cout << "Line: " << i << std::endl;
	}

	return 0;
}