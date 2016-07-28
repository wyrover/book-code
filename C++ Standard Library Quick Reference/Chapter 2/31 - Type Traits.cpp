#include <iostream>
#include <type_traits>

int main()
{
	std::cout << std::boolalpha;            // Print true/false instead of 1/0
	std::cout << std::is_integral<int>::value << '\n';                 // true
	std::cout << std::is_class<std::is_class<bool>>::value << '\n';    // true
	std::cout << std::is_function<int(void)>::value << '\n';           // true
	std::cout << std::is_function<decltype(main)>::value << '\n';      // true
	std::cout << std::is_pointer<decltype(&main)>::value << '\n';      // true
	struct A { void f() {}; };
	void(A::* p)() = &A::f;
	std::cout << std::is_member_function_pointer<decltype(p)>() << '\n';// true

	std::cout << std::endl;
}
