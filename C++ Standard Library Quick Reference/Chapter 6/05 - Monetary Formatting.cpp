#include <iostream>
#include <locale>
#include <sstream>
#include <cassert>

int main()
{
	std::locale my_locale("en-US");   // For Windows; for Linux use form "en_US"
	std::stringstream stream;
	stream.imbue(my_locale);

	// Perform equivalent of ‘stream << std::put_money(valueIn);’ explicitly:
	long double valueIn = 123456;  // (Or: ‘std::string valueIn = "123456";’)
	auto& money_formatter = std::use_facet<std::money_put<char>>(my_locale);
	stream << std::showbase;
	auto target = std::ostreambuf_iterator<char>(stream);
	money_formatter.put(target, false, stream, ' ', valueIn);   // $1,234.56

	// Perform equivalent of ‘stream >> std::get_money(valueOut);’ explicitly
	long double valueOut;          // (Or: ‘std::string valueOut;’)
	auto& money_parser = std::use_facet<std::money_get<char>>(my_locale);
	std::ios_base::iostate error = std::ios_base::goodbit;
	auto b = std::istreambuf_iterator<char>(stream);
	auto e = std::istreambuf_iterator<char>();
	money_parser.get(b, e, false, stream, error, valueOut);     // 123456
	if (error != std::ios_base::goodbit)
		stream.setstate(error);

	assert(valueIn == valueOut);


	std::cout << std::endl;
}
