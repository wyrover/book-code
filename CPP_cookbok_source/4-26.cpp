#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cctype>

using namespace std;

void countStuff(istream& in,
                int& chars,
                int& words,
                int& lines)
{
    char cur = '\0';
    char last = '\0';
    chars = words = lines = 0;

    while (in.get(cur)) {
        if (cur == '\n' ||
            (cur == '\f' && last == '\r'))
            lines++;
        else
            chars++;

        if (!std::isalnum(cur) &&   // This is the end of a
            std::isalnum(last))     // word
            words++;

        last = cur;
    }

    if (chars > 0) {               // Adjust word and line
        if (std::isalnum(last))     // counts for special
            words++;                 // case

        lines++;
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
        return (EXIT_FAILURE);

    ifstream in(argv[1]);

    if (!in)
        exit(EXIT_FAILURE);

    int c, w, l;
    countStuff(in, c, w, l);
    1
    cout << "chars: " << c << '\n';
    cout << "words: " << w << '\n';
    cout << "lines: " << l << '\n';
}
