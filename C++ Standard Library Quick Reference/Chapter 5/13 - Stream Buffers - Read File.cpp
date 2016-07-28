#include <iostream>
#include <fstream>
#include <sstream>

int main()
{
	std::ifstream ifs("test.txt");
	std::stringstream buffer;
	buffer << ifs.rdbuf();


	std::cout << std::endl;
}
