// Listing 60-4. Examining Expression Categories
#include <iostream>
#include <utility>

void print(std::string&& move)
{
   std::cout << "move: " << std::move(move) << '\n';
}

void print(std::string const& copy)
{
   std::cout << "copy: " << copy << '\n';
}

int main()
{
   std::string a{"a"}, b{"b"}, c{"c"};

  print(a);
  print(a + b);
  print(std::move(a));
  print(std::move(a + b));
  print(a + std::move(b));
  print(a + b + c);
} 
