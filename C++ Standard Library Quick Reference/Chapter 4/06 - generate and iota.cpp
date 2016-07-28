#include <iostream>
#include <algorithm>
#include <numeric>
#include <vector>

int main()
{
	std::vector<int> vec(6);						// 0,0,0,0,0,0
	int value = 11;
	std::generate(begin(vec), begin(vec) + 3,
		[&value] { value *= 2; return value; });	// 22,44,88,0,0,0
	std::iota(begin(vec) + 3, end(vec), 2);			// 22,44,88,2,3,4

	std::cout << std::endl;
}
