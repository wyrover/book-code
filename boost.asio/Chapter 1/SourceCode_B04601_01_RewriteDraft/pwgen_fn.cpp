/* pwgen_fn.cpp */
#include "pwgen_fn.h"
std::string PasswordGenerator::Generate(int passwordLength)
{
	int randomNumber;
	std::string password;
	std::srand(std::time(0));
	for(int i=0; i < passwordLength; i++)
	{
		randomNumber = std::rand() % 94 + 33;
		password += (char) randomNumber;
	}
	return password;
}