// Ex9_05.cpp
// Using count_if() with stream iterators to count word frequencies
#include <iostream>                                        // For standard streams
#include <iterator>                                        // For iterators and begin() and end()
#include <iomanip>                                         // For stream manipulators
#include <fstream>                                         // For ifstream
#include <algorithm>                                       // For count_if()
#include <string>
using std::string;

int main()
{
    string file_in {"G:/Beginning_STL/dictionary.txt"};
    std::ifstream in {file_in};

    if (!in) {
        std::cerr << file_in << " not open." << std::endl;
        exit(1);
    }

    string letters {"abcdefghijklmnopqrstuvwxyz"};
    size_t perline(9);

    for (auto ch : letters) {
        std::cout << ch << ": "
                  << std::setw(5)
                  << std::count_if(std::istream_iterator<string> {in}, std::istream_iterator<string> {},
        [&ch](const string & s) {
            return s[0] == ch;
        })
                << (((ch - 'a' + 1) % perline) ? " " : "\n");
        in.clear();                                            // Clear EOF...
        in.seekg(0);                                           // ... and back to the beginning
    }

    std::cout << std::endl;
}