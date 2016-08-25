#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
        return (EXIT_FAILURE);

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in || !out)
        return (EXIT_FAILURE);

    char c;

    while (in.get(c)) {
        if (c == '\t')
            out << "   "; // 3 spaces
        else
            out << c;
    }

    out.close();

    if (out)
        return (EXIT_SUCCESS);
    else
        return (EXIT_FAILURE);
}
