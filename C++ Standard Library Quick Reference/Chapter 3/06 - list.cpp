#include <iostream>
#include <list>

int main()
{
	std::list<int> list1{ 1,7,5 }, list2{ 5,6,2 }, list3{ 3,4 };
	list1.sort();        // 1,5,7
	list2.sort();        // 2,5,6
	list1.merge(list2);  // list1 = 1,2,5,5,6,7
						 // list2 = empty
	list1.unique();      // 1,2,5,6,7

	auto splicePosition = std::next(begin(list1), 2);
	list1.splice(splicePosition, list3); // list1 = 1,2,3,4,5,6,7
										 // list3 = empty


	std::cout << std::endl;
}
