/* stl.cpp */
#include <vector>
#include <iostream>
#include <algorithm>

int main(void)
{
	int temp;
	std::vector<int> collection;
	std::cout << "Please input the collection of integer numbers, input 0 to STOP!\n";
	while(std::cin >> temp != 0)
	{
		if(temp == 0) break;
		collection.push_back(temp);
	}
	std::sort(collection.begin(), collection.end());
	std::cout << "\nThe sort collection of your integer numbers:\n";
    for(int i: collection)
    {
        std::cout << i << std::endl;
    }
}