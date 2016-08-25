#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{
    // Parameter checking...
    try {
        path p = complete(path(argv[1], native));
        create_directory(p);
    } catch (exception& e) {
        cerr << e.what() << endl;
    }

    return (EXIT_SUCCESS);
}