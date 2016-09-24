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
#include <iostream>
#include <string>

class germanBoolNames : public std::numpunct_byname<char>
{
public:
    germanBoolNames(const std::string& name)
        : std::numpunct_byname<char>(name)
    {
    }
protected:
    virtual std::string do_truename() const
    {
        return "wahr";
    }
    virtual std::string do_falsename() const
    {
        return "falsch";
    }
};

int main()
{
    std::locale loc(std::locale(""), new germanBoolNames(""));
    std::cout.imbue(loc);
    std::cout << std::boolalpha << true << std::endl;
}
