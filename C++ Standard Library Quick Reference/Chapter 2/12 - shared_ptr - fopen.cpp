#include <iostream>
#include <memory>
#include <cstdio>

void DoSomethingWith(FILE* file)
{
	/* ... */
}

int main()
{
	{
		std::shared_ptr<FILE> smartFilePtr(fopen("test.txt", "r"), fclose);
		DoSomethingWith(smartFilePtr.get());
	}              // The FILE* is closed, even if DoSomethingWith() throws

	std::cout << std::endl;
}
