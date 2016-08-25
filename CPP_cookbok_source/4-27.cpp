#include <iostream>
#include <fstream>
#include <map>
#include <string>

typedef std::map<std::string, int> StrIntMap;

void countWords(std::istream& in, StrIntMap& words)
{
    std::string s;

    while (in >> s) {
        ++words[s];
    }
}

int main(int argc, char** argv)
{
    if (argc < 2)
        return (EXIT_FAILURE);

    std::ifstream in(argv[1]);

    if (!in)
        exit(EXIT_FAILURE);

    StrIntMap w;
    countWords(in, w);

    for (StrIntMap::iterator p = w.begin();
         p != w.end(); ++p) {
        std::cout << p->first << " occurred "
                  << p->second << " times.\n";
    }
}
