#include <iostream>
#include <valarray>

int main()
{
	std::valarray<int> ints1(7);                // 7 zero-initialized integers
	std::valarray<double> doubles = { 1.1, 2.2, 3.3 };
	int carray[] = { 6,5,4,3,2,1 };
	std::valarray<int> ints2(carray, 3);                     // Contains 6,5,4

	std::cout << std::endl;
}
