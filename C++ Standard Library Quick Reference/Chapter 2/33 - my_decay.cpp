#include <iostream>
#include <typeinfo>

using namespace std;

template<typename T>
struct my_decay
{
private:
	typedef remove_reference_t<T> U;

public:
	typedef conditional_t<is_array<U>::value, remove_extent_t<U>*,
		conditional_t<is_function<U>::value, add_pointer_t<U>,
		remove_cv_t<U >> > type;
};


int main()
{
	std::cout << typeid(my_decay<int&&>::type).name() << '\n';			// int
	std::cout << typeid(my_decay<const int[12]>::type).name() << '\n';	// const int *


	std::cout << std::endl;
}
