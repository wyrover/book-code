#include <boost/algorithm/string.hpp>
#include <string>
#include <iostream>
#include <vector>

int main()
{
    std::string dogtypes =
        "mongrel and puppy and whelp and hound";
    std::vector<std::string> dogs;
    boost::iter_split(dogs, dogtypes,
                      boost::first_finder(" and "));
    assert(dogs.size() == 4);
    assert(dogs[0] == "mongrel" && dogs[1] == "puppy" &&
           dogs[2] == "whelp" && dogs[3] == "hound");
}

