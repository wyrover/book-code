#include <iostream>
#include <direct.h>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 2) {
        cerr << "Usage: " << argv[0] << " [dir name]" << endl;
        return (EXIT_FAILURE);
    }

    if (rmdir(argv[1]) == -1) {  // Remove the directory
        cerr << "Error: " << strerror(errno) << endl;;
        return (EXIT_FAILURE);
    }
}