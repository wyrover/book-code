#include <iostream>
#include <boost/filesystem/operations.hpp>
#include <boost/filesystem/fstream.hpp>

using namespace boost::filesystem;

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [dir name]\n";
        return (EXIT_FAILURE);
    }

    path fullPath =    // Create the full, absolute path name
        system_complete(path(argv[1], native));

    if (!exists(fullPath)) {
        std::cerr << "Error: the directory " << fullPath.string()
                  << " does not exist.\n";
        return (EXIT_FAILURE);
    }

    if (!is_directory(fullPath)) {
        std::cout << fullPath.string() << " is not a directory!\n";
        return (EXIT_SUCCESS);
    }

    directory_iterator end;

    for (directory_iterator it(fullPath);
         it != end; ++it) {               // Iterate through each
        // element in the dir,
        std::cout << it->leaf();            // almost as you would

        if (is_directory(*it))             // an STL container
            std::cout << " (dir)";

        std::cout << '\n';
    }

    return (EXIT_SUCCESS);
}