#include <iostream>
#include <mutex>
#include <string>

void initialise(const std::string& str)
{
	/* ... */
}

int main()
{
	std::once_flag flag;
	// ...
	std::call_once(flag, initialise, "a string argument");


	std::cout << std::endl;
}
