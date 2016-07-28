#include <iostream>
#include <exception>
#include <thread>

int main()
{
	std::exception_ptr threadException;
	std::thread t([&threadException] {					// Needs <thread>
		try {
			throw std::invalid_argument("Test");		// In worker thread
		}
		catch (...) {
			threadException = std::current_exception();	// Store exception
		}
	});

	t.join();				// Wait for thread to finish.

	if (threadException) {	// In main thread: handle exception if there is one.
		try {
			std::rethrow_exception(threadException);
		}
		catch (const std::exception& caughtException) {
			std::cout << "Caught from thread: " << caughtException.what() << '\n';
		}
	}


	std::cout << std::endl;
}
