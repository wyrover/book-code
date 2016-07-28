/* argumentbind.cpp */
#include <boost/bind.hpp>
#include <iostream>

void cubevolume(float f)
{
	std::cout << "Volume of the cube is " << f * f * f << std::endl;
}

int main(void)
{
	boost::bind(&cubevolume, 4.23f)();
	return 0;
}