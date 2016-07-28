#include <boost/progress.hpp>
#include <iostream>
#include <sstream>

using namespace std;

int main(int argc, char *argv[])
{
  { 
	  boost::progress_timer timer;
  }
  stringstream ss; 
  {
	  boost::progress_timer t(ss); 
  }
  cout << ss.str() << endl;

  boost::progress_display display(100);

  for (int i = 0; i < 100; ++i) {
    ++display;
  }

  return 0;
}
