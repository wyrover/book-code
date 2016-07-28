#include <iostream>
#include <cassert>

void foo(const char* msg)
{
	assert(msg != nullptr);	 // or: assert(msg);
}

int main()
{
	foo("Test");  // OK
	foo(nullptr); // Triggers the assertion.


	std::cout << std::endl;
}
