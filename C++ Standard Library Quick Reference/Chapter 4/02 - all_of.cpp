#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> vec{ 1,2,3,4,5,6 };
	bool allOf = std::all_of(cbegin(vec), cend(vec),
		[](auto& element) { return element % 2 == 0; });  // false
	std::cout << allOf << std::endl;
}
