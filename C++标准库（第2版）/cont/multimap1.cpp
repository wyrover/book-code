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
#include <map>
#include <string>
#include <iostream>
#include <iomanip>
using namespace std;

int main()
{
    // create multimap as string/string dictionary
    multimap<string, string> dict;
    // insert some elements in random order
    dict.insert({ {"day", "Tag"}, {"strange", "fremd"},
        {"car", "Auto"}, {"smart", "elegant"},
        {"trait", "Merkmal"}, {"strange", "seltsam"},
        {"smart", "raffiniert"}, {"smart", "klug"},
        {"clever", "raffiniert"}
    });
    // print all elements
    cout.setf(ios::left, ios::adjustfield);
    cout << ' ' << setw(10) << "english "
         << "german " << endl;
    cout << setfill('-') << setw(20) << ""
         << setfill(' ') << endl;

    for (const auto& elem : dict) {
        cout << ' ' << setw(10) << elem.first
             << elem.second << endl;
    }

    cout << endl;
    // print all values for key "smart"
    string word("smart");
    cout << word << ": " << endl;

    for (auto pos = dict.lower_bound(word);
         pos != dict.upper_bound(word);
         ++pos) {
        cout << "    " << pos->second << endl;
    }

    // print all keys for value "raffiniert"
    word = ("raffiniert");
    cout << word << ": " << endl;

    for (const auto& elem : dict) {
        if (elem.second == word) {
            cout << "    " << elem.first << endl;
        }
    }
}
