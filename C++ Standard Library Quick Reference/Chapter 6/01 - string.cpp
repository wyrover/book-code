#include <iostream>
#include <string>
#include <utility>

int main()
{
	{
		std::string s = "Strings be fun";
		s.reserve(20);
		s.push_back('!');                                // "Strings be fun!"
		const auto found = std::find(cbegin(s), cend(s), 'b');
		s[found - s.cbegin()] = 'r';                     // "Strings re fun!"
		s.insert(found, 'a');                            // "Strings are fun!"
	}


	{
		std::string s = "Strings be fun";
		s.reserve(20);
		s.push_back('!');                                // "Strings be fun!"
		const size_t found = s.find('b');
		s[found] = 'r';
		s.insert(found, "a");     // (no index-based single-character insert exists)
	}


	{
		std::string s = "Strings be fun";
		s.reserve(20);
		s.push_back('!');                                // "Strings be fun!"
		const size_t found = s.find("be");
		s.replace(found, 2, "are");	      // 2 = number of characters to replace
	}


	{
		using namespace std::literals::string_literals;
		auto a = "a is a const char*";
		auto b = "b is a std::string"s;
		auto c = std::make_pair(3u, L"c is a pair<unsigned, wstring>"s); // <pair>
	}


	{
		std::string s(u8"字符串"); // UTF-8 encoding of Chinese word for "string"
		std::cout << s.length();   // Length: 9 code units!
	}


	std::cout << std::endl;
}
