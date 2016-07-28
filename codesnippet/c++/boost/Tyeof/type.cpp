#include <boost/typeof/typeof.hpp>
#include <iostream>
using namespace std;

/*
#include BOOST_TYPEOF_INCREMENT_REGISTRATION_GROUP()
namespace ex {
  struct Test {
     int x;
     int y;
  };
}
BOOST_TYPEOF_REGISTIER_TYPE(ex::Test)
*/

int main(int argc, char *argv[])
{

  BOOST_TYPEOF(2) a = 10;
  BOOST_AUTO(b, 2*3.0);

  cout << a << " " << b << endl;
  cout << typeid(a).name() << endl;
  cout << typeid(b).name() << endl;  

  //BOOST_ATUO(c, ex::Test);   
  //cout << typeid(c).name() << endl;

  return 0;
}
