#include <iostream>
#include <atomic>
#include "../Common/Person.h"

int main()
{
	{
		std::atomic_int atom;	    // Uninitialized!
		atom = 123;
		std::cout << atom << std::endl;        // 123


		atom.store(123);
		std::cout << atom.load() << std::endl; // 123
	}


	{
		std::atomic<Person*> atomic_person(nullptr);
		// ... (share references to atomic_person with other threads)
		auto person = new Person();
		person->SetFirstName("Jay");
		person->SetLastName("Pritchett");
		atomic_person = person;        // atomic store + release fence!

		// ...

		auto p = atomic_person.exchange(nullptr);
		delete p;
	}


	std::cout << std::endl;
}
