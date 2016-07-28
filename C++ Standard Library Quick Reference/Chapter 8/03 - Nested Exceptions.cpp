#include <iostream>
#include <exception>

void execute_helper() {
	throw std::range_error("Out-of-range error in execute_helper()");
}

void execute() {
	try { execute_helper(); }
	catch (...) {
		std::throw_with_nested(std::runtime_error("Caught in execute()"));
	}
}

void print(const std::exception& exc) {
	std::cout << "Exception: " << exc.what() << std::endl;
	try { std::rethrow_if_nested(exc); }
	catch (const std::exception& e) {
		std::cout << "   Nested ";
		print(e);
	}
}

int main()
{
	try { execute(); }
	catch (const std::exception& e) { print(e); }


	std::cout << std::endl;
}
