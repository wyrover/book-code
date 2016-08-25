#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
        return (EXIT_FAILURE);

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in || !out)
        return (EXIT_FAILURE);

    string tmp;
    in >> tmp;          // Grab the first word
    out << tmp;         // Dump it to the output stream

    while (in >> tmp) { // operator>> ignores whitespace, so all I have
        out << ' ';      // to do is add a space and each chunk of non-
        out << tmp;      // whitespace
    }

    out.close();
}
