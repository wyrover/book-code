#include <boost/pool/singleton_pool.hpp>
#include <iostream>

using namespace std;

struct pool_tag1 { };
struct pool_tag2 { };

typedef boost::singleton_pool<pool_tag1, sizeof(int)> ipl;
typedef boost::singleton_pool<pool_tag2, sizeof(float)> fpl;

int main(int argc, char *argv[])
{
  int *p = (int *) ipl::malloc();  
  float *pf = (float *)fpl::malloc();

  return 0;
}
