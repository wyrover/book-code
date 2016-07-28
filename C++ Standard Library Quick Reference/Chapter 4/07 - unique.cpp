#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> v{ 3,4,4,4,5,6,4,5,5,7 };
	auto result = std::unique(begin(v), end(v));	// possible outcome: 3,4,5,6,4,5,7,5,5,7
	v.erase(result, end(v));						// final outcome:    3,4,5,6,4,5,7

	std::cout << std::endl;
}
