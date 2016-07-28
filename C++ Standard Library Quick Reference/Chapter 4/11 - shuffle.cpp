#include <iostream>
#include <algorithm>
#include <vector>
#include <random>
#include <ctime>

int main()
{
	std::random_device seeder;
	const auto seed = seeder.entropy() ? seeder() : std::time(nullptr);
	std::default_random_engine gen(
		static_cast<std::default_random_engine::result_type>(seed));
	std::vector<int> vec{ 1,2,3,4,5,6 };
	std::shuffle(begin(vec), end(vec), gen);   // Possible result: 5 1 4 2 6 3

	std::cout << std::endl;
}
