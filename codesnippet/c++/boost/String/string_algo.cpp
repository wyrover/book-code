#include <boost/algorithm/string.hpp>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
  string str("readme.txt");   
  
  if (boost::ends_with(str, "txt")) {
    cout << str << " is a text file" << endl;
  }

  string path("/var/log/");
  if (boost::starts_with(path, "/")) {
    cout << path << " is a absolute path" << endl;
  } else {
    cout << path << " is a relative path" << endl;
  }

  // lower case and upper case 
  cout << boost::to_upper_copy(str) << endl; 
  cout << str << endl;

  boost::replace_first(str, "txt", "");
  cout << str << endl;
  
  string str1("HELLO WORLD!");
  cout << boost::to_lower_copy(str1) << endl;
  cout << str1 << endl;
  boost::to_lower(str1);
  cout << str1 << endl;
  boost::to_upper(str1);
  cout << str1 << endl;

  // 
  // contains, equals, lexicographical_compare, all
  string url("http://test.com:80/index.html"); 
  if (boost::contains(url, ":")) {
    cout << "url contains port" << endl;
  }

  string str4("test"), str5("TEST");   
  if (boost::equals(str4, str5)) {
    cout << str4 << " = " << str5 << endl;
  }

  if (boost::iequals(str4, str5)) {
    cout << str4 << " = " << str5 << endl;
  }

  string str6 = " abcd ";
  cout << boost::trim_copy(str6) << endl;
  cout << boost::trim_left_copy(str6) << endl;
  cout << boost::trim_right_copy(str6) << endl;

   
  return 0;
}

