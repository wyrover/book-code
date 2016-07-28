#include <iostream>
#include <random>
#include <functional>

int main()
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 6);
	std::function<int()> roller = std::bind(distribution, generator);
	for (int i = 0; i < 100; ++i)
	{
		std::cout << roller() << '\n';
	}

	std::cout << std::endl;
}
