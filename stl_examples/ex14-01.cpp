// Finding all anagram groups in order of decreasing size.
#include <iterator.h>
#include <algo.h>
#include <vector.h>
#include <map.h>
#include <list.h>
#include <fstream.h>
#include <bstring.h>
#include "ps.h" // Definitions of PS, firstLess, firstEqual
 
typedef vector<PS>::const_iterator PSi;
 
struct PPS : pair<PSi, PSi> {
  PPS() : pair<PSi, PSi>(PSi(), PSi()) { }
  PPS(PSi p, PSi q) : pair<PSi, PSi>(p, q) { }
};
 
int main() {
  cout << "Anagram group finding program:\n"
    << "finds all anagram groups in a dictionary.\n\n"
    << "First, enter the name of the file containing\n"
    << "the dictionary: " << flush;
 
  string dictionary_name; 
  cin >> dictionary_name;
 
  ifstream ifs(dictionary_name.c_str());
  cout << "\nReading the dictionary ..." << flush;
 
  // Copy words from dictionary file to 
  // a vector of PS objects:
  typedef istream_iterator<string, ptrdiff_t> 
          string_input;
  vector<PS> word_pairs;
  copy(string_input(ifs), string_input(), 
       back_inserter(word_pairs));
 
  cout << "\nSearching " << word_pairs.size() 
    << " words for anagram groups..." << flush;
 
  // Sort word_pairs, using the first member of
  // each pair as a basis for comparison. This
  // brings all anagram groups together:
  sort(word_pairs.begin(), word_pairs.end(), firstLess);
 
  // Set up the map from anagram group sizes to lists of
  // groups of that size.
  typedef map<int, list<PPS>, greater<int> > map_1;
  map_1 groups;
 
  // Find all of the anagram groups and save their 
  // positions in the groups map.
  cout << "\n\nThe anagram groups are: " << endl;
  PSi j = word_pairs.begin(), finis = word_pairs.end(), k;
  while (true) {
    // Make j point to the next anagram 
    // group, or to the end of the vector:
    j = adjacent_find(j, finis, firstEqual);
    if (j == finis) break;
    
    // Find the end of the anagram group that begins at j:
    k = find_if(j + 1, finis, 
                not1(bind1st(firstEqual, *j)));
    
    if (k-j > 1)
      // Save the positions j and k delimiting the anagram
      // group in the list of groups of size k-j:
      groups[k-j].push_back(PPS(j, k));
 
    // Prepare to continue search at position k:
    j = k;
  }
 
  // Iterate through the groups map to output the anagram
  // groups in order of decreasing size:
  map_1::const_iterator m;
  for (m = groups.begin(); m != groups.end(); ++m) {
 
    cout << "\n\nAnagram groups of size "
      << (*m).first << ":\n";
 
    list<PPS>::const_iterator l;
    for (l = (*m).second.begin();
         l != (*m).second.end(); ++l) {
      cout << "\n   ";
      j = (*l).first;  // Beginning of the anagram group 
      k = (*l).second; // End of the anagram group
      copy(j, k, ostream_iterator<string>(cout, " "));
      cout << endl;
    }
    cout << endl;
  }
} 
