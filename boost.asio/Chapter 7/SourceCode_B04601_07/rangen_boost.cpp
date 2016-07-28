/* rangen_boost.cpp */
#include <boost/random/mersenne_twister.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <iostream>

int main(void)
{
	int guessNumber;
	std::cout << "Select number among 0 to 10: ";
	std::cin >> guessNumber;
	if(guessNumber < 0 || guessNumber > 10)
	{
		return 1;
	}
	boost::random::mt19937 rng;
	boost::random::uniform_int_distribution<> ten(0,10);
	int randomNumber = ten(rng);
	
	if(guessNumber == randomNumber)
	{
		std::cout << "Congratulation, " << guessNumber << " is your lucky number.\n";
	}
	else
	{
		std::cout << "Sorry, I'm thinking about number " << randomNumber << "\n"; 
	}
	return 0;
}