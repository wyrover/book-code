#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/device/back_inserter.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <vector>
namespace io = boost::iostreams;

int main()
{
    typedef std::vector<char> charvec;
    charvec output;
    io::back_insert_device<charvec> sink(output);
    io::stream<io::back_insert_device<charvec>> out(sink);
    out << "Size of output is " << output.size() << std::flush;
    std::vector<char> vchars(output.begin(),
                             output.begin() + output.size());
    io::array_source src(vchars.data(), vchars.size());
    io::stream<io::array_source> in(src);
    io::copy(in, std::cout);
}

