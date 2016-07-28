#include <string>
#include <boost/algorithm/string.hpp>
#include <cassert>

int main()
{
    std::string song = "Green-tinted sixties mind";
    typedef boost::iterator_range<std::string::iterator>
    RangeType;
    RangeType range = boost::make_iterator_range(
                          song.begin() + 13, song.begin() + 20);
    boost::to_upper(range);
    assert(song == "Green-tinted SIXTIES mind");
}

