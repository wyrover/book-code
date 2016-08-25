#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

using namespace std;
const static char PAD_CHAR = '.';

// addMargins takes two streams and two numbers. The streams are for
// input and output. The first of the two numbers represents the
// left margin width (i.e., the number of spaces to insert at the
// beginning of every line in the file). The second number represents
// the total line width to pad to.
void addMargins(istream& in, ostream& out,
                int left, int right)
{
    string tmp;

    while (!in.eof()) {
        getline(in, tmp, '\n');                     // getline is defined
        // in <string>
        tmp.insert(tmp.begin(), left, PAD_CHAR);
        rpad(tmp, right, PAD_CHAR);                 // rpad from Recipe
        // 4.2
        out << tmp << '\n';
    }
}

int main(int argc, char** argv)
{
    if (argc < 3)
        return (EXIT_FAILURE);

    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in || !out)
        return (EXIT_FAILURE);

    int left = 8;
    int right = 72;

    if (argc == 5) {
        left = atoi(argv[3]);
        right = atoi(argv[4]);
    }

    addMargins(in, out, left, right);
    out.close();

    if (out)
        return (EXIT_SUCCESS);
    else
        return (EXIT_FAILURE);
}
