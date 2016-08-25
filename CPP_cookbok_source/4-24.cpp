#include <iostream>
#include <istream>
#include <ostream>
#include <fstream>
#include <cstdlib>

using namespace std;

void spacesToTabs(istream& in, ostream& out, int spaceLimit)
{
    int consecSpaces = 0;
    char c;

    while (in.get(c)) {
        if (c != ' ') {
            if (consecSpaces > 0) {
                for (int i = 0; i < consecSpaces; i++) {
                    out.put(' ');
                }

                consecSpaces = 0;
            }

            out.put(c);
        } else {
            if (++consecSpaces == spaceLimit) {
                out.put('\t');
                consecSpaces = 0;
            }
        }
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

    spacesToTabs(in, out, 3);
    out.close();

    if (out)
        return (EXIT_SUCCESS);
    else
        return (EXIT_FAILURE);
}
