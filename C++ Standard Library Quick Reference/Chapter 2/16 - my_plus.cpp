#include <iostream>

template <typename T>
struct my_plus
{
	T operator() (const T& x, const T& y) const { return x + y; }
};

int main()
{
	my_plus<int> functor;
	std::cout << functor(11, 22) << std::endl;		    // 33

	std::cout << std::endl;
}
