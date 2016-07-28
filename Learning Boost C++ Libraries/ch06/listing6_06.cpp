#include <boost/multi_index_container.hpp>
#include <boost/multi_index/indexed_by.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/identity.hpp>
#include <boost/multi_index/member.hpp>
#include <iostream>
#include <cassert>
#include "PersonEntry.h"
#include <boost/lambda/lambda.hpp>

using namespace boost::multi_index;

namespace bl = boost::lambda;  // lambda placeholder

typedef ordered_non_unique<member<PersonEntry, std::string,
        &PersonEntry::name>> by_name;
typedef ordered_unique<member<PersonEntry, std::string,
        &PersonEntry::phoneNumber>>by_phone;

typedef multi_index_container<PersonEntry,
        indexed_by<by_name,
        by_phone>> directory_t;
int main()
{
    directory_t phonedir;  // directory_t defined in listing 6.3
    phonedir.insert(PersonEntry{"Dr. Dolittle", "639 420 7624",
                                "Atlanta"});
    phonedir.insert(PersonEntry{"Arindam Mukherjee",
                                "990 770 2458", "Calcutta"});
    phonedir.insert(PersonEntry{"Ace Ventura", "457 330 1288",
                                "Tampa"});
    phonedir.insert(PersonEntry{"Arindam Mukherjee",
                                "550 888 9999", "Pune"});
    auto& name_index = phonedir.get<0>();
    auto range = name_index.range("Ar" <= bl::_1, "D" > bl::_1);

    for (auto start = range.first; start != range.second;
         ++start) {
        std::cout << start->name << ", " << start->phoneNumber
                  << ", " << start->city << "\n";
    }
}

