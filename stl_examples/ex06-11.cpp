// Demonstrating the STL list erase function.
#include <iostream.h>
#include <list.h>
#include <algo.h>
#include <assert.h>
list<char> lst(char* s)
  // Return list<char> containing the characters of s
  // (not including the terminating null).
{
  list<char> x;
  while (*s != '\0') 
    x.push_back(*s++);
  return x;
} 
 
int main() 
{
  cout << "Demonstrating STL list erase function." << endl;
  list<char> list1 = lst("remembering");
  list<char>::iterator j;

  j = find(list1.begin(), list1.end(), 'i');

  list1.erase(j++); 
  assert(list1 == lst("rememberng"));
  
  // j now points to the 'n'.
  list1.erase(j++); 
  assert(list1 == lst("rememberg"));

  // j now points to the 'g'.
  list1.erase(j++); 
  assert(list1 == lst("remember"));
  list1.erase(list1.begin()); 
  assert(list1 == lst("emember"));

  list1.erase(list1.begin());
  assert(list1 == lst("member"));
}      
