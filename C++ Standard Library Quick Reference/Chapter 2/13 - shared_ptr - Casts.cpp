#include <iostream>
#include <memory>

class A { public: virtual ~A() {}; };
class B : public A {};
class C {};

int main()
{
	std::shared_ptr<A> a = std::make_shared<B>();    // a points to a new B()
	std::shared_ptr<B> b = std::dynamic_pointer_cast<B>(a);
	std::shared_ptr<C> c = std::dynamic_pointer_cast<C>(a);
	// (c == nullptr) && (a.use_count() == b.use_count() == 2)

	std::cout << std::endl;
}
