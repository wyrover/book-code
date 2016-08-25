// Illustrating the generic generate algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>

class calc_square {
  int i;
 public:
  calc_square(): i(0) {}
  int operator()() { ++i; return i * i; }
};

int main()
{
  vector<int> vector1(10);
  
  // Fill vector1 with 1, 4, 9, 16, ..., 100
  generate(vector1.begin(), vector1.end(), calc_square());

  for (int j = 0; j < 10; ++j)
    assert(vector1[j] == (j+1)*(j+1));
}
