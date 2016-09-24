/* The following code example is taken from the book
 * "The C++ Standard Library - A Tutorial and Reference, 2nd Edition"
 * by Nicolai M. Josuttis, Addison-Wesley, 2012
 *
 * (C) Copyright Nicolai M. Josuttis 2012.
 * Permission to copy, use, modify, sell and distribute this software
 * is granted provided this copyright notice appears in all copies.
 * This software is provided "as is" without express or implied
 * warranty, and with no claim as to its suitability for any purpose.
 */
#include <iterator>
#include <algorithm>
#include <fstream>
#include <iostream>

int countLines(std::istream& in)
{
    return std::count(std::istreambuf_iterator<char>(in),
                      std::istreambuf_iterator<char>(),
                      '\n');
}

int main(int argc, char* argv[])
{
    int count;

    if (argc == 1) {
        // no argument => count lines of standard input
        count = countLines(std::cin);
    } else {
        // count number of lines of all files passed as argument
        std::ifstream in;
        count = 0;

        for (int i = 1; i < argc; ++i) {
            std::ifstream file(argv[i]);

            if (!file) {
                std::cerr << "failed to open " << argv[i] << "\n";
            } else {
                count += countLines(file);
            }
        }
    }

    std::cout << count << std::endl;
}
