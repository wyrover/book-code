#include <boost/utility.hpp>
#include <boost/swap.hpp>
#include <algorithm>

using namespace std;

int main(int argc, char *argv[])
{
  int a[10];
  int b[10];

  std::fill_n(a, 10, 5);
  std::fill_n(b, 10, 5);

  boost::swap(a, b);

  return 0;
}
