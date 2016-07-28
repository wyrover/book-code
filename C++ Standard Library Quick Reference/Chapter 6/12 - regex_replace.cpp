#include <iostream>
#include <regex>
#include <string>

int main()
{
	std::regex vowels("[aeiou]");
	std::cout << std::regex_replace("devoweled", vowels, "*") << '\n';

	std::regex bolds("<b>(.*?)</b>");
	std::string target = "<b>debolded</b>";
	std::ostream_iterator<char> out(std::cout);
	std::regex_replace(out, target.cbegin(), target.cend(), bolds, "$1");


	std::cout << std::endl;
}
