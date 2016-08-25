// Demonstrating STL vector copying constructors.
#include <iostream.h>
#include <vector.h>
#include <assert.h>

int main() 
{
  cout << "Demonstrating STL vector copying constructors" 
       << endl;
  char* name = "George Foreman";
  vector<char> George(name, name + 6);

  vector<char> anotherGeorge(George.begin(), George.end());
  assert(anotherGeorge == George);

  vector<char> son1(George); // Uses copy constructor
  assert(son1 == anotherGeorge);

  vector<char> son2 = George; // Also uses copy constructor
  assert(son2 == anotherGeorge);
}
