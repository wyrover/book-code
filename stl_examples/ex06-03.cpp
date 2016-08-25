// Demonstrating STL vector constructors with a user-defined type and 
// showing copying explicitly.
#include <iostream.h>
#include <vector.h>
#include <assert.h>

class U { 
public: 
  unsigned long id;
  unsigned long generation;
  static unsigned long total_copies;
  U() : id(0), generation(0) { }
  U(unsigned long n) : id(n), generation(0) { }
  U(const U& z) : id(z.id), generation(z.generation + 1) { 
    ++total_copies;
  } 
};

bool operator==(const U& x, const U& y) 
{ 
  return x.id == y.id; 
}

unsigned long U::total_copies = 0;

int main() 
{
  cout << "Demonstrating STL vector constructors with " 
       << "a user-defined type and showing copying "
       << "explicitly" << endl;
  vector<U> vector1, vector2(3);

  assert(vector1.size() == 0);
  assert(vector2.size() == 3);

  assert(vector2[0] == U() && vector2[1] == U() &&
         vector2[2] == U());

  for (int i = 0; i != 3; ++i)
    cout << "vector2[" << i << "].generation: " 
         << vector2[i].generation << endl;

  cout << "Total copies: " << U::total_copies << endl;
}
