#include <iostream>
#include <string>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{
    // Parameter checking...
    try {
        // Turn the args into absolute paths using native formatting
        path src = complete(path(argv[1], native));
        path dst = complete(path(argv[2], native));
        copy_file(src, dst);
    } catch (exception& e) {
        cerr << e.what() << endl;
    }

    return (EXIT_SUCCESS);
}