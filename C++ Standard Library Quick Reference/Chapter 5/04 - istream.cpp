#include <iostream>
#include <string>

int main()
{
	int anInt;
	double aDouble;
	std::cout << "Enter an integer followed by some whitespace "
		<< "and a double, and press enter: ";
	std::cin >> anInt >> aDouble;
	std::cout << "You entered: ";
	std::cout << "Integer = " << anInt << ", Double = " << aDouble << std::endl;

	std::string message;
	std::cout << "Enter a string. End input with a * and enter: ";
	std::getline(std::cin >> std::ws, message, '*');
	std::cout << "You entered: '" << message << "'" << std::endl;


	std::cout << std::endl;
}
