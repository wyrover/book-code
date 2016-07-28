#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> vec{ 9,2,4,7,3,6,1 };
	auto middle = begin(vec) + vec.size() / 2;
	std::nth_element(begin(vec), middle, end(vec));
	int median = *middle;    // 4
	std::cout << median << std::endl;
}
