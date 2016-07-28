#include <string>
#include <boost/algorithm/string.hpp>
#include <cassert>

int main()
{
    char song[17] = "Book of Taliesyn";
    typedef boost::iterator_range<char*> RangeType;
    RangeType rng = boost::make_iterator_range(song + 8,
                    song + 16);
    boost::to_upper(rng);
    assert(std::string(song) == "Book of TALIESYN");
}

