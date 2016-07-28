#include <boost/bimap.hpp>
#include <boost/assign.hpp>
#include <string>
#include <iostream>
#include <cassert>
using namespace boost::assign;

typedef boost::bimap<std::string, std::string> string_bimap_t;

int main()
{
    string_bimap_t countryCapitals;
    insert(countryCapitals)("Slovenia", "Ljubljana")
    ("New Zealand", "Wellington")
    ("Tajikistan", "Bishkek")
    ("Chile", "Santiago")
    ("Jamaica", "Kingston");
    string_bimap_t::left_map& countries = countryCapitals.left;
    string_bimap_t::left_map::const_iterator it
        = countries.find("Slovenia");

    if (it != countries.end()) {
        std::cout << "Capital of " << it->first << " is "
                  << it->second << "\n";
    }

    string_bimap_t::right_map& cities = countryCapitals.right;
    string_bimap_t::right_map::const_iterator it2
        = cities.find("Santiago");

    if (it2 != cities.end()) {
        std::cout << it2->first << " is the capital of "
                  << it2->second << "\n";
    }

    size_t size = countryCapitals.size();
    countryCapitals.insert(
        string_bimap_t::value_type("Chile", "Valparaiso"));
    assert(countries.at("Chile") == "Santiago");
    assert(size == countryCapitals.size());
    countryCapitals.insert(
        string_bimap_t::value_type("Norfolk Island", "Kingston"));
    assert(cities.at("Kingston") == "Jamaica");
    assert(size == countryCapitals.size());
}

