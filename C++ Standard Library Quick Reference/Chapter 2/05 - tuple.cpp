#include <iostream>
#include <string>
#include <tuple>

int main()
{
	auto t = std::make_tuple(1, 2, 0.3, std::string("4"));
	std::cout << std::get<0>(t) << '\n';         // get using 0-based index
	std::get<2>(t) = 3.0;       // no set required: get returns a reference
	std::cout << std::get<double>(t) << '\n';    // get using unique type
	// std::cout << std::get<int>(t) << '\n';   --> ambiguous: compiler error!
	std::string s = std::get<3>(std::move(t));   // move a value out of a tuple


	int one, two;
	double three;
	std::tie(one, two, three, std::ignore) = t;


	std::cout << std::tuple_size<decltype(t)>::value << '\n';				// 4
	std::tuple_element<0, decltype(t)>::type elementOne = std::get<0>(t);	// int
	std::cout << elementOne << std::endl;


	std::cout << std::endl;
}
