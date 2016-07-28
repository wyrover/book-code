#include <iostream>
#include <utility>
#include <tuple>
#include "../Common/Person.h"

int main()
{
	std::pair<unsigned int, Person> p1(42u, Person("Douglas", "Adams"));


	auto p2 = std::make_pair(42u, Person("Douglas", "Adams"));


	std::pair<unsigned, Person> p3(std::piecewise_construct,
		std::make_tuple(42u), std::forward_as_tuple("Douglas", "Adams"));


	std::cout << std::endl;
}
