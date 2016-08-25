// Illustrating the generic partial_sum algorithm.
#include <algo.h>
#include <iostream.h>

int main()
{
  const int N = 20;
  int x1[N], x2[N];
  for (int i = 0; i != N; ++i)
    x1[i] = i;

  // Compute the partial sums of 0, 1, 2, 3, ..., N - 1,
  // putting the result in x2:
  partial_sum(&x1[0], &x1[N], &x2[0]);

  for (i = 0; i != N; ++i)
    cout << x2[i] << " ";
  cout << endl;
}
