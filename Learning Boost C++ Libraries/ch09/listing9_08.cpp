#include <boost/filesystem.hpp>
#include <iostream>
#include <cassert>
namespace fs = boost::filesystem;

int main()
{
    fs::path p1 = "notpresent/dirtest";
    boost::system::error_code ec;

    if (!is_directory(p1.parent_path()) || exists(p1)) {
        assert(! create_directory(p1, ec));

        if (is_directory(p1)) assert(!ec.value());
        else assert(ec.value());
    }

    try {
        if (create_directories(p1)) {
            assert(! create_directory(p1));
        }
    } catch (std::exception& e) {
        std::cout << "Exception caught: " << e.what() << '\n';
    }
}

