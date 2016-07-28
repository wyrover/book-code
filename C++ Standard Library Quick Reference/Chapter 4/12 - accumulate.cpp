#include <iostream>
#include <numeric>
#include <vector>

int main()
{
	std::vector<int> vec{ 4,2,5,1,3,6 };
	int sum = std::accumulate(begin(vec), end(vec), 0);    // 21
	std::cout << sum << std::endl;
}
