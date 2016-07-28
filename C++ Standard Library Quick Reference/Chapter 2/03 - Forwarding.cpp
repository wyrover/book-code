#include <iostream>

struct A
{
	A() {}
	A(const A&) = delete;	// A objects cannot be copied
};

void f(const A&)
{
	std::cout << "lval, ";	// forwarded as lvalue reference
}

void f(A&&)
{
	std::cout << "rval, ";	// forwarded as rvalue reference
}

// Three different forwarding (fwd) schemes:
template <typename T> void good_fwd(T&& t) { f(std::forward<T>(t)); }
template <typename T> void bad_fwd(T&& t)  { f(t); }
template <typename T> void ugly_fwd(T t)   { f(t); }

int main()
{
	A a;
	good_fwd(a); good_fwd(std::move(a)); good_fwd(A()); // lval, rval, rval, 
	bad_fwd(a);  bad_fwd(std::move(a)); bad_fwd(A());   // lval, lval, lval, 
	// ugly_fwd(a); ugly_fwd(std::move(a)); ugly_fwd(A()); --> error: 3x copy

	std::cout << std::endl;
}
