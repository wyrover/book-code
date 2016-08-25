// Finding all anagram groups in a given dictionary of words.
#include <iterator.h>
#include <algo.h>
#include <map.h>
#include <multimap.h>
#include <list.h>
#include <fstream.h>
#include <bstring.h>
#include <assert.h>
 
typedef multimap<string, string, less<string> > multimap_1;
 
typedef multimap_1::value_type PS;
 
typedef multimap_1::const_iterator PSi;
 
struct PPS : pair<PSi, PSi> {
  PPS() : pair<PSi, PSi>(PSi(), PSi()) { }
  PPS(PSi p, PSi q) : pair<PSi, PSi>(p, q) { }
};
 
// Work-around for compiler bug: this is not called in this
// program, but is defined to give a warning and 
// abort if it ever is.
bool operator<(const PSi& x, const PSi& y) 
{ 
  cout << "Program attempted to use operator< on multiset "
    << "iterators; aborting." << endl;
  exit(1);
  return false;
}

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
  // a multimap:
  typedef istream_iterator<string, ptrdiff_t> 
          string_input;
  multimap_1 word_pairs;
 
  for (string_input in(ifs); in != string_input(); ++in) {
    vector<char> vstring = *in;
    sort(vstring.begin(), vstring.end());
    word_pairs.insert(PS(string(vstring), *in));
  }
 
  cout << "\nSearching " << word_pairs.size() 
    << " words for anagram groups..." << flush;
 
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
    // group, or to the end of the multimap:
    j = adjacent_find(j, finis,
                      not2(word_pairs.value_comp()));
    if (j == finis) break;
    
    // Find the end of the anagram group that begins at j:
    k = find_if(j, finis, 
                bind1st(word_pairs.value_comp(), *j));
 
    multimap_1::size_type n = 0;
    distance(j, k, n);
    if (n > 1)
      // Save the positions j and k delimiting the anagram
      // group in the list of groups of size n:
      groups[n].push_back(PPS(j, k));
 
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
      for (; j != k; ++j)
        cout << (*j).second << " ";
      cout << endl;
    }
    cout << endl;
  }
}
