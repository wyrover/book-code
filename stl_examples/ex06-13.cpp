// Demonstrating the STL list sort and unique functions.
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
   cout << "Demonstrating STL list sort and unique "
        << "functions." << endl;
   list<char> list1 = lst("Stroustrup");

   list1.sort();
   assert(list1 == lst("Soprrsttuu"));

   list1.unique();
   assert(list1 == lst("Soprstu"));
}      

