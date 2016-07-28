// Listing 17-3. Finding the Points with the Largest x and y Values
#include <algorithm>
#include <iostream>
#include <limits>
#include <vector>

int main()
{
  typedef std::vector<int>  intvec;
  typedef intvec::iterator  iterator;

  intvec xs, ys;        // store the xs and ys

  { // local block to keep I/O variables local
    int x{}, y{};      // variables for I/O
    char sep{};
    // Loop while the input stream has an integer (x), a character (sep),
    // and another integer (y); then test that the separator is a comma.
    while (std::cin >> x >> sep and sep == ',' and std::cin >> y)
    {
      xs.push_back(x);
      ys.push_back(y);
    }
  }

  for (iterator x{xs.begin()}, y{ys.begin()}; x != xs.end(); ++x, ++y)
    std::cout << *x << ',' << *y << '\n';
}
