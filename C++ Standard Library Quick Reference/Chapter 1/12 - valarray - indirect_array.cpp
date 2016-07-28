#include <iostream>
#include <valarray>

int main()
{
	std::valarray<int> ints = { 0,1,2,3,4 };
	std::valarray<size_t> indices = { 1,3,4 };
	ints[indices] = -1;                        // 0,-1,2,-1,-1

	std::cout << std::endl;
}
