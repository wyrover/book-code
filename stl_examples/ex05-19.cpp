// Illustrating the generic swap_ranges algorithm.
#include <algo.h>
#include <vector.h>
#include <assert.h>

int main()
{
  vector<char> vector1, vector2;

  char elem1[5] = {'H', 'E', 'L', 'L', 'O'};
  char elem2[5] = {'T', 'H', 'E', 'R', 'E'};

  // Initialize vectors:
  for (int i = 0; i < 5; i++) {
    vector1.push_back(elem1[i]);
    vector2.push_back(elem2[i]);
  }

  // Save vector1 and vector2 contents, for checking:
  vector<char> temp1 = vector1, temp2 = vector2;

  // Swap the contents of vector1 and vector2:
  swap_ranges(vector1.begin(), vector1.end(), 
              vector2.begin());

  assert(vector1 == temp2 && vector2 == temp1);
}
