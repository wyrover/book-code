#include <iostream>
#include <string>

void f(std::string s)
{
	std::cout << "Moved or copied: " << s << '\n';
}

void g(std::string&& s)
{
	std::cout << "Moved " << s << '\n';
}

std::string h()
{
	std::string s("test");
	return s;	// moved implicitly, same as std::move(s)
}

int main()
{
	std::string test("123");
	f(test);               // test is copied to a new string
	f(std::move(test));    // test is moved to a new string (move constructor)
	// std::cout << test; --> Undefined: may give "" or "123", or simply crash
	test = "456";          // test is reinitialized, and may be used again
	// g(test);           --> Does not compile
	g(std::move(test));
	g(std::string("789")); // Unnamed objects are moved implicitly
	g(h());

	std::cout << std::endl;
}
