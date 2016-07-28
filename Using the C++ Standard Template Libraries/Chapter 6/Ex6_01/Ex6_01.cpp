// Ex6_01.cpp
// Sorting class objects
#include <iostream>                              // For standard streams
#include <string>                                // For string class
#include <vector>                                // For vector container
#include <iterator>                              // For stream and back insert iterators
#include <algorithm>                             // For sort() algorithm
#include "Name.h"


int main()
{
    std::vector<Name> names;
    std::cout << "Enter names as first name followed by second name. Enter Ctrl+Z to end:";
    std::copy(std::istream_iterator<Name>(std::cin), std::istream_iterator<Name>(),
              std::back_insert_iterator<std::vector<Name>>(names));
    std::cout << names.size() << " names read. Sorting in ascending sequence...\n";
    std::sort(std::begin(names), std::end(names), [](const Name & name1, const Name & name2) {
        return name1.get_second() < name2.get_second();
    });
    std::cout << "\nThe names in ascending sequence are:\n";
    std::copy(std::begin(names), std::end(names), std::ostream_iterator<Name>(std::cout, "\n"));
}
