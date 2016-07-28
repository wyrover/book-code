// Ex9_09.cpp
// Copying a file using stream buffer iterators
#include <iostream>                                        // For standard streams
#include <iterator>                                        // For iterators and begin() and end()
#include <fstream>                                         // For file streams
#include <string>                                          // For string class
using std::string;

int main()
{
    string file_name {"G:/Beginning_STL/dictionary.txt"};
    std::ifstream file_in {file_name};

    if (!file_in) {
        std::cerr << file_name << " not open." << std::endl;
        exit(1);
    }

    string file_copy {"G:/Beginning_STL/dictionary_copy.txt"};
    std::ofstream file_out {file_copy, std::ios_base::out | std::ios_base::trunc};
    std::istreambuf_iterator<char> in {file_in};                        // Input stream buffer iterator
    std::istreambuf_iterator<char> end_in;                              // End of stream buffer iterator
    std::ostreambuf_iterator<char> out {file_out};                      // Output stream buffer iterator

    while (in != end_in)
        out = *in++;                                                      // Copy character from in to out

    std::cout << "File copy completed." << std::endl;
    file_in.close();                                                    // Close the file
    file_out.close();
}