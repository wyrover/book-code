// Program to find all anagrams of a given word, using a 
// dictionary read from a file.
#include <iterator.h>
#include <algo.h>
#include <vector.h>
#include <fstream.h>
#include <bstring.h>  

int main() 
{
  cout << "Anagram finding program:\n"
    << "finds all words in a dictionary that can\n"
    << "be formed with the letters of a given word.\n\n"
    << "First, enter the name of the file containing\n"
    << "the dictionary: " << flush;

  string dictionary_name; 
  cin >> dictionary_name;

  ifstream ifs(dictionary_name.c_str());
  cout << "\nReading the dictionary ..." << flush;

  // Copy words from dictionary file to a vector:
  typedef istream_iterator<string, ptrdiff_t> 
          string_input;
  vector<string> dictionary;
  copy(string_input(ifs), string_input(), 
       back_inserter(dictionary));

  cout << "\nThe dictionary contains " 
    << dictionary.size() << " words.\n\n";

  cout << "Now type a word (or any string of letters),\n"
    << "and I'll see if it has any anagrams: " << flush;

  // For each input word, check if it has any 
  // anagrams in the dictionary:
  for (string_input j(cin); j != string_input(); ++j) {

    // Put the input string into a vector so it 
    // can be sorted.
    vector<char> word = *j;

    // Start with letters of word in alphabetical order:
    sort(word.begin(), word.end());

    // Search dictionary for all permutations of word:
    bool found_one = false;
    do {
      if (binary_search(dictionary.begin(),
                        dictionary.end(), 
                        string(word))) {
        cout << "\n  " << string(word) << flush;
        found_one = true;
      }
    } while (next_permutation(word.begin(), word.end()));

    if (!found_one) 
      cout << "\n  Sorry, none found.";
    cout << "\n\nReady for another word: " << flush;
  }
}
