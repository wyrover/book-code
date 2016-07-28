#include <iostream>
#include <random>

int main()
{
	std::default_random_engine generator;
	std::uniform_int_distribution<int> distribution(1, 6);
	int dice_roll = distribution(generator);   // 1 <= dice_roll <= 6
	std::cout << dice_roll << std::endl;
}
