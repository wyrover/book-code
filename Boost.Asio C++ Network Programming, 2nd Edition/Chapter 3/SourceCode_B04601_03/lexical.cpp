/* lexical.cpp */
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

int main(void)
{
	try 
	{
		std::string str;
		std::cout << "Please input first number: ";
		std::cin >> str;
		int n1 = boost::lexical_cast<int>(str);
		std::cout << "Please input second number: ";
		std::cin >> str;
		int n2 = boost::lexical_cast<int>(str);
		std::cout << "The sum of the two numbers is ";
		std::cout << n1 + n2 << "\n";
		return 0;
	}
	catch (const boost::bad_lexical_cast &e)
	{
		std::cerr << e.what() << "\n";
		return 1;
	}
}