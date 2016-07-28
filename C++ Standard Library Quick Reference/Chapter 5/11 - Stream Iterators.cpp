#include <iostream>
#include <iterator>
#include <sstream>

int main()
{
	std::ostringstream oss;
	std::istream_iterator<double> begin(std::cin), end;
	std::copy(begin, end, std::ostream_iterator<double>(oss, "\t"));
	std::cout << oss.str() << std::endl;


	std::cout << std::endl;
}
