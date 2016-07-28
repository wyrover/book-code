#include <boost/variant.hpp>
#include <iostream>
#include <string>

class Foo
{
public:
    Foo() {}
};

std::ostream& operator<<(std::ostream& os, const Foo&)
{
    os << "Foo\n";
    return os;
}

struct PrintVisitor : boost::static_visitor<> {
    template <typename T>
    void operator()(const T& t) const
    {
        std::cout << t << '\n';
    }
};

int main()
{
    boost::variant<std::string, double, long, Foo> v1;
    boost::apply_visitor(PrintVisitor(), v1);
}

