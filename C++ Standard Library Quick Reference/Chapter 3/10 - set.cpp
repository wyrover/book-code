#include <iostream>
#include <set>

int main()
{
	std::set<int> mySet{ 3,2,1 };
	mySet.insert(2);
	mySet.insert(6);
	std::cout << mySet.size() << ' ' << *mySet.begin();    // 4 1


	std::cout << std::endl;
}
