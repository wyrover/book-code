#include <iostream>
#include <bitset>

int main()
{
	{
		std::bitset<10> myBitset;
	}


	{
		std::bitset<4> myBitset("1001");
	}

	std::cout << std::endl;
}
