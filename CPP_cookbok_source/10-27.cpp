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
        // Compose a path from the two args
        path p1 = complete(path(argv[2], native),
                           path(argv[1], native));
        cout << p1.string() << endl;
        // Create a path with a base of the current dir
        path p2 = system_complete(path(argv[2], native));
        cout << p2.string() << endl;
    } catch (exception& e) {
        cerr << e.what() << endl;
    }

    return (EXIT_SUCCESS);
}