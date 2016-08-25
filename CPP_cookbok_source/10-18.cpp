#include <iostream>
#include <string>
#include <cstdlib>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace std;
using namespace boost::filesystem;

void removeRecurse(const path& p)
{
    // First, remove the contents of the directory
    directory_iterator end;

    for (directory_iterator it(p);
         it != end; ++it) {
        if (is_directory(*it)) {
            removeRecurse(*it);
        } else {
            remove(*it);
        }
    }

    // Then, remove the directory itself
    remove(p);
}

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << "Usage: " << argv[0] << " [dir name]\n";
        return (EXIT_FAILURE);
    }

    path thePath = system_complete(path(argv[1], native));

    if (!exists(thePath)) {
        cerr << "Error: the directory " << thePath.string()
             << " does not exist.\n";
        return (EXIT_FAILURE);
    }

    try {
        removeRecurse(thePath);
    } catch (exception& e) {
        cerr << e.what() << endl;
        return (EXIT_FAILURE);
    }

    return (EXIT_SUCCESS);
}