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
#include <string>
#include <regex>
#include <iostream>
using namespace std;

int main()
{
    // case-insensitive find LaTeX index entries
    string pat1 = R"(\\.*index\{([^}]*)\})";       // first capture group
    string pat2 = R"(\\.*index\{(.*)\}\{(.*)\})";  // 2nd and 3rd capture group
    regex pat(pat1 + "\n" + pat2,
              regex_constants::egrep | regex_constants::icase);
    // initialize string with characters from standard input:
    string data((istreambuf_iterator<char>(cin)),
                istreambuf_iterator<char>());
    // search and print matching index entries:
    smatch m;
    auto pos = data.cbegin();
    auto end = data.cend();

    for (; regex_search(pos, end, m, pat); pos = m.suffix().first) {
        cout << "match: " << m.str() << endl;
        cout << "  val: " << m.str(1) + m.str(2) << endl;
        cout << "  see: " << m.str(3) << endl;
    }
}
