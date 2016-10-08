#include <iostream>
#include <memory.h>
#include <vector>
#include <algorithm>
using namespace std;

string lexicon[2000];
string word[20];
string phrase;
string oldphrase;
int n;
int m;
int letters[26];

void findAnagram(int index, vector<string> &anagram)
{
    bool find = true;

    for (int i = 0; i < 26; i++)
        if (letters[i] > 0) {
            find = false;
            break;
        }

    if (find) {
        string s;

        for (int i = 0; i < anagram.size(); i++) {
            if (i > 0) s += " ";

            s += anagram[i];
        }

        if (s != phrase) cout << oldphrase << " = " << s << endl;

        return;
    }

    int alpha[26];

    for (int i = index; i < n; i++) {
        memset(alpha, 0, sizeof(alpha));
        bool more = true;

        for (int j = 0; j < lexicon[i].length(); j++) {
            int a = lexicon[i][j] - 'A';
            alpha[a]++;

            if (alpha[a] > letters[a]) {
                more = false;
                break;
            }
        }

        if (more) {
            for (int j = 0; j < 26; j++)
                letters[j] -= alpha[j];

            anagram.push_back(lexicon[i]);
            findAnagram(i, anagram);
            anagram.pop_back();

            for (int j = 0; j < 26; j++)
                letters[j] += alpha[j];
        }
    }
}

int main()
{
    n = 0;

    while (cin >> lexicon[n], lexicon[n] != "#") n++;

    string s;
    m = 0;

    while (true) {
        cin >> word[m];

        if ("#" == word[m++]) break;

        if (cin.get() == '\n') {
            oldphrase = "";

            for (int i = 0; i < m; i++) {
                if (i) oldphrase += " ";

                oldphrase += word[i];
            }

            sort(word, word + m);
            phrase = "";

            for (int i = 0; i < m; i++) {
                if (i) phrase += " ";

                phrase += word[i];
            }

            memset(letters, 0, sizeof(letters));

            for (int i = 0; i < phrase.length(); i++) {
                if (phrase[i] == ' ') continue;

                letters[phrase[i] - 'A']++;
            }

            vector<string> anagram;
            findAnagram(0, anagram);
            m = 0;
        }
    }
}
