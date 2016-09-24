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
#include <streambuf>
#include <locale>
#include <cstdio>

template <typename charT,
          typename traits = std::char_traits<charT> >
class basic_outbuf : public std::basic_streambuf<charT, traits>
{
protected:
    // central output function
    // - print characters in uppercase mode
    virtual typename traits::int_type
    overflow(typename traits::int_type c)
    {
        if (!traits::eq_int_type(c, traits::eof())) {
            // convert lowercase to uppercase
            c = std::toupper(c, this->getloc());
            // convert the character into a char (default: '?')
            char cc = std::use_facet<std::ctype<charT>>
                      (this->getloc()).narrow(c, '?');

            // and write the character to the standard output
            if (std::putchar(cc) == EOF) {
                return traits::eof();
            }
        }

        return traits::not_eof(c);
    }
};

typedef basic_outbuf<char>    outbuf;
typedef basic_outbuf<wchar_t> woutbuf;
