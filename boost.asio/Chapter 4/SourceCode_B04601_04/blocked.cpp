/* blocked.cpp */
#include <boost/asio.hpp>
#include <iostream>

int main(void)
{
	boost::asio::io_service io_svc;
	boost::asio::io_service::work worker(io_svc);

	io_svc.run();

	std::cout << "We will not see this line in console window :(" << std::endl;

	return 0;
}