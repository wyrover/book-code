#include <iostream>
#include <algorithm>
#include <vector>
#include "../Common/Person.h"

int main()
{
	auto people = { Person("Wally"), Person("Wilma"), Person("Wenda"),
		Person("Odlaw"), Person("Waldo"), Person("Woof") };
	auto iter = std::find_if(begin(people), end(people),
		[](const Person& p) { return p.GetFirstName() == "Waldo"; });
	std::cout << iter->GetFirstName() << std::endl;
}
