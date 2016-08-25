// Illustrating the generic for_each algorithm.
#include <algo.h>
#include <bstring.h>
#include <list.h>
#include <iostream.h>

void print_list(string s)
{
  cout << s << endl;
}

int main()
{
  list<string> dlist;
  dlist.insert(dlist.end(), "Clark");
  dlist.insert(dlist.end(), "Rindt");
  dlist.insert(dlist.end(), "Senna");

  // Print out each list element.
  for_each(dlist.begin(), dlist.end(), print_list);
}
