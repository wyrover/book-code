#include <iostream>
#include <memory>
#include <cstdio>
#include <functional>

void DoSomethingWith(FILE* file)
{
	/* ... */
}

int main()
{
	{
		std::unique_ptr<FILE, std::function<void(FILE*)>> smartFilePtr(fopen("test.txt", "r"), fclose);
		DoSomethingWith(smartFilePtr.get());
	}              // The FILE* is closed, even if DoSomethingWith() throws

	std::cout << std::endl;
}
