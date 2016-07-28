// Listing 14-3. Copying Integers, with Minimal Error-Checking
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
      if (not out)
        std::perror("list1402.out");
    }
  }
}
