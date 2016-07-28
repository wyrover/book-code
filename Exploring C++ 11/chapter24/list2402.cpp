// Listing 24-2. Overloading Functions in the Manner of a Library-Writer
#include <algorithm>
#include <iostream>
#include <locale>

bool is_alpha(char ch)
{
  return std::isalpha(ch, std::locale{});
}

bool is_alpha(std::string const& str)
{
  for (char ch : str)
    if (not is_alpha(ch))
      return false;
  return true;
}

char to_lower(char ch)
{
  return std::tolower(ch, std::locale{});
}

std::string to_lower(std::string str)
{
  for (char& ch : str)
    ch = to_lower(ch);
  return str;
}
  
char to_upper(char ch)
{
  return std::toupper(ch, std::locale{});
}

std::string to_upper(std::string str)
{
  for (char& ch : str)
    ch = to_upper(ch);
  return str;
}

int main()
{
  std::string str{};
  while (std::cin >> str)
  {
    if (is_alpha(str))
      std::cout << "alpha\n";
    else
      std::cout << "not alpha\n";
    std::cout << "lower: " << to_lower(str) << "\nupper: " << to_upper(str) << '\n';
  }
}
