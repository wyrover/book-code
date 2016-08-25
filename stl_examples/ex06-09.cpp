// Demonstrating the STL deque push_back and push_front functions.
#include <iostream.h>
#include <deque.h>
#include <algo.h>
#include <assert.h>

deque<char> deq(char* s)
  // Return deque<char> containing the characters of s 
  // (not including the terminating null).
{
  deque<char> x;
  while (*s != '\0') 
    x.push_back(*s++);
  return x;
}
  
int main() 
{
  deque<char> deque1 = deq("Bjarne Stroustrup"), deque2;
  deque<char>::iterator i;

  cout << "Demonstrating deque push_back function" << endl;
  for (i = deque1.begin(); i != deque1.end(); ++i)
    deque2.push_back(*i);
  
  assert(deque1 == deque2);
  
  deque1 = deq("Bjarne Stroustrup");
  deque2 = deq("");
   
  cout << "Demonstrating deque push_front function" << endl;
  for (i = deque1.begin(); i != deque1.end(); ++i)
    deque2.push_front(*i);
  
  assert(deque2 == deq("purtsuortS enrajB"));

  // Show that deque2 is the reverse of deque1, by using
  // STL generic reverse function to reverse deque1
  reverse(deque1.begin(), deque1.end());
  
  assert(deque2 == deque1);
}      

