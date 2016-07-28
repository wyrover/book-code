#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	// Step 1. An instance of 'io_service' class is required by
	// socket constructor. 
	asio::io_service ios;

	// Step 2. Creating an object of 'tcp' class representing
	// a TCP protocol with IPv6 as underlying protocol.
	asio::ip::tcp protocol = asio::ip::tcp::v6();

	// Step 3. Instantiating an acceptor socket object.
	asio::ip::tcp::acceptor acceptor(ios);

	// Used to store information about error that happens
	// while opening the acceptor socket.
	boost::system::error_code ec;

	// Step 4. Opening the acceptor socket.
	acceptor.open(protocol, ec);

	if (ec.value() != 0) {
		// Failed to open the socket.
		std::cout
			<< "Failed to open the acceptor socket!"
			<< "Error code = "
			<< ec.value() << ". Message: " << ec.message();
		return ec.value();
	}

	return 0;
}
