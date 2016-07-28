#include <iostream>
#include <functional>
#include <vector>

int main()
{
	int i = 234;
	std::vector<std::reference_wrapper<int>> v{ std::ref(i) };  // cf. Chapter 3
	v[0].get() = 432;    // Occasionally, like here, an explicit get() is needed
						 // (v[0] returns reference_wrapper<int>&, not int&).
	std::cout << v[0] << "==" << i << std::endl;                // 432==432

	std::cout << std::endl;
}
