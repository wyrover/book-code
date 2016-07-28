// Listing 54-2. Exploring Character Sets and Locales
#include <iomanip>
#include <iostream>
#include <locale>
#include <ostream>

#include "ioflags.hpp"  // from Listing 39-4


/// Print a character's categorization in a locale.
void print(int c, std::string const& name, std::locale loc)
{
  // Don't concern yourself with the & operator. I'll cover that later
  // in the book, in Exploration 63. Its purpose is just to ensure
  // the character's escape code is printed correctly.
  std::cout << "\\x" << std::setw(2) << (c & 0xff) <<
               " is " << name << " in " << loc.name() << '\n';
}

/// Test a character's categorization in the locale, @p loc.
void test(char c, std::locale loc)
{
  ioflags save{std::cout};
  std::cout << std::hex << std::setfill('0');
  if (std::isalnum(c, loc))
    print(c, "alphanumeric", loc);
  else if (std::iscntrl(c, loc))
    print(c, "control", loc);
  else if (std::ispunct(c, loc))
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

  // ISO 8859-8 contains some Hebrew. The character set is no longer widely used.
  // Choose any language and region that support the ISO 8859-8 character set.
  test(c, std::locale{"he_IL.iso88598"});
}
