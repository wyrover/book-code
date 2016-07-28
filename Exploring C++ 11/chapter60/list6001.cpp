// Listing 60-1. Exposing How Strings Are Copied
#include <iostream>
#include <string>
#include <vector>

class mystring : public std::string
{
public:
   mystring() : std::string{} { std::cout << "mystring()\n"; }
   mystring(mystring const& copy) : std::string(copy) {
      std::cout << "mystring copy(\"" << *this << "\")\n";
   }
};

std::vector<mystring> read_data()
{
   std::vector<mystring> strings{};
   mystring line{};
   while (std::getline(std::cin, line))
      strings.push_back(line);
   return strings;
}

int main()
{
   std::vector<mystring> strings{};
   strings = read_data();
}
