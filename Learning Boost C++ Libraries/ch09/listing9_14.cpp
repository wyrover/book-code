#include <boost/iostreams/device/file.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filter/counter.hpp>
#include <boost/iostreams/copy.hpp>
#include <iostream>
#include <vector>
namespace io = boost::iostreams;

int main(int argc, char *argv[])
{
    if (argc <= 1) {
        return 0;
    }

    io::file_source infile(argv[1]);
    io::counter counter;
    io::filtering_istream fis;
    fis.push(counter);
    assert(!fis.is_complete());
    fis.push(infile);
    assert(fis.is_complete());
    io::copy(fis, std::cout);
    io::counter *ctr = fis.component<io::counter>(0);
    std::cout << "Chars: " << ctr->characters() << '\n'
              << "Lines: " << ctr->lines() << '\n';
}

