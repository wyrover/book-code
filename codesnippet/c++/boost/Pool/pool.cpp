#include <boost/pool/pool.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  boost::pool<> pl(sizeof(int));
  int *p = (int *)pl.malloc();
  *p = 10;
 
  int *pp = (int *)pl.ordered_malloc(10);  
  for (int i = 0; i < 10; ++i) {
    pp[i] = i;
  }
  
  for (int i = 0; i < 10; ++i) {
    cout << pp[i] << endl;
  }


  return 0;
}
