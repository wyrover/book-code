#include <boost/mpl/and.hpp>
#include <boost/mpl/int.hpp>
#include <boost/mpl/integral_c.hpp>
#include <boost/mpl/less.hpp>
#include <iostream>
namespace mpl = boost::mpl;

template <typename L, typename R>
struct is_smaller : mpl::less <
mpl::integral_c<size_t, sizeof(L)>
, mpl::integral_c<size_t, sizeof(R) >> {
};

int main()
{
    if (is_smaller<short, int>::value) {
        std::cout << "short is smaller than int\n";
    } else {
        std::cout << "short is larger than int\n";
    }
}

