#include <iostream>
#include <initializer_list>

class ExampleClass {
public:
	ExampleClass(int, int) { std::cout << "(int, int) ctor" << '\n'; }
	ExampleClass(std::initializer_list<int>) { std::cout << "initializer_list<int> ctor" << '\n'; }
};

int main()
{
	ExampleClass a(1, 2);	// (int, int) constructor is used
	ExampleClass b{ 1, 2 };	// initializer_list<int> constructor is used


	std::cout << std::endl;
}
