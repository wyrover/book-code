#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> vec{ 11,22,33 };
	const int valueToAdd = 18;
	auto lower = std::lower_bound(cbegin(vec), cend(vec), valueToAdd);
	vec.insert(lower, valueToAdd);    // 11,18,22,33

	std::cout << std::endl;
}
