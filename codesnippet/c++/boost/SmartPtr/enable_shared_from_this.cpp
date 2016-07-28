#include <boost/enable_shared_from_this.hpp>
#include <boost/make_shared.hpp>
//#include <iostream>
//#include <string>

using namespace std;

class Test : public boost::enable_shared_from_this<Test> {
public :
   Test(int b) : a(b) { }
   int get() { return a; }
   ~Test() { cout << "destroyed" << endl; }
private :
   int a;
};

int main(int argc, char *argv[])
{
  boost::shared_ptr<Test> sp = boost::make_shared<Test>(100);
  // Note : you can't get the this pointer from a normal object, you must use it from a shared pointer.
  boost::shared_ptr<Test> sp1 = sp->shared_from_this();
  cout << sp1->get() << endl;
  cout << sp.use_count() << endl;

  return 0;
}
