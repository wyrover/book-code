#include <iostream>
#include <algorithm>
#include <vector>
#include <functional>

int main()
{
	std::vector<int> vec{ 1,2,3,4,5,6 };

	std::transform(cbegin(vec), cend(vec), begin(vec),
		[](auto& element) { return element * 2; });

	std::transform(cbegin(vec), cend(vec), begin(vec), std::negate<>());

	std::for_each(cbegin(vec), cend(vec),
		[](auto& element) { std::cout << element << " "; });


	std::cout << std::endl;
}
