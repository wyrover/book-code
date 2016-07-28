#include <boost/timer.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  boost::timer timer;

  cout << "max elapsed time : " << timer.elapsed_max( ) << "seconds" << endl;
  cout << "min elapsed time : " << timer.elapsed_min() << "seconds" << endl;
  cout << "current elapsed time : " << timer.elapsed() << "seconds" << endl;
   
  return 0;
}
