// Program: Test Score
// Purpose: To demonstrate the use of algorithm
// with respect to a vector of test scores

#include <algorithm>  // If you want to use an 
// algorithm this is the header used.
#include <numeric>  // (For Accumulate)
#include <vector>
#include <iostream>
using namespace std;

int testscore[] = {67, 56, 24, 78, 99, 87, 56};

// predicate that evaluates a passed test
bool passed_test(int n)
{
    return (n >= 60);
}

// predicate that evaluates a failed test
bool failed_test(int n)
{
    return (n < 60);
}

int main(int argc, char* argv[])
{
    int total;
    // Initialize a vector with the data in the testscore array
    vector <int> vecTestScore(testscore,
                              testscore + sizeof(testscore) / sizeof(int));
    vector <int>::iterator vi;
    // Sort and display the vector
    sort(vecTestScore.begin(), vecTestScore.end());
    cout << "Sorted Test Scores:" << endl;

    for (vi = vecTestScore.begin(); vi != vecTestScore.end(); vi++) {
        cout << *vi << ", ";
    }

    cout << endl;
    // Display statistics
    // min_element returns an _iterator_ to the
    // element that is the minimum value in the range
    // Therefor * operator must be used to extract the value
    vi = min_element(vecTestScore.begin(), vecTestScore.end());
    cout << "The lowest score was " << *vi << "." << endl;
    // Same with max_element
    vi = max_element(vecTestScore.begin(), vecTestScore.end());
    cout << "The highest score was " << *vi << "." << endl;
    // Use a predicate function to determine the number who passed
    cout << count_if(vecTestScore.begin(), vecTestScore.end(), passed_test) <<
         " out of " << vecTestScore.size() <<
         " students passed the test" << endl;
    // and who failed
    cout << count_if(vecTestScore.begin(),
                     vecTestScore.end(), failed_test) <<
         " out of " << vecTestScore.size() <<
         " students failed the test" << endl;
    // Sum the scores
    total = accumulate(vecTestScore.begin(),
                       vecTestScore.end(), 0);
    // Then display the Average
    cout << "Average score was " <<
         (total / (int)(vecTestScore.size())) << endl;
    return 0;
}