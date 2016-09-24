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
#include <sstream>

template <typename charT, typename traits>
inline
std::basic_ostream<charT, traits>&
operator << (std::basic_ostream<charT, traits>& strm,
             const Fraction& f)
{
    // string stream
    // - with same format
    // - without special field width
    std::basic_ostringstream<charT, traits> s;
    s.copyfmt(strm);
    s.width(0);
    // fill string stream
    s << f.numerator() << '/' << f.denominator();
    // print string stream
    strm << s.str();
    return strm;
}
