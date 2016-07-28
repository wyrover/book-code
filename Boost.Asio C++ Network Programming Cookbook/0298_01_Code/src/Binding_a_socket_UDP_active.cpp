#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	// Step 1. Here we assume that the server application has
	// already obtained the protocol port number.
	unsigned short port_num = 3333;

	// Step 2. Creating an endpoint.
	asio::ip::udp::endpoint ep(asio::ip::address_v4::any(),
		port_num);

	// Used by 'socket' class constructor.
	asio::io_service ios;

	// Step 3. Creating and opening a socket.
	asio::ip::udp::socket sock(ios, ep.protocol());

	boost::system::error_code ec;

	// Step 4. Binding the socket to an endpoint.
	sock.bind(ep, ec);

	// Handling errors if any.
	if (ec != 0) {
		// Failed to bind the socket. Breaking execution.
		std::cout << "Failed to bind the socket."
			<< "Error code = " << ec.value() << ". Message: "
			<< ec.message();

		return ec.value();
	}

	return 0;
}

