/* unblocked.cpp */
#include <boost/asio.hpp>
#include <iostream>

int main(void)
{
	boost::asio::io_service io_svc;

	io_svc.run();

	std::cout << "We will see this line in console window." << std::endl;

	return 0;
}