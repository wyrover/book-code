#include <boost/variant.hpp>
#include <iostream>
#include <string>

struct SimpleVariantVisitor : public boost::static_visitor<void> {
    void operator()(const std::string& s) const
    {
        std::cout << "String: " << s << '\n';
    }

    void operator()(long n) const
    {
        std::cout << "long: " << n << '\n';
    }
};

int main()
{
    boost::variant<std::string, long, double> v1;
    v1 = 993.3773;
    boost::apply_visitor(SimpleVariantVisitor(), v1);
}

