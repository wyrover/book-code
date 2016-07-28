// Listing 54-4. Caching the ctype Facet
#include <iomanip>
#include <iostream>
#include <locale>

#include "ioflags.hpp"  // from Listing 39-4


void print(int c, std::string const& name, std::locale loc)
{
  // Don't concern yourself with the & operator. I'll cover that later
  // in the book. Its purpose is just to ensure the character's escape
  // code is printed correctly.
  std::cout << "\\x" << std::setw(2) << (c & 0xff) <<
               " is " << name << " in " << loc.name() << '\n';
}

/// Test a character's categorization in the locale, @p loc.
void test(char c, std::locale loc)
{
  ioflags save{std::cout};

  std::ctype<char> const& ctype{std::use_facet<std::ctype<char>>(loc)};

  std::cout << std::hex << std::setfill('0');
  if (ctype.is(std::ctype_base::alnum, c))
    print(c, "alphanumeric", loc);
  else if (ctype.is(std::ctype_base::cntrl, c))
    print(c, "control", loc);
  else if (ctype.is(std::ctype_base::punct, c))
    print(c, "punctuation", loc);
  else
    print(c, "none of the above", loc);
}

int main()
{
  // Test the same code point in different locales and character sets.
  char c{'\xd7'};

  // ISO 8859-1 is also called Latin-1 and is widely used in Western Europe
  // and the Americas. It is often the default character set in these regions.
  // The country and language are unimportant for this test.
  // Choose any that support the ISO 8859-1 character set.
  test(c, std::locale{"en_US.iso88591"});

  // ISO 8859-5 is Cyrillic. It is often the default character set in Russia
  // and some Eastern European countries. Choose any language and region that
  // support the ISO 8859-5 character set.
  test(c, std::locale{"ru_RU.iso88595"});

  // ISO 8859-7 is Greek. Choose any language and region that
  // support the ISO 8859-7 character set.
  test(c, std::locale{"el_GR.iso88597"});

  // ISO 8859-8 contains some Hebrew. It is no longer widely used.
  // Choose any language and region that support the ISO 8859-8 character set.
  test(c, std::locale{"he_IL.iso88598"});
}
