#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

void processRequest(asio::ip::tcp::socket& sock) {
	// We use extensibel buffer because we don't
	// know the size of the request message.
	asio::streambuf request_buf;

	system::error_code ec;

	// Receiving the request.
	asio::read(sock, request_buf, ec);

	if (ec != asio::error::eof)
		throw system::system_error(ec);

	// Request received. Sending response.
	// Allocating and filling the buffer with
	// binary data.
	const char response_buf[] = { 0x48, 0x69, 0x21 };

	// Sending the request data.
	asio::write(sock, asio::buffer(response_buf));

	// Shutting down the socket to let the
	// client know that we've sent the whole
	// response.
	sock.shutdown(asio::socket_base::shutdown_send);
}

int main()
{
	unsigned short port_num = 3333;
	
	try {
		asio::ip::tcp::endpoint ep(asio::ip::address_v4::any(),
			port_num);

		asio::io_service ios;

		asio::ip::tcp::acceptor acceptor(ios, ep);

		asio::ip::tcp::socket sock(ios);

		acceptor.accept(sock);

		processRequest(sock);
	}
	catch (system::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}
