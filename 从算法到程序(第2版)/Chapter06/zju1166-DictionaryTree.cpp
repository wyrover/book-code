#include <stdio.h>
#include <iostream>
#include <set>
using namespace std;

#define maxWord 2001
#define maxChar 20

struct CDicRec {
    set<char> alpha;
    string lex;
} lexicon[maxWord], thisWord;

struct CHold {
    int nIndex;
    int nProfile[26];
} hold[201];

int prof[26];
int thisProfile[26];
int nWords, nHolds;
int anagram[50];
int NumAnags;
string thisString;
bool found;

bool inclusion(set<char> s1, set<char> s2)
{
    char i;
    bool flag = false;

    for (i = 'A'; i <= 'Z'; i++)
        if (s1.find(i) != s1.end())
            if (s2.find(i) != s2.end()) flag = true;
            else return false;

    return flag;
}

bool ReadWord()
{
    int i;
    string s;
    char ch[50];
    gets(ch);
    s = ch;

    if (s.find('#') != -1) return false;
    else {
        thisString = s;
        thisWord.alpha.clear();
        thisWord.lex = "";

        for (i = 0; i < s.size(); i++)
            if (s[i] >= 'A' && s[i] <= 'Z') {
                thisWord.alpha.insert(s[i]);
                thisWord.lex += s[i];
            }

        return true;
    }
}

void RecordAnagram()
{
    int i;
    int num;
    bool equal;
    string anags[21], phrase_words[21], s;

    for (i = 1; i <= NumAnags; i++)
        anags[i] = lexicon[anagram[i - 1]].lex;

    num = 1;
    s = thisString;
    int p = s.find(' ');

    while (p >= 0) {
        phrase_words[num++] = s.substr(0, p);
        s = s.substr(p + 1, s.size() - 1);
        p = s.find(' ');
    }

    phrase_words[num] = s;
    equal = true;

    if (num != NumAnags) equal = false;
    else {
        phrase_words[0] = " ";

        for (i = 2; i <= num; i++) {
            s = phrase_words[i];
            p = i - 1;

            while (s < phrase_words[p]) {
                phrase_words[p + 1] = phrase_words[p];
                p--;
            }

            phrase_words[p + 1] = s;
        }

        for (i = 1; i <= num; i++)
            if (anags[i] != phrase_words[i]) equal = false;
    }

    if (!equal) {
        printf("%s =", thisString.c_str());

        for (i = 0; i < NumAnags; i++)
            printf(" %s", lexicon[anagram[i]].lex.c_str());

        printf("\n");
    }
}

void FindAnagram(int k, int thisprofile[])
{
    int i, j;
    bool good;
    int count = 0;
    anagram[NumAnags++] = hold[k].nIndex;

    for (i = 0; i < 26; i++) {
        thisprofile[i] -= hold[k].nProfile[i];
        count += (thisprofile[i] > 0 ? 1 : 0);
    }

    if (count == 0) RecordAnagram();
    else {
        for (i = k + 1; i < nHolds; i++) {
            good = true;

            for (j = 0; j < 26; j++)
                if (hold[i].nProfile[j] > thisprofile[j]) good = false;

            if (good) FindAnagram(i, thisprofile);
        }
    }

    NumAnags--;

    for (i = 0; i < 26; i++)
        thisprofile[i] += hold[k].nProfile[i];
}

void solve()
{
    int i, j;
    nHolds = 0;

    for (i = 0; i < 26; i++)
        thisProfile[i] = 0;

    for (i = 0; i < thisWord.lex.size(); i++)
        thisProfile[thisWord.lex[i] - 'A']++;

    for (i = 0; i < nWords; i++)
        if (inclusion(lexicon[i].alpha, thisWord.alpha)) {
            hold[nHolds].nIndex = i;

            for (j = 0; j < 26; j++)
                hold[nHolds].nProfile[j] = 0;

            for (j = 0; j < lexicon[i].lex.size(); j++)
                hold[nHolds].nProfile[lexicon[i].lex[j] - 'A']++;

            nHolds++;
        }

    NumAnags = 0;

    for (i = 0; i < nHolds; i++)
        FindAnagram(i, thisProfile);
}

int main()
{
    nWords = 0;

    while (ReadWord())
        lexicon[nWords++] = thisWord;

    while (ReadWord()) solve();

    return 0;
}
