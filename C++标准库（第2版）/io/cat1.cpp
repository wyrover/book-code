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
// header files for file I/O
#include <fstream>
#include <iostream>
using namespace std;

// for all filenames passed as command-line arguments
// - open, print contents, and close file
int main(int argc, char* argv[])
{
    ifstream file;

    // for all command-line arguments
    for (int i = 1; i < argc; ++i) {
        // open file
        file.open(argv[i]);
        // write file contents to cout
        char c;

        while (file.get(c)) {
            cout.put(c);
        }

        // close file
        file.close();
    }
}
