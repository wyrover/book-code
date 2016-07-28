#include <iostream>
#include <system_error>

int main()
{
	try {
		throw std::system_error(std::make_error_code(std::errc::invalid_argument),
			"Now what am I to do with that argument?");  // optional what() message
	} catch (const std::exception& caughtException) {
		std::cout << caughtException.what();
	}


	std::cout << std::endl;
}
