#include <iostream>
#include <iterator>
#include <vector>

int main()
{
	std::vector<double> vec{ 1.11, 2.22, 3.33, 4.44 };
	std::copy(cbegin(vec), cend(vec),
		std::ostream_iterator<double>(std::cout, "\t"));

	std::cout << std::endl;
}
