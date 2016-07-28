#include <boost/container/flat_set.hpp>
#include <iostream>
#include <string>

template<typename C>
void printContainerInternals(const C& container)
{
    std::cout << "Container layout" << '\n'
              << "-------------\n";

    for (const auto& elem : container) {
        std::cout << "[Addr=" << &elem
                  << "] : [value=" << elem << "]\n";
    }
}

int main()
{
    boost::container::flat_set<std::string> someStrings;
    someStrings.reserve(8);
    someStrings.insert("Guitar");
    printContainerInternals(someStrings);
    someStrings.insert("Mandolin");
    printContainerInternals(someStrings);
    someStrings.insert("Cello");
    printContainerInternals(someStrings);
    someStrings.insert("Sitar");
    printContainerInternals(someStrings);
}

