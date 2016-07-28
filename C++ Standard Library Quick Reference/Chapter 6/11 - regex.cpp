#include <iostream>
#include <regex>
#include <string>

int main()
{
	std::regex pattern(R"(<(.+)>(.*?)</\1>)");
	std::string target = "<b>Bold</b>, not bold, <b>bold again</b>.";

	std::cout << std::boolalpha;		  // print true/false instead of 1/0
	std::cout << std::regex_match(target, pattern) << "\n\n";           // false

	{
		std::smatch results;
		auto begin = target.cbegin(), end = target.cend();
		while (std::regex_search(begin, end, results, pattern)) {
			std::cout << results.str(2) << '\n';      // "Bold", then "bold again"
			begin += results.length();
		}
	}


	{
		std::sregex_iterator begin(target.cbegin(), target.cend(), pattern),
							 end;    // default constructor creates end-iterator
		std::for_each(begin, end, [](auto& results) {    /* const std::smatch& */
			std::cout << results.str(2) << '\n';
		});
	}


	{
		std::sregex_token_iterator beg(target.cbegin(), target.cend(), pattern, 2),
			end;  // default constructor creates end-iterator
		std::for_each(beg, end, [](auto& subMatch) {  /* const std::ssub_match& */
			std::cout << subMatch << '\n';
		});
	}


	std::cout << std::endl;
}
