#include <iostream>
#include <fstream>

int main()
{
	std::ofstream file("output.txt");
	auto oldCoutBuf = std::cout.rdbuf(file.rdbuf()); // Redirect cout to file
	std::cout << "Some output" << '\n';  // Write to file
	std::cout.rdbuf(oldCoutBuf);         // Restore the old cout buffer!


	std::cout << std::endl;
}
