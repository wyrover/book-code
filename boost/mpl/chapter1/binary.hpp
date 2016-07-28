#include <boost/mpl/long.hpp>
#include <boost/mpl/alias.hpp>

template <long n>
struct binary : mpl::long_< (binary<n/10>::value << 1) + n%10 >
{
};

template <>
struct binary<0> : mpl::long_<0>
{
};

