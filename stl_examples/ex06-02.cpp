// Demonstrating STL vector constructors with a user-defined type.
#include <iostream.h>
#include <vector.h>
#include <assert.h>

class U { 
public: 
  unsigned long id;
  U() : id(0) { }
  U(unsigned long x) : id(x) { }
};

bool operator==(const U& x, const U& y) 
{ 
  return x.id == y.id; 
}

int main() 
{
  cout << "Demonstrating STL vector constructors with " 
       << "a user-defined type." << endl;
  vector<U> vector1, vector2(3);
  assert(vector1.size() == 0);
  assert(vector2.size() == 3);
  assert(vector2[0] == U() && vector2[1] == U() &&
         vector2[2] == U());
  assert(vector2 == vector<U>(3, U()));
}
