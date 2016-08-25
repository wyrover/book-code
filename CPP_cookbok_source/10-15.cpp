#include <iostream>
#include <direct.h>

int main(int argc, char** argv)
{
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " [new dir name]\n";
        return (EXIT_FAILURE);
    }

    if (mkdir(argv[1]) == -1) {  // Create the directory
        std::cerr << "Error: " << strerror(errno);
        return (EXIT_FAILURE);
    }
}