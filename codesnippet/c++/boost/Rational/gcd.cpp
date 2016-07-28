#include <boost/rational.hpp>

using namespace std;

int main(int argc, char *argv[])
{
  cout << boost::gcd(3,4) << endl; 
  cout << boost::lcm(2,6) << endl;

  return 0;
}
