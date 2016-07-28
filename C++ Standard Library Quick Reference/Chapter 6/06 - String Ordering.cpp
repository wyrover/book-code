#include <iostream>
#include <locale>
#include <vector>
#include <string>
#include <algorithm>

int main()
{
	std::vector<std::string> strings = { "liberté", "égalité", "fraternité" };
	auto printSortedMotto = [&strings] {
		for (auto& s : strings) std::cout << s << ' ';
		std::cout << std::endl;
	};
	std::sort(begin(strings), end(strings));       // Lexicographic sort
	printSortedMotto();                            // fraternité liberté égalité 
	std::sort(begin(strings), end(strings), std::locale("C"));
	printSortedMotto();                            // fraternité liberté égalité 
	std::sort(begin(strings), end(strings), std::locale("french"));
	printSortedMotto();                            // égalité fraternité liberté


	std::cout << std::endl;
}
