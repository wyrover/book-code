#include <catch.hpp>
#include <tracetool/tracetool.h>
#include <Windows.h>
#include <iostream>
#include <map>

using std::multimap;

TEST_CASE("Test for std::multimap", "[std::multimap]")
{
    SECTION("multimap", "[std::multimap]") {
        // map from author to title; there can be multiple titles per author
        multimap<string, string> authors;
        // add data to authors
        authors.insert({"Alain de Botton", "On Love"});
        authors.insert({"Alain de Botton", "Status Anxiety"});
        authors.insert({"Alain de Botton", "Art of Travel"});
        authors.insert({"Alain de Botton",
                        "Architecture of Happiness"
                       });
        string search_item("Alain de Botton"); // author we'll look for
        auto entries = authors.count(search_item); // number of elements
        auto iter = authors.find(search_item); // first entry for this author

        // loop through the number of entries there are for this author
        while (entries) {
            cout << iter->second << endl; // print each title
            ++iter;     // advance to the next title
            --entries;  // keep track of how many we've printed
        }

        // definitions of authors and search_item as above
        // beg and end denote the range of elements for this author
        for (auto beg = authors.lower_bound(search_item),
             end = authors.upper_bound(search_item);
             beg != end; ++beg)
            cout << beg->second << endl; // print each title

        // definitions of authors and search_item as above
        // pos holds iterators that denote the range of elements for this key
        for (auto pos = authors.equal_range(search_item);
             pos.first != pos.second; ++pos.first)
            cout << pos.first->second << endl; // print each title



		
    }
}

