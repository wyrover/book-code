#include <iostream>
#include <fstream>
#include <cstdio>
#include <string>

int main()
{
    char* pFileName = NULL;
    pFileName = tmpnam(NULL);
    // Right here is where another program may get the same temp
    // filename.

    if (!pFileName) {
        std::cerr << "Couldn't create temp file name.\n";
        return (EXIT_FAILURE);
    }

    std::cout << "The temp file name is: " << pFileName << '\n';
    std::ofstream of(pFileName);

    if (of) {
        of << "Here is some temp data.";
        of.close();
    }

    std::ifstream ifs(pFileName);
    std::string s;

    if (ifs) {
        ifs >> s;
        std::cout << "Just read in \"" << s << "\"\n";
        ifs.close();
    }
}