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
#include <iostream>
#include <string>
#include <memory>    // for unique_ptr
#include <dirent.h>  // for opendir(), ...
#include <cstring>   // for strerror()
#include <cerrno>    // for errno
using namespace std;

class DirCloser
{
public:
    void operator()(DIR* dp)
    {
        if (closedir(dp) != 0) {
            std::cerr << "OOPS: closedir() failed" << std::endl;
        }
    }
};

int main()
{
    // open current directory:
    unique_ptr<DIR, DirCloser> pDir(opendir("."));
    // process each directory entry:
    struct dirent *dp;

    while ((dp = readdir(pDir.get())) != nullptr) {
        string filename(dp->d_name);
        cout << "process " << filename << endl;
        //...
    }
}
