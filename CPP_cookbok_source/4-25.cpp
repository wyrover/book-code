#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <cctype>
#include <functional>

using namespace std;

void textWrap(istream& in, ostream& out, size_t width)
{
    string tmp;
    char cur = '\0';
    char last = '\0';
    size_t i = 0;

    while (in.get(cur)) {
        if (++i == width) {
            ltrimws(tmp);                  // ltrim as in Recipe
            out << '\n' << tmp;            // 4.1
            i = tmp.length();
            tmp.clear();
        } else if (isspace(cur) &&   // This is the end of
                   !isspace(last)) { // a word
            out << tmp;
            tmp.clear();
        }

        tmp += cur;
        last = cur;
    }
}

int main(int argc, char** argv)
{
    if (argc < 3)
        return (EXIT_FAILURE);

    int w = 72;
    ifstream in(argv[1]);
    ofstream out(argv[2]);

    if (!in || !out)
        return (EXIT_FAILURE);

    if (argc == 4)
        w = atoi(argv[3]);

    textWrap(in, out, w);
    out.close();

    if (out)
        return (EXIT_SUCCESS);
    else
        return (EXIT_FAILURE);
}
