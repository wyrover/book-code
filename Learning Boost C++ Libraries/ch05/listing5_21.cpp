#include <boost/assign.hpp>
#include <iostream>

using namespace boost::assign;

template<typename RangeType>
int inspect_range(RangeType&& rng)
{
    size_t sz = boost::size(rng);

    if (sz > 0) {
        std::cout << "First elem: " << *boost::begin(rng) << '\n';
        std::cout << "Last elem: " << *(boost::end(rng) - 1) << '\n';
    }

    return sz;
}

int main()
{
    std::cout << inspect_range(
                  cref_list_of<10>(1)(2)(3)(4)(5)(6)(7)(8));
    typedef std::map<std::string, std::string> strmap_t;
    strmap_t helloWorlds =
        cref_list_of<3, strmap_t::value_type>
        (strmap_t::value_type("hello", "world"))
        (strmap_t::value_type("hola", "el mundo"))
        (strmap_t::value_type("hallo", "Welt"));
}

