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
#include <string>
#include <vector>

// convert string to wstring
std::wstring to_wstring(const std::string& str,
                        const std::locale& loc = std::locale())
{
    std::vector<wchar_t> buf(str.size());
    std::use_facet<std::ctype<wchar_t>>(loc).widen(str.data(),
                                     str.data() + str.size(),
                                     buf.data());
    return std::wstring(buf.data(), buf.size());
}

// convert wstring to string with '?' as default character
std::string to_string(const std::wstring& str,
                      const std::locale& loc = std::locale())
{
    std::vector<char> buf(str.size());
    std::use_facet<std::ctype<wchar_t>>(loc).narrow(str.data(),
                                     str.data() + str.size(),
                                     '?', buf.data());
    return std::string(buf.data(), buf.size());
}
