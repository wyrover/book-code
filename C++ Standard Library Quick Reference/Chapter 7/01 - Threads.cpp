#include <iostream>
#include <thread>
#include <string>

void function(const std::string& str, int i)
{
	/* ... */
}

int main()
{
	int anotherArg = 42;
	std::thread worker1(function, "arg", anotherArg);
	std::thread worker2([=] { function("arg", anotherArg); });

	worker1.join();	// See text.
	worker2.join();	// See text.

	std::cout << std::endl;
}
