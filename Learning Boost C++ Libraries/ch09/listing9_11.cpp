#include <boost/iostreams/stream.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <iostream>
#include <string>
#include <cassert>
#include <sys/types.h>
#include <fcntl.h>
#ifdef WINNT
    #include <io.h>
#endif
namespace io = boost::iostreams;

int main(int argc, char *argv[])
{
    if (argc < 2) {
        return 0;
    }

    int fdr = open(argv[1], O_RDONLY);

    if (fdr >= 0) {
        io::file_descriptor_source fdDevice(fdr,
                                            io::file_descriptor_flags::close_handle);
        io::stream<io::file_descriptor_source> in(fdDevice);
        assert(fdDevice.is_open());
        std::string line;

        while (std::getline(in, line))
            std::cout << line << '\n';
    }
}

