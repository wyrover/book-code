#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/grep.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/regex.hpp>
#include <iostream>
namespace io = boost::iostreams;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 0;
    }

    io::file_source infile(argv[1]);
    io::filtering_istream fis;
    io::grep_filter grep(boost::regex("^\\s*$"),
                         boost::regex_constants::match_default, io::grep::invert);
    fis.push(grep);
    fis.push(infile);
    io::copy(fis, std::cout);
}

