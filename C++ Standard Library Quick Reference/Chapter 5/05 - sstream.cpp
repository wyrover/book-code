#include <iostream>
#include <sstream>
#include <string>

int main()
{
	std::ostringstream oss;
	oss << 123 << " " << 3.1415;
	std::string myString = oss.str();
	std::cout << "ostringstream contains: '" << myString << "'" << std::endl;

	std::istringstream iss(myString);
	int myInt; double myDouble;
	iss >> myInt >> myDouble;
	std::cout << "int = " << myInt << ", double = " << myDouble << std::endl;


	std::cout << std::endl;
}
