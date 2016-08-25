// Illustrating the generic adjacent_find algorithm.
#include <algo.h>
#include <bstring.h>
#include <deque.h>
#include <function.h>
#include <assert.h>

int main()
{
  deque<string> player(5, string());
  deque<string>::iterator diter;

  // Initialize the deque:
  player[0] = "Pele";
  player[1] = "Platini";
  player[2] = "Maradona";
  player[3] = "Maradona";
  player[4] = "Rossi";

  // Find the first pair of equal consecutive names:
  diter = adjacent_find(player.begin(), player.end());

  assert(*diter == "Maradona" && *(diter+1) == "Maradona");

  // Find the first name that is lexicographically
  // greater than the following name:
  diter = adjacent_find(player.begin(), player.end(), 
        greater<string>());

  assert(*diter == "Platini" && *(diter+1) == "Maradona");
}
