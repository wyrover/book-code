#include <iostream>
#include <string>

int main()
{
	std::string one("Test 123");
	std::string other;
	other = std::move(one);
	// std::cout << one; --> Undefined behavior: one was moved to other

	std::cout << std::endl;
}
