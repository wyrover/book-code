// Ex9_05.cpp
// Copying file contents using stream iterators
#include <iostream>                                      // For standard streams
#include <fstream>                                       // For file streams
#include <iterator>                                      // For iterators and begin() and end()
#include <string>                                        // For string class
using std::string;

int main()
{
    string file_in {"G:/Beginning_STL/dictionary.txt"};
    std::ifstream in {file_in};

    if (!in) {
        std::cerr << file_in << " not open." << std::endl;
        exit(1);
    }

    string file_out {"G:/Beginning_STL/dictionary_copy.txt"};
    std::ofstream out {file_out};
    std::copy(std::istream_iterator<string> {in}, std::istream_iterator<string> {}, std::ostream_iterator<string> {out, " "});
    in.clear();                                              // Clear EOF
    std::cout << "Original file length: " << in.tellg() << std::endl;
    std::cout << "File copy length: " << out.tellp() << std::endl;
    in.close();
    out.close();
}
