#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <vector>
#include <cassert>

int main()
{
    std::string dogtypes = "mongrel, puppy, whelp, hound";
    std::vector<std::string> dogs;
    boost::split(dogs, dogtypes, boost::is_any_of(" ,"),
                 boost::token_compress_on);
    assert(dogs.size() == 4);
    assert(dogs[0] == "mongrel" && dogs[1] == "puppy" &&
           dogs[2] == "whelp" && dogs[3] == "hound");
}

