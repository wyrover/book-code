#include <iostream>
#include <queue>
#include <stack>
#include "../Common/Person.h"

int main()
{
	std::queue<Person> cont;
	cont.emplace("Doug", "B", true);
	cont.emplace("Phil", "W", false);
	cont.emplace("Stu", "P", true);
	cont.emplace("Alan", "G", false);
	while (!cont.empty())
	{
		std::cout << cont.front() << std::endl;  // queue
		// std::cout << cont.top() << std::endl; // priority_queue and stack
		cont.pop();
	}


	std::cout << std::endl;
}
