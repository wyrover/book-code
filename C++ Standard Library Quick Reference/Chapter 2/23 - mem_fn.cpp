#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <functional>

int main()
{
	std::vector<std::string> v{ "Test", "", "123", "", "" };
	std::cout << std::count_if(begin(v), end(v), std::mem_fn(&std::string::empty)); // 3

	std::cout << std::endl;
}
