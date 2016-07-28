#include <iostream>
#include <fstream>
#include <string>

int main()
{
	const std::string filename = "output.txt";
	std::ofstream ofs(filename);
	ofs << 123 << " " << 3.1415;
	ofs.close();

	std::ifstream ifs(filename);
	int myInt; double myDouble;
	ifs >> myInt >> myDouble;
	std::cout << "int = " << myInt << ", double = " << myDouble << std::endl;


	std::cout << std::endl;
}
