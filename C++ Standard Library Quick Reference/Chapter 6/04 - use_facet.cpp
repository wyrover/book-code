#include <iostream>
#include <locale>

int main()
{
	const auto& f = std::use_facet<std::numpunct<char>>(std::locale::classic());
	std::cout << f.decimal_point() << std::endl;   // Prints a dot character '.'


	std::cout << std::endl;
}
