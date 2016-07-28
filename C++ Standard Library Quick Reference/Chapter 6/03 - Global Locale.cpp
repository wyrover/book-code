#include <iostream>
#include <locale>

int main()
{
	std::locale current_locale;
	std::cout << '"' << current_locale.name() << '"' << '\n';  // "C"
	std::cout << 100000 << '\n';			// 100000
	std::locale::global(std::locale(""));	// Global locale --> user preferences
	std::cout << 100000 << '\n';			// 100000
	std::cout.imbue(std::locale());			// Imbue the current global locale
	std::cout << 100000 << '\n';			// Some possible outputs (locale dependent):
											// 100,000; 100 000; 100.000; 1,00,000; ...


	std::cout << std::endl;
}
