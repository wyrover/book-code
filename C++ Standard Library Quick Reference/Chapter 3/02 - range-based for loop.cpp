#include <iostream>

int main()
{
	int myArray[] = { 1,2,3,4 };
	for (const auto& element : myArray) {
		std::cout << element << std::endl;
	}


	std::cout << std::endl;
}
