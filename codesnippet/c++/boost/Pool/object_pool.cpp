#include <boost/pool/object_pool.hpp>
#include <boost/utility.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>

using namespace std;

class Test {
public :
   Test(int a, int b): m_a(a), m_b(b) { }
   int getA() { return m_a; }
   int getB() { return m_b; }
private :
   int m_a;
   int m_b;
};

int main(int argc, char *argv[])
{
    boost::object_pool<Test>  opl;
    
    Test *pt = opl.malloc();

    assert(opl.is_from(pt)); 

    boost::object_pool<string> spl;
    for (int i = 0; i < 10; ++i) {
      string *ps = spl.construct("string" + boost::lexical_cast<string>(i));
      cout << *ps << endl; 
    }     
   
    return 0; 
}
