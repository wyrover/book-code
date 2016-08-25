// Find all anagram groups in a dictionary, and print them to 
// standard output stream.
#include <iterator.h>
#include <algo.h>
#include <vector.h>
#include <fstream.h>
#include <bstring.h>
#include "ps.h" // Definitions of PS, firstLess, firstEqual

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

  // Output all of the anagram groups:
  cout << "\n\nThe anagram groups are: " << endl;
  vector<PS>::const_iterator j = word_pairs.begin(),
                             finis = word_pairs.end();
  while (true) {
    // Make j point to the next anagram 
    // group, or to the end of the vector:
    j = adjacent_find(j, finis, firstEqual);
    if (j == finis) break;
    
    // Find the end of the anagram group that begins at j:
    vector<PS>::const_iterator k =
      find_if(j + 1, finis, 
	      not1(bind1st(firstEqual, *j)));
    
    // Output the anagram group in positions j to k:
    cout << endl << "  ";
    copy(j, k, ostream_iterator<string>(cout, " "));  
    cout << endl;

    // Prepare to continue search at position k:
    j = k;
  }
}
