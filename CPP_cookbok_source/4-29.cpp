#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;

int main(int argc, char** argv)
{
    if (argc < 3)
        return (EXIT_FAILURE);

    ifstream in(argv[1]);
    ofstream out(argv[2]);
    int w = 72;

    if (argc == 4)
        w = atoi(argv[3]);

    string tmp;
    out.setf(ios_base::right);   // Tell the stream to

    // right-justify
    while (!in.eof()) {
        out.width(w);                  // Reset width after
        getline(in, tmp, '\n');        // each write
        out << tmp << '\n';
    }

    out.close();
}
