#include <boost/algorithm/string.hpp>
#include <string>
#include <vector>
#include <list>

using namespace std;

int main(int argc, char *argv[])
{
  vector<string> names; 
  string name_list = "jim,tom,lucy,lily";
  
  boost::split(names,name_list, boost::is_any_of(",")); 

  cout << "there are " << names.size() << " person" << endl;  

  return 0;
}
