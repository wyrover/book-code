#define _CRT_SECURE_NO_WARNINGS		// To make this example work in Visual C++.
#include <iostream>
#include <cerrno>
#include <cmath>
#include <string>

int main()
{
	errno = 0;                         // Reset errno to 0!
	std::exp(100000);                  // Causes an overflow error.
	// Convert the errno error code to an error_code instance.
	std::error_code errorCode(errno, std::generic_category());
	std::error_condition okCondition;  // Default constructor creates
									   // a no-error condition.
	if (errorCode != okCondition)      // Check for an error.
		std::cerr << "Error: " << errorCode.message() << std::endl;


	std::cerr << "Error: " << std::strerror(errno) << std::endl;
	std::perror("Error");     // Prefix string is non-optional 


	std::cout << std::endl;
}
