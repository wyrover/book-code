#include <iostream>
#include <fstream>

int main()
{
	{
		std::ifstream ofs("non_existing_file.ext");
		std::cout << ofs.fail() << std::endl;        // 1
	}


	{
		std::ifstream ofs("non_existing_file.ext");
		try {
			ofs.exceptions(ofs.failbit);      // Enable exceptions for failures.
		}
		catch (const std::ios_base::failure& exception) {
			std::cout << exception.what() << std::endl;
		}
	}


	std::cout << std::endl;
}
