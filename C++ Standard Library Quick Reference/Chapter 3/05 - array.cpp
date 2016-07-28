#include <iostream>
#include <array>

int main()
{
	{
		std::array<int, 3> myArray;
	}


	{
		std::array<int, 3> myArray{ 1,2 };  // 1,2,0
	}


	{
		std::array<int, 3> myArray{};  // 0,0,0


		myArray.fill(5);  // 5,5,5
	}


	std::cout << std::endl;
}
