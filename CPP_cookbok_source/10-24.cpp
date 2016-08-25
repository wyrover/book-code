#include <iostream>
#include <cstdlib>
#include <boost/filesystem/operations.hpp>

using namespace std;
using namespace boost::filesystem;

int main(int argc, char** argv)
{
    // Parameter checking...
    try {
        path p = complete(path(argv[1], native));
        cout << p.branch_path().string() << endl;
    } catch (exception& e) {
        cerr << e.what() << endl;
    }

    return (EXIT_SUCCESS);
}