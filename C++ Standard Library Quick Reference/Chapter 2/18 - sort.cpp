#include <iostream>
#include <functional>
#include <algorithm>

int main()
{
	int array[] = { 7, 9, 7, 2, 0, 4 };
	std::sort(std::begin(array), std::end(array), std::greater<int>());

	std::cout << std::endl;
}
