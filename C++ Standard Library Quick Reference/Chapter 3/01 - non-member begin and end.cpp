#include <iostream>
#include <iterator>

int main()
{
	int myArray[] = { 1,2,3,4 };
	auto beginIter = std::cbegin(myArray);
	auto endIter = std::cend(myArray);
	for (auto iter = beginIter; iter != endIter; ++iter) {
		std::cout << *iter << std::endl;
	}

	std::cout << std::endl;
}
