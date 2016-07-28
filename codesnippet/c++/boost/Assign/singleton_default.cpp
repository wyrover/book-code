#include <boost/pool/details/singleton_default.hpp>
#include <iostream>

using namespace std;

class Point {
public :
  Point(int a, int b, int c):x(a), y(b), z(c) { 
    cout << "constructor" << endl;
  }
  int getX() { return x; } 
  int getY() { return y; } 
  int getZ() { return z; } 
private :
  int x;
  int y;
  int z;
};

int main(int argc, char *argv[])
{
  typedef boost::details::pool::singleton_default<Point> point;
  point::instance() 

  return 0;
}
