#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> vec{ 1,2,3,4,5,6 };
	std::rotate(begin(vec), begin(vec) + 4, end(vec));

	std::cout << std::endl;
}
