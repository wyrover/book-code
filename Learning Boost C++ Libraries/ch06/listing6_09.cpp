#include <boost/bimap.hpp>
#include <boost/bimap/multiset_of.hpp>
#include <boost/assign.hpp>
#include <string>
#include <iostream>
#include <cassert>
using namespace boost::assign;
namespace boostbi = boost::bimaps;

typedef boost::bimap<boostbi::multiset_of<std::string>,
        boostbi::multiset_of<std::string>> string_bimap_t;

int main()
{
    string_bimap_t namesShortNames;
    insert(namesShortNames)("Robert", "Bob")
    ("Robert", "Rob")
    ("William", "Will")
    ("Christopher", "Chris")
    ("Theodore", "Ted")
    ("Edward", "Ted");
    size_t size = namesShortNames.size();
    namesShortNames.insert(
        string_bimap_t::value_type("William", "Bill"));
    assert(size + 1 == namesShortNames.size());
    namesShortNames.insert(
        string_bimap_t::value_type("Christian", "Chris"));
    assert(size + 2 == namesShortNames.size());
    string_bimap_t::left_map& names = namesShortNames.left;
    string_bimap_t::left_map::const_iterator it1
        = names.lower_bound("William");
    string_bimap_t::left_map::const_iterator it2
        = names.upper_bound("William");

    while (it1 != it2) {
        std::cout << it1->second << " is a nickname for "
                  << it1->first << '\n';
        ++it1;
    }

    string_bimap_t::right_map& shortNames =
        namesShortNames.right;
    auto iter_pair = shortNames.equal_range("Chris");

    for (auto it3 = iter_pair.first; it3 != iter_pair.second;
         ++it3) {
        std::cout << it3->first << " is a nickname for "
                  << it3->second << '\n';
    }
}

