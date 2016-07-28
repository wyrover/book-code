#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <boost/phoenix/core.hpp>
#include <boost/phoenix/function.hpp>
#include <boost/phoenix/operator.hpp>
#include <boost/phoenix/stl/container.hpp>
#include <boost/phoenix/stl/algorithm.hpp>

struct substr_impl {
    template<typename C, typename F1, typename F2>
    struct result  {
        typedef C type;
    };

    template<typename C, typename F1, typename F2>
    C operator()(const C& c, const F1& offset,
                 const F2& length) const
    {
        return c.substr(offset, length);
    }
};

int main()
{
    namespace phx = boost::phoenix;
    using phx::arg_names::arg1;
    std::vector<std::string> names{"Pete Townshend",
                                   "Roger Daltrey", "Keith Moon", "John Entwistle"};
    phx::function<substr_impl> const substr = substr_impl();
    std::for_each(names.begin(), names.end(), std::cout <<
                  substr(arg1, 0, phx::find(arg1, ' ')
                         - phx::begin(arg1))
                  << '\n');
}

