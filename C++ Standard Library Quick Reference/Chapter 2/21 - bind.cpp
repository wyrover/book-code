#include <iostream>
#include <functional>
#include <vector>
#include <algorithm>
#include <string>

bool my_less(int x, int y) { return x < y; }
void f(std::string& x, char y) { x += y; }

int main()
{
	using namespace std::placeholders;						// contains _1, _2, _3, ... 
	auto my_greater = std::bind(my_less, _2, _1);			// function + swap _1 _2
	auto twice = std::bind(std::plus<int>{}, _1, _1);		// functor + twice _1
	auto plus5 = std::bind(std::plus<int>{}, _1, 5);		// functor + fixed 5
	std::cout << my_greater(twice(13), plus5(20)) << '\n';	// 1 (true)

	// bind() expressions may be nested whilst sharing placeholders:
	auto g = std::bind(my_greater, std::bind(twice, _1), std::bind(plus5, _1));
	std::cout << g(10) << ' ' << g(4) << '\n';				// 1 0 (true false)

	// Use std::ref()/cref() to pass references
	// (For containers, algorithms, and strings see chapters 3, 4, and 6)
	std::vector<char> v{ 'c', 'o', 'n', 'c', 'a', 't' };
	std::string concat;
	std::for_each(begin(v), end(v), std::bind(f, std::ref(concat), _1));
	std::cout << concat << std::endl;

	std::cout << std::endl;
}
