// Demonstrating generic reverse algorithm on a list.
#include <iostream.h>
#include <algo.h>
#include <list.h>
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
  cout << "Demonstrating generic reverse algorithm on a list" 
       << endl;

  list<char> list1 = lst("mark twain");

  reverse(list1.begin(), list1.end());

  assert(list1 == lst("niawt kram"));
}
