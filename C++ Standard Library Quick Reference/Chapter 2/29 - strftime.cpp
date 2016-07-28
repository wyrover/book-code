#define _CRT_SECURE_NO_WARNINGS	// To make sure localtime() works with Visual C++.

#include <iostream>
#include <ctime>

int main()
{
	time_t time = std::time(nullptr);
	tm time_tm = *std::localtime(&time);
	char buffer[256];
	strftime(buffer, sizeof(buffer), "Today is %a %e/%m%n", &time_tm);
	std::cout << buffer;          // Today is Sat 30/01	
	strftime(buffer, sizeof(buffer), "%c--%x %X", &time_tm);
	std::cout << buffer << '\n';  // Sat Jan 30 17:58:23 2016--01/30/16 17:58:23

	std::cout << std::endl;
}
