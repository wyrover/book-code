#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "utils.h"

using namespace std;
using namespace utils;

int main()
{
    vector<string> vec1, vec2;
    vec1.push_back("Charles");
    vec1.push_back("in");
    vec1.push_back("Charge");
    vec2.push_back("Charles");
    vec2.push_back("in");
    vec2.push_back("charge");  // Note the small "c"

    if (<userinput>equal < / userinput > (vec1.begin(), vec1.end(), vec2.begin())) {
        cout << "The two ranges are equal!" << endl;
    } else {
        cout << "The two ranges are NOT equal!" << endl;
    }

    string s1 = "abcde";
    string s2 = "abcdf";
    string s3 = "abc";
    cout << boolalpha  // Show bools as "true" or "false"
         << <userinput>lexicographical_compare < / userinput > (s1.begin(), s1.end(),
                 s1.begin(), s1.end()) << endl;
    cout << <userinput>lexicographical_compare < / userinput > (s1.begin(), s1.end(),
            s2.begin(), s2.end()) << endl;
    cout << <userinput>lexicographical_compare < / userinput > (s2.begin(), s2.end(),
            s1.begin(), s1.end()) << endl;
    cout << <userinput>lexicographical_compare < / userinput > (s1.begin(), s1.end(),
            s3.begin(), s3.end()) << endl;
    cout << <userinput>lexicographical_compare < / userinput > (s3.begin(), s3.end(),
            s1.begin(), s1.end()) << endl;
    pair<string::iterator, string::iterator> iters =
        <userinput>mismatch < / userinput > (s1.begin(), s1.end(), s2.begin());
    cout << "first mismatch  = " << *(iters.first) << endl;
    cout << "second mismatch = " << *(iters.second) << endl;
}
