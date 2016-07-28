// Listing 17-1. Working and Playing with Characters
#include <iostream>

int main()
{
  int value{};
  bool have_value{false};
  char ch{};

  while (std::cin.get(ch))
  {
    if (ch >= '0' and ch <= '9')
    {
      value = ch - '0';
      have_value = true;
      while (std::cin.get(ch) and ch >= '0' and ch <= '9')
        value = value * 10 + ch - '0';

    }
    if (ch == '\n')
    {
      if (have_value)
      {
        std::cout << value << '\n';
        have_value = false;
      }
    }
    else if (ch != ' ' and ch != '\t')
    {
      std::cout << '\a';
      have_value = false;

      while (std::cin.get(ch) and ch != '\n')
        /*empty*/;
    }
  }
}
