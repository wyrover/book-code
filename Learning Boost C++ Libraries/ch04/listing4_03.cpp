#include <string>
#include <boost/algorithm/string.hpp>
#include <cassert>

int main()
{
    char song[17] = "Book of Taliesyn";
    boost::to_upper(song);
    assert(std::string(song) == "BOOK OF TALIESYN");
}

