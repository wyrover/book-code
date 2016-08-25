// Illustrating the generic find_if algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>

// Define a unary predicate object type: 
class GreaterThan50 {
 public:
  bool operator()(int x) const { return x > 50; }
};

int main()
{
  // Create a vector with values 0, 1, 4, 9, 16, ..., 144:
  vector<int> vector1;
  for (int i = 0; i < 13; ++i)
    vector1.push_back(i * i);

  vector<int>::iterator where;
  where = find_if(vector1.begin(), vector1.end(),
                  GreaterThan50());

  assert(*where == 64);
}
