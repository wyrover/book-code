#include <iostream>
#include <unordered_set>
#include "../Common/Person.h"

namespace std {
	template<> struct hash<Person> {
		// Two nested types required by the Standard for specializations, but
		// not specified to be part of the (unspecified) hash<Key> template:
		typedef Person argument_type;
		typedef std::size_t result_type;

		result_type operator()(const argument_type& p) const {
			auto firstNameHash(std::hash<std::string>()(p.GetFirstName()));
			auto lastNameHash(std::hash<std::string>()(p.GetLastName()));
			return firstNameHash ^ lastNameHash;
		}
	};
}

int main()
{
	std::unordered_set<Person> persons;
	persons.insert(Person("John", "Doe"));

	std::cout << std::endl;
}
