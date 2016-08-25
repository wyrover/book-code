// Illustrating the generic search algorithm.
#include <algo.h>
#include <vector.h>
#include <deque.h>
#include <assert.h>

int main()
{
  vector<int> vec(20);
  deque<int>  deq(5);
  // Initialize vector with 0, 1, ..., 19:
  for (int i = 0; i < 20; ++i) 
    vec[i] = i;

  // Initialize deque deq with 5, 6, 7, 8, 9:
  for (i = 0; i < 5; ++i) 
    deq[i] = i + 5;

  // Search for first occurrence of the deque contents 
  // as a subsequence of the vector contents:
  vector<int>::iterator viter = 
    search(vec.begin(), vec.end(), deq.begin(), deq.end());

  for (i = 0; i < 5; ++i) 
    assert(*(viter + i) == i + 5);
}
