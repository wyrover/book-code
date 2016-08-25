#include <iostream>
#include <cstdio>
#include <cerrno>

using namespace std;

int main(int argc, char** argv)
{
    if (argc != 2) {
        cerr << "You must supply a file name to remove." << endl;
        return (EXIT_FAILURE);
    }

    if (remove(argv[1]) == -1) {  // remove( ) returns -1 on error
        cerr << "Error: " << strerror(errno) << endl;
        return (EXIT_FAILURE);
    } else {
        cout << "File '" << argv[1] << "' removed." << endl;
    }
}