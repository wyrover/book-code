#include <boost/asio/steady_timer.hpp>
#include <iostream>

using namespace boost;

int main()
{
	asio::io_service ios;

	asio::steady_timer t1(ios);
	t1.expires_from_now(std::chrono::seconds(2));

	asio::steady_timer t2(ios);
	t2.expires_from_now(std::chrono::seconds(5));

	t1.async_wait([&t2](boost::system::error_code ec) {
		if (ec == 0) {
			std::cout << "Timer #2 has expired!" << std::endl;
		}
		else if (ec == asio::error::operation_aborted) {
			std::cout << "Timer #2 has been cancelled!" << std::endl;
		}
		else {
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message() << std::endl;
		}

		t2.cancel();
	});

	t2.async_wait([](boost::system::error_code ec) {
		if (ec == 0) {
			std::cout << "Timer #2 has expired!" << std::endl;
		}
		else if (ec == asio::error::operation_aborted) {
			std::cout << "Timer #2 has been cancelled!" << std::endl;
		}
		else {
			std::cout << "Error occured! Error code = "
				<< ec.value()
				<< ". Message: " << ec.message() << std::endl;
		}
	});

	ios.run();

	return 0;
}