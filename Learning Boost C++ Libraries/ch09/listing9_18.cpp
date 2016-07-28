#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filter/bzip2.hpp>
#include <boost/iostreams/filter/zlib.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/tee.hpp>
namespace io = boost::iostreams;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 0;
    }

    io::file_source infile(argv[1]);  // input
    io::stream<io::file_source> ins(infile);
    io::gzip_compressor gzip;
    io::file_sink gzfile(argv[1] + std::string(".gz"));
    io::filtering_ostream gzout;     // gz output
    gzout.push(gzip | gzfile);
    auto gztee = tee(gzout);
    io::bzip2_compressor bzip2;
    io::file_sink bz2file(argv[1] + std::string(".bz2"));
    io::filtering_ostream bz2out;     // bz2 output
    bz2out.push(bzip2 | bz2file);
    auto bz2tee = tee(bz2out);
    io::zlib_compressor zlib;
    io::file_sink zlibfile(argv[1] + std::string(".zlib"));
    io::filtering_ostream zlibout;
    zlibout.push(gztee | bz2tee | zlib | zlibfile);
    io::copy(ins, zlibout);
}

