#include <string>
#include <boost/algorithm/string.hpp>
#include <cassert>

int main()
{
    std::string song = "Green-tinted sixties mind";
    boost::to_upper(song);
    assert(song == "GREEN-TINTED SIXTIES MIND");
}

