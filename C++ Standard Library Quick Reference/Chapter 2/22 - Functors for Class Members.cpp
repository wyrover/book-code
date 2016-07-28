#include <iostream>
#include <functional>

struct my_struct { int val; bool fun(int i) { return val == i; } };

int main()
{
	my_struct s{ 234 };

	std::function<int(my_struct&)> f_get_val = &my_struct::val;
	std::function<bool(my_struct&, int)> f_call_fun = &my_struct::fun;
	std::cout << f_get_val(s) << ' ' << f_call_fun(s, 123) << std::endl;

	using std::placeholders::_1;
	auto b_get_val = std::bind(&my_struct::val, _1);
	auto b_call_fun_on_s = std::bind(&my_struct::fun, std::ref(s), _1);
	std::cout << b_get_val(s) << ' ' << b_call_fun_on_s(234) << std::endl;

	auto m_get_val = std::mem_fn(&my_struct::val);
	auto m_call_fun = std::mem_fn(&my_struct::fun);
	std::cout << m_get_val(s) << ' ' << m_call_fun(s, 456) << std::endl;


	std::cout << std::endl;
}
