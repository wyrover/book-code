#define _CRT_SECURE_NO_WARNINGS		// To make the example work with Visual C++.
#include <cstdio>
#include <string>

int main()
{
	std::string s = "int: +123; double: -2.34E-3; chars: abcdef";
	int i = 0; double d = 0.0; char chars[4] = { 0 };
	std::sscanf(s.data(), "int: %i; double: %lE; chars: %[abc]", &i, &d, chars);
	std::printf("int: %+i; double: %.2lE; chars: %s", i, d, chars);

	std::printf("\n");
}
