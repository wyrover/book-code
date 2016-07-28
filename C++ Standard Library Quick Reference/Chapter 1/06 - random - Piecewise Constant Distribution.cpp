#include <iostream>
#include <vector>
#include <random>

int main()
{
	std::mt19937 generator;		  // Default-seeded for this example
	std::vector<double> intervals = { 1,20,40,60,80 };
	std::vector<double> weights = { 1,3,1,3 };
	std::piecewise_constant_distribution<double> distribution(
		begin(intervals), end(intervals), begin(weights));
	int value = static_cast<int>(distribution(generator));
	std::cout << value << std::endl;
}
