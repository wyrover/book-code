#include <iostream>
#include <random>
#include <ctime>

int main()
{
	std::random_device seeder;
	const auto seed = seeder.entropy() ? seeder() : std::time(nullptr);
	std::default_random_engine generator(
		static_cast<std::default_random_engine::result_type>(seed));

	std::uniform_int_distribution<int> distribution(1, 6);
	int dice_roll = distribution(generator);   // 1 <= dice_roll <= 6
	std::cout << dice_roll << std::endl;
}
