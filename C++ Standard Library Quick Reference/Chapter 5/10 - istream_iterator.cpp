#include <iostream>
#include <iterator>
#include <algorithm>

int main()
{
	std::istream_iterator<double> begin(std::cin), end;
	double sum = 0.0; int count = 0;
	std::for_each(begin, end, [&](double value) { sum += value; ++count;});
	std::cout << sum / count << std::endl;


	std::cout << std::endl;
}
