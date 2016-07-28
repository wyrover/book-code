#include <iostream>
#include <deque>

int main()
{
	std::deque<int> myDeque = { 1,2,3,4 };
	myDeque.insert(myDeque.begin() + 2, 22); // 1,2,22,3,4
	myDeque.pop_front();                     // 2,22,3,4
	myDeque.erase(myDeque.begin() + 1);      // 2,3,4
	myDeque.push_front(11);                  // 11,2,3,4


	std::cout << std::endl;
}
