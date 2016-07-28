#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
namespace io = boost::iostreams;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 0;
    }

    // compress
    io::file_source infile(argv[1]);
    io::filtering_istream fis;
    io::gzip_compressor gzip;
    fis.push(gzip);
    fis.push(infile);
    io::file_sink outfile(argv[1] + std::string(".gz"));
    io::stream<io::file_sink> os(outfile);
    io::copy(fis, os);
    // decompress
    io::file_source infile2(argv[1] + std::string(".gz"));
    fis.reset();
    io::gzip_decompressor gunzip;
    fis.push(gunzip);
    fis.push(infile2);
    io::copy(fis, std::cout);
}

