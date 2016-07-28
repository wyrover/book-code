#include <iostream>
#include <algorithm>
#include <numeric>
#include <iterator>
#include <vector>
#include <deque>
#include <string>

int main()
{
	std::vector<std::string> v{ "1","2","3" };

	std::deque<std::string> reversed;
	std::copy(cbegin(v), cend(v), std::front_inserter(reversed));

	std::string s123 = std::accumulate(std::make_move_iterator(begin(v)),
		std::make_move_iterator(end(v)),
		std::string());
	std::cout << s123 << '\n';                            // 123


	std::cout << std::endl;
}
