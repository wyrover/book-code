#include <iostream>
#include <valarray>

int main()
{
	std::valarray<int> ints = { 0,1,2,3,4,5,6,7 };
	std::slice mySlicer(2, 3, 2);
	const std::valarray<int>& constInts = ints;
	auto copies = constInts[mySlicer];     // valarray<int> with copies of 2,4,6
	auto refs = ints[mySlicer];     // slice_array<int> with references to 2,4,6
	std::valarray<int> factors{ 6,3,2 };
	refs *= factors;                       // ints will be 0,1,12,3,12,5,12,7 

	std::cout << std::endl;
}
