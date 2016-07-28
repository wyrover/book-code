#include <string>
#include <vector>
#include <deque>
#include <list>
#include <iostream>
#include <boost/foreach.hpp>

using namespace std;

int main()
{
  string hello( "Hello, world!" );
  BOOST_FOREACH( char ch, hello )
  {
      cout << ch;
  }
  cout << endl;

  list<int> list_int;
  list_int.push_back(1);
  list_int.push_back(2);
  list_int.push_back(3);
  list_int.push_back(4);
  BOOST_FOREACH( int i, list_int )
  {
    cout << i << "\t";    
  }
  cout << endl;

  short array_short[] = {1,2,3};
  BOOST_FOREACH( int i, array_short )
  {
    cout << i << "\t";
  }  
  cout << endl;

  std::deque<int> deque_int;
  deque_int.push_back(1);
  deque_int.push_back(1);
  deque_int.push_back(2);
  BOOST_FOREACH( int i, deque_int)
  {
      if( i == 0 ) return 0;
      if( i == 1 ) continue;
      if( i == 2 ) break;
  }

  std::vector<float> fvector;
  fvector.push_back(1.0);
  BOOST_FOREACH(float f, fvector)
  {
     cout << f ;
  }  
  cout << endl;

  BOOST_REVERSE_FOREACH( int i, list_int )
  {
    cout << i << "\t"; 
  }
  cout << endl;

  return 0;
}
