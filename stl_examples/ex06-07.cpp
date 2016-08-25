// Demonstrating the STL vector back and pop_back operations.
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
  cout << "Demonstrating STL vector back "
       << "and pop_back operations." << endl;
  vector<char> vector1 = vec("abcdefghij");
  
  cout << "Popping characters off the back produces: "; 
  
  while (vector1.size() > 0) {
    cout << vector1.back();
    vector1.pop_back();
  }
  cout << endl;
}
