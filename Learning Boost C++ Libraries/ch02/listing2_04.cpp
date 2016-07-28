#include <boost/variant.hpp>
#include <string>

struct Foo {
    Foo(int n = 0) : id_(n) {} // int convertible to Foo
private:
    int id_;
};

struct Bar {
    Bar(int n = 0) : id_(n) {} // int convertible to Bar
private:
    int id_;
};

int main()
{
    boost::variant<Foo, int, std::string> value; // error if Foo
    // not be default constructible
    boost::variant<std::string, Foo, Bar> value2;
    value = 1;                 // sets int, not Foo
    int *pi = boost::get<int>(&value);
    assert(pi != 0);
    value = "foo";             // sets std::string
    value = Foo(42);           // sets Foo
    // value2 = 1;             // ERROR: ambiguous - Foo or Bar?
    // std::cout << value << ' ' << value2 << '\n'; // ERROR:
    // Foo, Bar cannot be streamed to ostream
}

