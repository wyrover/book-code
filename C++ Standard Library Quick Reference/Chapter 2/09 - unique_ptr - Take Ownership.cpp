#include <iostream>
#include <memory>
#include "../Common/Person.h"

void TakeOwnership(Person* person)
{
	/* Do something with person and delete it! */
}

int main()
{
	auto niles = std::make_unique<Person>("Niles", "Crane");
	niles.reset(new Person("Niles", "Butler"));        // Niles Crane is deleted
	TakeOwnership(niles.release());  // TakeOwnership() must delete Niles Butler

	std::cout << std::endl;
}
