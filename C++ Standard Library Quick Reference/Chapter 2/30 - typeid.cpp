#include <iostream>
#include <typeinfo>

int main()
{
	std::string s;
	std::cout << typeid(s).name() << '\n';
	std::cout << (typeid(typeid(s).name()) == typeid(s.data())); // 1 (true)

	std::cout << std::endl;
}
