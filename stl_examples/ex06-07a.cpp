// Demonstrating the STL vector front and erase operations.
// This is the modification of Example 6-7 discussed on p. 131.
#include <iostream.h>
#include <vector.h>
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
  cout << "Demonstrating STL vector front "
       << "and erase operations." << endl;
  vector<char> vector1 = vec("abcdefghij");
  
  cout << "Popping characters off the front produces: "; 

  while (vector1.size() > 0) {
    cout << vector1.front();
    vector1.erase(vector1.begin());
  }


   cout << endl;
}
