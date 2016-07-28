#include <boost/asio.hpp>
#include <iostream>

using namespace boost;

int main()
{
	try {
		asio::io_service ios;

		// Create and open a TCP socket.
		asio::ip::tcp::socket sock(ios, asio::ip::tcp::v4());

		// Create an object representing receive buffer
		// size option.
		asio::socket_base::receive_buffer_size cur_buf_size;

		// Get the currently set value of the option. 
		sock.get_option(cur_buf_size);

		std::cout << "Current receive buffer size is "
			<< cur_buf_size.value() << " bytes."
			<< std::endl;

		// Create an object representing receive buffer
		// size option with new value.
		asio::socket_base::receive_buffer_size
			new_buf_size(cur_buf_size.value() * 2);

		// Set new value of the option.
		sock.set_option(new_buf_size);

		std::cout << "New receive buferr size is "
			<< new_buf_size.value() << " bytes."
			<< std::endl;
	}
	catch (system::system_error &e) {
		std::cout << "Error occured! Error code = " << e.code()
			<< ". Message: " << e.what();

		return e.code().value();
	}

	return 0;
}
