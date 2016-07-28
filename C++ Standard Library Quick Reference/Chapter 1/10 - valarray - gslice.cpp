#include <iostream>
#include <valarray>

int main()
{
	std::valarray<int> a = { 0,11,22,33,44,55,66,77,88,99,111 };
	std::valarray<size_t> sizes{ 2, 3 };
	std::valarray<size_t> strides{ 5, 2 };
	std::valarray<int> r = a[std::gslice(1, sizes, strides)]; //11,33,55,66,88,111

	std::cout << std::endl;
}
