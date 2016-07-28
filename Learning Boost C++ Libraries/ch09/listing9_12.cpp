#include <boost/iostreams/device/array.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <vector>
namespace io = boost::iostreams;

int main()
{
    char out_array[256];
    io::array_sink sink(out_array, out_array + sizeof(out_array));
    io::stream<io::array_sink> out(sink);
    out << "Size of out_array is " << sizeof(out_array)
        << '\n' << std::ends << std::flush;
    std::vector<char> vchars(out_array,
                             out_array + strlen(out_array));
    io::array_source src(vchars.data(), vchars.size());
    io::stream<io::array_source> in(src);
    io::copy(in, std::cout);
}

