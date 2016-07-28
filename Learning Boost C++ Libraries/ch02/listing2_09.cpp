#include <boost/any.hpp>
#include <vector>
#include <iostream>
#include <string>
#include <cassert>
using boost::any_cast;

struct MyValue {
    MyValue(int n) : value(n) {}

    int get() const
    {
        return value;
    }

    int value;
};

int main()
{
    boost::any v1, v2, v3, v4;
    assert(v1.empty());
    const char *hello = "Hello";
    v1 = hello;
    v2 = 42;
    v3 = std::string("Hola");
    MyValue m1(10);
    v4 = m1;

    try {
        std::cout << any_cast<const char*>(v1) << '\n';
        std::cout << any_cast<int>(v2) << '\n';
        std::cout << any_cast<std::string>(v3) << '\n';
        auto x = any_cast<MyValue>(v4);
        std::cout << x.get() << '\n';
    } catch (std::exception& e) {
        std::cout << e.what() << '\n';
    }
}

