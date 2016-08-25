// Illustrating the generic merge algorithms.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  // Initialize vector of integers: 
  vector<int> vecInt1(5);
  vector<int> vecInt2(5);
  vector<int> vecInt3(10);

  for (int i = 0; i < 5; i++) 
    vecInt1[i] = 2 * i;
  
  for (i = 0; i < 5; i++)
    vecInt2[i] = 1 + 2 * i;

  // Merge contents of vecInt1 and vecInt2,
  // putting result in vecInt3:
  merge(vecInt1.begin(), vecInt1.end(),
        vecInt2.begin(), vecInt2.end(),
        vecInt3.begin());
  
  for (i = 0; i < 10; i++)
    assert(vecInt3[i] == i);

  for (i = 0; i < 5; i++)
    vecInt3[i] = vecInt1[i];
  for (i = 0; i<5; i++)
    vecInt3[i + 5] = vecInt2[i];

  // Merge the two sorted halves of vecInt3
  // in place to obtain a sorted vecInt3:
  inplace_merge(vecInt3.begin(), vecInt3.begin() + 5,
                vecInt3.end());

  for (i = 0; i < 10; i++)
    assert(vecInt3[i] == i);
}
