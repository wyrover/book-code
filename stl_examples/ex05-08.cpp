// Illustrating the generic equal and mismatch algorithms.
#include <algo.h>
#include <bstring.h>
#include <list.h>
#include <deque.h>
#include <assert.h>

int main()
{
  list<string> driver_list;
  vector<string> vec;
  deque<string> deq;

  driver_list.insert(driver_list.end(), "Clark");
  driver_list.insert(driver_list.end(), "Rindt");
  driver_list.insert(driver_list.end(), "Senna");

  vec.insert(vec.end(), "Clark");
  vec.insert(vec.end(), "Rindt");
  vec.insert(vec.end(), "Senna");
  vec.insert(vec.end(), "Berger");

  deq.insert(deq.end(), "Clark");
  deq.insert(deq.end(), "Berger");

  // Show that driver_list and the first 3 elements of
  // vec are equal in all corresponding positions:
  assert(equal(driver_list.begin(), driver_list.end(),
               vec.begin()));

  // Show that deq and the first 2 elements of driver_list
  // are not equal in all corresponding positions:
  assert(!equal(deq.begin(), deq.end(),
                driver_list.begin()));

  // Find the corresponding positions in deq and driver_list
  // at which unequal elements first occur:
  pair<deque<string>::iterator, list<string>::iterator>
    pair1 = mismatch(deq.begin(), deq.end(),
                     driver_list.begin());

  if (pair1.first != deq.end())
    cout << "First disagreement in deq and driver_list:\n  "
         << *(pair1.first) << " and " << *(pair1.second)
         << endl;
}
