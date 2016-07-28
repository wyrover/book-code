#include <iostream>
#include <map>
#include "../Common/Person.h"

int main()
{
	std::map<Person, int> myMap{ { Person("Jenne"), 1 },{ Person("Bart"), 2 } };


	auto iter = begin(myMap);	// type of iter is pair<Person, int>
	std::cout << "Key=" << iter->first.GetFirstName(); // Key=Bart (not Jenne)
	std::cout << ", Value=" << iter->second;           // , Value=2


	myMap[Person("Peter")] = 3;


	myMap.insert(std::make_pair(Person("Marc"), 4));


	myMap.emplace(Person("Anna"), 4);


	myMap.emplace(std::piecewise_construct,
		std::forward_as_tuple("Anna"), std::forward_as_tuple(4));


	std::cout << std::endl;
}
