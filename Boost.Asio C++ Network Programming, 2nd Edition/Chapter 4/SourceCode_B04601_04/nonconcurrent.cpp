/* nonconcurrent.cpp */
#include <iostream>

void Print1(void)
{
	for(int i=0; i<5; i++)
	{
		std::cout << "[Print1] Line: " << i << "\n";
}
}

void Print2(void)
{
	for(int i=0; i<5; i++)
	{
		std::cout << "[Print2] Line: " << i << "\n";
	}
}

int main(void)
{
	Print1();
	Print2();
	return 0;
}