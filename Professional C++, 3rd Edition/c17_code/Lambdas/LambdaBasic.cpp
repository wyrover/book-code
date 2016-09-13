#include <iostream>

using namespace std;

int main()
{
    // Basic lambda
    auto basicLambda = [] { cout << "Hello from Lambda" << endl; };
    basicLambda();
    // Lambda with parameters
    auto parametersLambda = [](int value) {
        cout << "The value is " << value << endl;
    };
    parametersLambda(42);
    // Lambda returning a value
    auto returningLambda = [](int a, int b) -> int { return a + b; };
    int sum = returningLambda(11, 22);
    cout << "The result is " << sum << endl;
    // Lambda returning a value, with return type omitted
    auto returningLambda2 = [](int a, int b) {
        return a + b;
    };
    sum = returningLambda2(11, 22);
    cout << "The result is " << sum << endl;
    // Lambda capturing a variable by value
    double data = 1.23;
    auto capturingVLambda = [data] { cout << "Data = " << data << endl; };
    capturingVLambda();
    // Lambda capturing a variable by value
    auto capturingVLambda2 = [data]() mutable { data *= 2; cout << "Data = " << data << endl; };
    capturingVLambda2();
    // Lambda capturing a variable by reference
    auto capturingRLambda = [&data] { data *= 2; };
    capturingRLambda();
    cout << "Data = " << data << endl;
    return 0;
}
