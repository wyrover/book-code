// Listing 14-2. Copying Integers from a Named File to a Named File
#include <cstdio>
#include <fstream>
#include <iostream>

int main()
{
  std::ifstream in{"list1402.in"};
  if (not in)
    std::perror("list1402.in");
  else
  {
    std::ofstream out{"list1402.out"};
    if (not out)
      std::perror("list1402.out");
    else
    {
      int x{};
      while (in >> x)
        out << x << '\n';
      out.close();
      in.close();
    }
  }
}
