#include <iostream>
#include <valarray>

int main()
{
	std::valarray<int> ints = { 0,1,2,3,4,5,6,7,8,9,10 };
	// Construct a valarray<bool> with true for all even elements in ints.
	std::valarray<bool> even = ((ints % 2) == 0);
	// Count the number of true values in even.
	int count = std::count(begin(even), end(even), true);
	// Construct a valarray with count elements of value 4.
	std::valarray<int> factors(4, count);
	// Multiply the even elements in ints with a factor of 4.
	ints[even] *= factors;                        // 0,1,8,3,16,5,24,7,32,9,40

	std::cout << std::endl;
}
