#include <iostream>
#include <functional>

int main()
{
	std::plus<> functor;                         // defaults to std::plus<void> 
	std::cout << functor(234, 432) << ' ' << functor(1.101, 2.0405) << std::endl;

	std::cout << std::endl;
}
