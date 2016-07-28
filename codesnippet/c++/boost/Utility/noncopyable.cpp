#include <boost/utility.hpp>
#include <boost/noncopyable.hpp>
#include <string>
#include <iostream>


class Test : boost::noncopyable {
public :
  Test(int b):a(b) {
  }
  int get() { return a; }
private :
  int a;
};

int main(int argc, char *argv[])
{
  Test a(1);
  Test b(2);
  //a = b;
  //Test c(b);
  //
  return 0;
}
