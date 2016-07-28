#include <boost/container/vector.hpp>
#include <boost/foreach.hpp>
#include "String.h"  // for class String
#include <iostream>
#include <cassert>

int main()
{
    boost::container::vector<String> strVec;
    String world("world");
    // Move temporary (rvalue)
    strVec.push_back(String("Hello"));
    // Error, copy semantics needed
    //strVec.push_back(world);
    // Explicit move
    strVec.push_back(boost::move(world));
    // world nulled after move
    assert(world.get() == nullptr);
    // in-place construction
    strVec.emplace_back("Hujambo Dunia!"); // Swahili
    BOOST_FOREACH(String & str, strVec) {
        std::cout << str.get() << '\n';
    }
}

