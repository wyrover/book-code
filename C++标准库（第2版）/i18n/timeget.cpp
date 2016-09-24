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
#include <locale>
#include <ctime>
#include <iterator>
#include <iostream>
#include <string>
#include <exception>
#include <cstdlib>
using namespace std;

int main()
{
    try {
        // use German locale:
#ifdef _MSC_VER
        locale locG("deu_deu.1252");
#else
        locale locG("de_DE.ISO-8859-1");
#endif
        const time_get<char>& tgG = use_facet<time_get<char>>(locG);
        // print date order of German locale:
        typedef time_base TB;
        time_get<char>::dateorder d = tgG.date_order();
        cout << "dateorder: "
             << (d == TB::no_order || d == TB::mdy ? "mdy" :
                 d == TB::dmy ? "dmy" :
                 d == TB::ymd ? "ymd" :
                 d == TB::ydm ? "ydm" : "unknown") << endl;
        // read weekday (in German) and time (hh::mm))
        cout << "<wochentag> <hh>:<mm>: ";
        string format = "%A %H:%M";
        struct tm val;
        ios_base::iostate err = ios_base::goodbit;
        tgG.get(istreambuf_iterator<char>(cin),
                istreambuf_iterator<char>(),
                cin, err, &val,
                format.c_str(), format.c_str() + format.size());

        if (err != ios_base::goodbit) {
            cerr << "invalid format" << endl;
        }
    } catch (const std::exception& e) {
        cerr << "Exception: " << e.what() << endl;
        return EXIT_FAILURE;
    }
}
