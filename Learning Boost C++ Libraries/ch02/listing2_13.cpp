#include <boost/current_function.hpp>
#include <iostream>

namespace FoFum
{
class Foo
{
public:
    void bar()
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
        bar_private(5);
    }

    static void bar_static()
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
    }

private:
    float bar_private(int x) const
    {
        std::cout << BOOST_CURRENT_FUNCTION << '\n';
        return 0.0;
    }
};
} // end namespace FoFum

namespace
{
template <typename T>
void baz(const T& x)
{
    std::cout << BOOST_CURRENT_FUNCTION << '\n';
}
} // end unnamed namespace

int main()
{
    std::cout << BOOST_CURRENT_FUNCTION << '\n';
    FoFum::Foo f;
    f.bar();
    FoFum::Foo::bar_static();
    baz(f);
}

