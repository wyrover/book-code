#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
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
    io::bzip2_compressor bzip2;
    io::grep_filter grep(boost::regex("^\\s*$"),
                         boost::regex_constants::match_default,
                         io::grep::invert);
    io::filtering_istream fis;
    fis.push(bzip2 | grep | infile);
    io::file_sink outfile(argv[1] + std::string(".bz2"));
    io::stream<io::file_sink> os(outfile);
    io::copy(fis, os);
}

