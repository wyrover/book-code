// Illustrating the generic set operations.
#include <vector.h>
#include <algo.h>
#include <assert.h>

int main()
{
  bool result;

  // Initialize a vectors of characters:
  vector<char> vecChar1(5);
  vector<char> vecChar2(5);
  char elem1[5] = { 'a', 'b', 'c', 'd', 'e' };
  char elem2[5] = { 'a', 'e', 'i', 'o', 'u' };

  for(int i = 0; i < 5; i++) vecChar1[i] = elem1[i];
  for(i = 0; i < 5; i++) vecChar2[i] = elem2[i];

  // Illustrate includes:
  result = includes(vecChar1.begin(), vecChar1.end(),
        vecChar2.begin(), vecChar2.end());
  assert(result == false);

  // Illustrate set_union():
  vector<char> setUnion(8);
  set_union(vecChar1.begin(), vecChar1.end(),
        vecChar2.begin(), vecChar2.end(),
        setUnion.begin());

  assert(setUnion[0] == 'a');
  assert(setUnion[1] == 'b');
  assert(setUnion[2] == 'c');
  assert(setUnion[3] == 'd');
  assert(setUnion[4] == 'e');
  assert(setUnion[5] == 'i');
  assert(setUnion[6] == 'o');
  assert(setUnion[7] == 'u');

  // Illustrate set_intersection:
  vector<char> setIntersection(2);
  set_intersection(vecChar1.begin(), vecChar1.end(),
        vecChar2.begin(), vecChar2.end(),
        setIntersection.begin());

  assert(setIntersection[0] == 'a');
  assert(setIntersection[1] == 'e');

  // Illustrate set_symmetric_difference:
  vector<char> setDiff(6);

  set_symmetric_difference(vecChar1.begin(),
        vecChar1.end(),
        vecChar2.begin(), vecChar2.end(),
        setDiff.begin());

  assert(setDiff[0] == 'b');
  assert(setDiff[1] == 'c');
  assert(setDiff[2] == 'd');
  assert(setDiff[3] == 'i');
  assert(setDiff[4] == 'o');
  assert(setDiff[5] == 'u');
}
