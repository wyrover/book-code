#include <iostream>
#include <memory>
#include "../Common/Person.h"

void DoSomethingWith(Person& person)
{
	/* ... */
}

int main()
{
	{
		std::unique_ptr<Person> jeff(new Person("Jeffrey"));
		if (jeff != nullptr)
			jeff->SetLastName("Griffin");
		if (jeff)
			DoSomethingWith(*jeff);		// Dereference as Person&
	}									// jeff is deleted, even if DoSomethingWith() throws


	{
		auto jeff = std::make_unique<Person>("Jeffrey");
		if (jeff != nullptr)
			jeff->SetLastName("Griffin");
		if (jeff)
			DoSomethingWith(*jeff);		// Dereference as Person&
	}									// jeff is deleted, even if DoSomethingWith() throws


	std::cout << std::endl;
}
