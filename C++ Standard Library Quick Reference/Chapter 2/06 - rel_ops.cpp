#include <iostream>
#include <utility>
#include "../Common/Person.h"

int main()
{
	// Works even though only operator< is defined for our Person class:
	using namespace std::rel_ops;
	const bool comparison = (Person("Alexander") > Person("Bob"));
	std::cout << comparison;  // 0 (Alexander is not greater)

	std::cout << std::endl;
}
