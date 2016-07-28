/* uncalledbind.cpp */
#include <boost/bind.hpp>
#include <iostream>

void func()
{
	std::cout << "Binding Function" << std::endl;
}

int main(void)
{
	boost::bind(&func);
	return 0;
}