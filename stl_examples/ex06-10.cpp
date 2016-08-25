// Demonstrating the STL list push_back and push_front functions.
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
  list<char> list1 = lst("Bjarne Stroustrup"), list2;
  list<char>::iterator i;

  cout << "Demonstrating list push_back function" << endl;
  for (i = list1.begin(); i != list1.end(); ++i)
    list2.push_back(*i);

  assert(list1 == list2);

  list1 = lst("Bjarne Stroustrup");
  list2 = lst("");
   
  cout << "Demonstrating list push_front function" << endl;
  for (i = list1.begin(); i != list1.end(); ++i)
    list2.push_front(*i);

  assert(list2 == lst("purtsuortS enrajB"));

  // Show that list2 is the reverse of list1, by using
  // STL generic reverse function to reverse list1
  reverse(list1.begin(), list1.end());

  assert(list2 == list1);
}
