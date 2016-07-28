#include <boost/shared_ptr.hpp>
#include <iostream>
#include <cassert>

class Foo
{
public:
    Foo() {}
    ~Foo()
    {
        std::cout << "~Foo() destructor invoked." << '\n';
    }
};

typedef boost::shared_ptr<Foo> SPFoo;

int main()
{
    SPFoo f1(new Foo);
    // SPFoo f1 = new Foo; // Won’t work, explicit ctor
    assert(f1.use_count() == 1);
    // copy construction
    SPFoo f2(f1);
    assert(f1.use_count() == f2.use_count() &&
           f1.get() == f2.get() && f1.use_count() == 2);
    std::cout << "f1 use_count: " << f1.use_count() << '\n';
    SPFoo f3(new Foo);
    SPFoo f4(f3);
    assert(f3.use_count() == 2 && f3.get() == f4.get());
    std::cout << "f3 use_count: " << f3.use_count() << '\n';
    // copy assignment
    f4 = f1;
    assert(f4.use_count() == f1.use_count() &&
           f1.use_count() == 3 && f1.get() == f4.get());
    assert(f3.use_count() == 1);
    std::cout << "f1 use_count: " << f1.use_count() << '\n';
    std::cout << "f3 use_count: " << f3.use_count() << '\n';
}

