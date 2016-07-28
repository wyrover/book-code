/* classbind.cpp */	
#include <boost/bind.hpp>
#include <iostream>
#include <string>

class TheClass
{
public:
	void identity(std::string name, int age, float height)
	{
		std::cout << "Name   : " << name << std::endl;
		std::cout << "Age    : " << age << " years old" << std::endl;
		std::cout << "Height : " << height << " inch" << std::endl;
	}
};

int main(void)
{
	TheClass cls;
	boost::bind(&TheClass::identity, &cls, "John", 25, 68.89f)();
	return 0;
}