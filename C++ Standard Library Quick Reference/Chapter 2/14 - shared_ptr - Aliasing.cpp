#include <iostream>
#include <memory>

struct A
{
	int* member;
	/* ... */
};

int main()
{
	auto a = std::make_shared<A>();
	auto m = std::shared_ptr<int>(a, a->member);  // aliasing constructor
	// a.use_count() == m.use_count() == 2


	std::cout << std::endl;
}
