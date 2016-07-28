#include <boost/assign/std/vector.hpp> 
#include <boost/assign/list_inserter.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/assert.hpp> 
#include <utility>
#include <string>
#include <deque>
#include <list>
#include <stack>
#include <map>

using namespace std;
using namespace boost::assign; 
// bring 'operator+=()' into scope

int main(int argc, char *argv[])
{
  vector<int> values;  
  values += 1,2,3,4,5,6,7,8,9; // insert values at the end of the container

  BOOST_ASSERT(values.size() == 9);
  BOOST_ASSERT(values[0] == 1);
  BOOST_ASSERT(values[8] == 9);

  map<string,int> months;  
  insert( months )
  ( "january",   31 )( "february", 28 )
  ( "march",     31 )( "april",    30 )
  ( "may",       31 )( "june",     30 )
  ( "july",      31 )( "august",   31 )
  ( "september", 30 )( "october",  31 )
  ( "november",  30 )( "december", 31 );
  BOOST_ASSERT( months.size() == 12 );
  BOOST_ASSERT( months["january"] == 31 );  

  typedef pair< string,string > str_pair;
  deque<str_pair> deq;
  push_front( deq )( "foo", "bar")( "boo", "far" ); 
  BOOST_ASSERT( deq.size() == 2 );
  BOOST_ASSERT( deq.front().first == "boo" );
  BOOST_ASSERT( deq.back().second == "bar" );

  deque<int> di;    
  push_front( di ) = 1,2,3,4,5,6,7,8,9;
  BOOST_ASSERT( di.size() == 9 );    
  BOOST_ASSERT( di[0] == 9 ); 

  const list<int> primes = list_of(2)(3)(5)(7)(11);
  BOOST_ASSERT( primes.size() == 5 );
  BOOST_ASSERT( primes.back() == 11 );
  BOOST_ASSERT( primes.front() == 2 );
  
  const stack<string> names = list_of( "Mr. Foo" )( "Mr. Bar")( "Mrs. FooBar" ).to_adapter();
  const stack<string> names2 = (list_of( "Mr. Foo" ), "Mr. Bar", "Mrs. FooBar" ).to_adapter();
  BOOST_ASSERT( names.size() == 3 );
  BOOST_ASSERT( names.top() == "Mrs. FooBar");

  map<int,int> next = map_list_of(1,2)(2,3)(3,4)(4,5)(5,6);
    BOOST_ASSERT( next.size() == 5 );
    BOOST_ASSERT( next[ 1 ] == 2 );
    BOOST_ASSERT( next[ 5 ] == 6 );
    
  //  or we can use 'list_of()' by specifying what type
  //       the list consists of
  next = list_of< pair<int,int> >(6,7)(7,8)(8,9);
  BOOST_ASSERT( next.size() == 3 );
  BOOST_ASSERT( next[ 6 ] == 7 );
  BOOST_ASSERT( next[ 8 ] == 9 );  

  typedef boost::tuple<int,std::string,int> tuple;

  vector<tuple> v = tuple_list_of( 1, "foo", 2 )( 3, "bar", 4 );
  BOOST_CHECK( v.size() == 2 );
  BOOST_CHECK( boost::get<0>( v[1] ) ==  3 );

  vector<int> v;
  v += 1,2,3,repeat(10,4),5,6,7,8,9;
  v = [1,2,3,4,4,4,4,4,4,4,4,4,4,5,6,7,8,9]
  BOOST_ASSERT( v.size() == 3 + 10 + 5 );
          
  v = list_of(1).repeat(5,2)(3);
  v = [1,2,2,2,2,2,3]
  BOOST_ASSERT( v.size() == 1 + 5 + 1 );
                          
  push_back( v )(1).repeat(1,2)(3);
  // v = old v + [1,2,3]
  BOOST_ASSERT( v.size() == 10 );

  return 0;
}
