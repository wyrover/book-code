// Demonstrating the STL vector push_back and insert functions.
#include <iostream.h>
#include <vector.h>
#include <algo.h>
#include <assert.h>

vector<char> vec(char* s)
  // Return vector<char> containing the characters of s
  // (not including the terminating null).
{
  vector<char> x;
  while (*s != '\0') 
    x.push_back(*s++);
  return x;
}
  
int main() 
{
  cout << "Demonstrating vector push_back function" << endl;

  vector<char> vector1 = vec("Bjarne Stroustrup"), vector2;
  vector<char>::iterator i;

  for (i = vector1.begin(); i != vector1.end(); ++i)
    vector2.push_back(*i);
  assert(vector1 == vector2);

  vector1 = vec("Bjarne Stroustrup");
  vector2 = vec("");
   
  cout << "Demonstrating vector insertion at beginning" 
       << endl;

  for (i = vector1.begin(); i != vector1.end(); ++i)
    vector2.insert(vector2.begin(), *i);
  assert(vector2 == vec("purtsuortS enrajB"));

  // Show that vector2 is the reverse of vector1, by using
  // STL generic reverse function to reverse vector1.
  reverse(vector1.begin(), vector1.end());
  assert(vector2 == vector1);
}      
