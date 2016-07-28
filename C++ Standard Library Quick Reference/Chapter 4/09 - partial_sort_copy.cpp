#include <iostream>
#include <algorithm>
#include <vector>

int main()
{
	std::vector<int> vec{ 9,2,4,7,3,6,1 };
	std::vector<int> threeSmallestElements(3);
	std::partial_sort_copy(begin(vec), end(vec),
		begin(threeSmallestElements), end(threeSmallestElements));

	std::cout << std::endl;
}
