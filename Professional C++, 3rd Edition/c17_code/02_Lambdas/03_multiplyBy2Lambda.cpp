#include <iostream>
#include <functional>

using namespace std;

function<int(void)> multiplyBy2Lambda(int x)
{
    return [x] { return 2 * x; };
}

// Using C++14 function return type deduction
//auto multiplyBy2Lambda(int x)
//{
//  return [x]{ return 2 * x; };
//}

int main()
{
    //function<int(void)> fn = multiplyBy2Lambda(5);
    //cout << fn() << endl;
    auto fn = multiplyBy2Lambda(5);
    cout << fn() << endl;
    return 0;
}