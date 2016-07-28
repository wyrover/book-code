#include <iostream>
#include <memory>

int main()
{
	{
		auto bond = std::make_shared<int>(007);         // bond.use_count() == 1
		auto james = bond;    // james.use_count() == 2 && bond.use_count() == 2
		bond.reset();         // james.use_count() == 1 && bond.use_count() == 0
	}						  // 007 is deleted

	std::cout << std::endl;
}
