#include <iostream>
#include <cmath> // for fabs( )

using namespace std;

bool doubleEquals(double left, double right, double epsilon)
{
    return (fabs(left - right) < epsilon);
}

bool doubleLess(double left, double right, double epsilon,
                bool orequal = false)
{
    if (fabs(left - right) < epsilon) {
        // Within epsilon, so considered equal
        return (orequal);
    }

    return (left < right);
}

bool doubleGreater(double left, double right, double epsilon,
                   bool orequal = false)
{
    if (fabs(left - right) < epsilon) {
        // Within epsilon, so considered equal
        return (orequal);
    }

    return (left > right);
}

int main()
{
    double first = 0.33333333;
    double second = 1.0 / 3.0;
    cout << first << endl;
    cout << second << endl;
    // Straight equalify test. Fails when you wouldn't want it to.
    // (boolalpha prints booleans as "true" or "false")
    cout << boolalpha << (first == second) << endl;
    // New equality. Passes as scientific app probably wants.
    cout << doubleEquals(first, second, .0001) << endl;
    // New less-than
    cout << doubleLess(first, second, .0001) << endl;
    // New Greater-than
    cout << doubleGreater(first, second, .0001) << endl;
    // New less-than-or-equal-to
    cout << doubleLess(first, second, .0001, true) << endl;
    // New greater-than-or-equal-to
    cout << doubleGreater(first, second, .0001, true) << endl;
}
