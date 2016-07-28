#include <iostream>
#include <locale>
#include <clocale>

int main()
{
	// Use the user's preferred locale settings, 
	// but with neutral numeric and monetary formatting
	std::locale::global(std::locale(std::locale(""), "C",
		std::locale::numeric | std::locale::monetary));
	std::setlocale(LC_ALL, "");
	std::setlocale(LC_NUMERIC, "C");
	std::setlocale(LC_MONETARY, "C");


	std::cout << std::endl;
}
