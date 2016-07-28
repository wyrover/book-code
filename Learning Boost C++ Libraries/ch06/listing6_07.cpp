#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/random_access_index.hpp>
#include <iostream>
#include <cassert>
#include "PersonEntry.h"
#include <boost/lambda/lambda.hpp>

// by_name, by_phone defined Listing 6.3
using namespace boost::multi_index;

typedef ordered_non_unique<member<PersonEntry, std::string,
        &PersonEntry::name>> by_name;
typedef ordered_unique<member<PersonEntry, std::string,
        &PersonEntry::phoneNumber>> by_phone;
typedef multi_index_container<PersonEntry,
        indexed_by<random_access<>,
        by_name, by_phone>> phdir_t;

int main()
{
    phdir_t phonedir;
    phonedir.push_back(PersonEntry{"Dr. Dolittle",
                                   "639 420 7624", "Atlanta"}); // insert won't work
    auto& phindx = phonedir.get<2>();
    phindx.insert(PersonEntry{"Arindam Mukherjee",
                              "550 888 9999", "Pune"});
    auto& nameindx = phonedir.get<1>();
    nameindx.insert(PersonEntry{"Arindam Mukherjee",
                                "990 770 2458", "Calcutta"});
    phonedir.push_front(PersonEntry{"Ace Ventura",
                                    "457 330 1288", "Tampa"});
    nameindx.erase("Arindam Mukherjee");  // erases 2 matching
    phonedir.erase(phonedir.begin());     // erases Ace Ventura
    assert(phonedir.size() == 1);
    std::cout << "The lonesome " << phonedir.begin()->name << '\n';
    phonedir.push_back(PersonEntry{"Tarzan", "639 420 7624",
                                   "Okavango"});
    assert(phonedir.size() == 1);
    std::cout << "Still alone " << phonedir.begin()->name << '\n';
    phonedir.push_back(PersonEntry{"Tarzan", "9441500252",
                                   "Okavango"});
    assert(phonedir.size() == 2);
    PersonEntry tarzan = *(phonedir.begin() + 1);
    tarzan.phoneNumber = "639 420 7624";
    assert(!phonedir.replace(phonedir.begin() + 1, tarzan));
}

