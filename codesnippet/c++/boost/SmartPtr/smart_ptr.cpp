#include <boost/smart_ptr.hpp>
#include <boost/make_shared.hpp>
#include <vector>
#include <string>

using namespace std;

class Test {
public :
  int get();
  void set(int b) {
    a = b;
  }
private :
  int a;
};

void shutdown(Test *t) {
  cout << "destroy object t" << endl;
}

int main(int argc,char *argv[])
{
  // scoped_ptr 
  /* 
   * scoped_ptr is similar with the auto_ptr of stl. but scoped_ptr is noncopyable.
   * that is you can't change the ptr of scoped_ptr
   * reset() is not recommended to use
   */
  boost::scoped_ptr<string> sp(new string("test"));  
  cout << *sp << endl;
  cout << sp->size() << endl;

  int *intPtr = new int(10);
  boost::scoped_ptr<int> sp1(intPtr);
  assert(sp1.get() == intPtr); 

  // scoped_array
  /*
   * scoped_array is for new [],
   *
   */  
  boost::scoped_array<int> sp2(new int[100]);
  for (int i = 0; i < 100; ++i) {
   sp2[i] = i;
  }

  // shared_ptr
  /*
   * shared_ptr is a smart ptr which supports reference count
   *
   */   
  boost::shared_ptr<int> sp3(new int);
  *sp3 = 10;
   
  boost::shared_ptr<int> sp4 = sp3;

  assert(!sp3.unique());
  assert(sp3 == sp4);
  sp3.reset();
  assert(!sp3);
  assert(sp4.unique());
  cout << "use cound " << sp4.use_count() << endl;

  boost::shared_ptr<string> sp5 = boost::make_shared<string>("test"); 
  cout << *sp5 << endl;

  boost::shared_ptr<vector<int> > sp6 = boost::make_shared<vector<int> >(100, 0);
  assert(sp6->size() == 100);

  boost::shared_ptr<Test> sp7(new Test(), shutdown);

  // weak_ptr
  boost::shared_ptr<int> sp8(new int(5));
  assert(sp8.use_count() == 1);

  boost::weak_ptr<int> sp9(sp8);
  assert(sp9.use_count() == 1);
  assert(sp9.expired() == false);
  if (!sp9.expired()) {
    boost::shared_ptr<int> sp10 = sp9.lock();
    *sp10 = 10;
    cout << sp8.use_count() << endl;
  }
  
  cout << *sp8 << endl;
  sp8.reset();
  assert(sp9.expired() == true);

  return 0;
}
