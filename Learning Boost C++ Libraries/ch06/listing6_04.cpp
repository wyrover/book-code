#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <cassert>
#include "PersonEntry.h"  // contains PersonEntry definition
using namespace boost::multi_index;

typedef ordered_non_unique<member<PersonEntry, std::string,
        &PersonEntry::name>> by_name;
typedef ordered_unique<member<PersonEntry, std::string,
        &PersonEntry::phoneNumber>>by_phone;

typedef multi_index_container<PersonEntry,
        indexed_by<by_name,
        by_phone>> directory_t;
int main()
{
    directory_t phonedir;
    PersonEntry p1{"Arindam Mukherjee", "550 888 9999", "Pune"};
    PersonEntry p2{"Arindam Mukherjee", "990 770 2458",
                   "Calcutta"};
    PersonEntry p3{"Ace Ventura", "457 330 1288", "Tampa"};
    phonedir.insert(p1);
    phonedir.insert(p2);
    phonedir.insert(p3);
    auto iter = phonedir.find("Ace Ventura");
    assert(iter != phonedir.end() && iter->city == "Tampa");
    auto& ph_indx = phonedir.get<1>();
    auto iter2 = ph_indx.find("990 770 2458");
    assert(iter2 != ph_indx.end());
    assert(iter2->city == "Calcutta");

    for (auto& elem : ph_indx) {
        std::cout << elem.name << " lives in " << elem.city
                  << " and can be reached at " << elem.phoneNumber
                  << '\n';
    }
}

