// Demonstrating the STL vector erase function.
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
  cout << "Demonstrating STL vector erase function." << endl;

  vector<char> vector1 = vec("remembering");
  vector<char>::iterator j;

  j = find(vector1.begin(), vector1.end(), 'm');

  // j now points to the first 'm'
  assert(*j == 'm' && *(j+1) == 'e');

  vector1.erase(j--); 
  assert(vector1 == vec("reembering"));

  // j now points to the first 'e'
  assert(*j == 'e' && *(j+1) == 'e');

  vector1.erase(j--); 
  assert(vector1 == vec("rembering"));
  assert(*j == 'r');

  // Erase first 3 characters:
  vector1.erase(j, j + 3);
  assert(vector1 == vec("bering"));

  vector1.erase(vector1.begin() + 1);
  assert(vector1 == vec("bring"));
}      
