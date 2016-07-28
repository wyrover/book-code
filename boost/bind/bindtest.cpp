#include <boost/bind.hpp>
#include <iostream>

int foo(int a, int b)
{
	return a + b;
}

struct Functor
{
	int operator() (int a, int b) { return a - b; }
};

struct FunctorWithResultType
{
	typedef int result_type;
	int operator() (int a, int b) { return a - b; }
};

class ClassBar
{
public:
	int mul(int a, int b) { return a * b; }
};

int main()
{
	auto fn1 = boost::bind(foo, 1, 2);
	std::cout << "foo(1, 2) = " << fn1() << std::endl;

	auto fn2 = boost::bind(foo, _1, 2);
	std::cout << "foo(5, 2) = " << fn2(5) << std::endl;

	auto fn3 = boost::bind(foo, 1, _1);
	std::cout << "foo(1, 5) = " << fn3(5) << std::endl;

	Functor functor1;
	auto fn4 = boost::bind<int>(functor1, _2, _1);  // bind<int>(), int is result_type
	std::cout << "functor1(5, 1) = " << fn4(1, 5) << std::endl;

	FunctorWithResultType functor2;
	auto fn5 = boost::bind(functor2, _2, _1);
	std::cout << "functor2(5, 1) = " << fn5(1, 5) << std::endl;

	ClassBar bar;
	auto fn6 = boost::bind(&ClassBar::mul, &bar, _1, 9);
	std::cout << "ClassBar::mul(3, 9) = " << fn6(3) << std::endl;

	return 0;
}

