// Demonstrating the STL list splice functions.
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
   cout << "Demonstrating STL splice functions." << endl;
   list<char> list1, list2, list3;
   list<char>::iterator i, j, k;
   
   // Example of splice(iterator, list<char>&)
   list1 = lst("There is something about science.");
   list2 = lst("fascinating ");
   i = find(list1.begin(), list1.end(), 'a');
   list1.splice(i, list2);
   assert(list1 == 
       lst("There is something fascinating about science."));
   assert(list2 == lst(""));

   // Example of splice(iterator, list<char>&, iterator)
   list1 = 
         lst("One gets such wholesale return of conjecture");
   list2 = 
         lst("out of ssuch a trifling investment of fact.");
   list3 = lst(" of");
   i = search(list1.begin(), list1.end(), list3.begin(),
              list3.end());
   // i points to the blank before "of";
   j = find(list2.begin(), list2.end(), 's');
   list1.splice(i, list2, j);
   assert(list1 == 
       lst("One gets such wholesale returns of conjecture"));
   assert(list2 == 
       lst("out of such a trifling investment of fact."));

   // Example of splice(iterator, list<char>&, iterator,
   //                   iterator)
   list1 = lst("Mark Twain");
   list2 = lst(" --- ");
   j = find(list2.begin(), list2.end(), ' ');  
   k = find(++j, list2.end(), ' ');  // Find second blank.
   list1.splice(list1.begin(), list2, j, k);// Move the ---.
   assert(list1 == lst("---Mark Twain"));
   assert(list2 == lst("  "));
}
