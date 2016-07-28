// Ex1_03.cpp
// Passing functions to an algorithm
#include <iostream>                         // For standard streams
#include <algorithm>                        // For transform()
#include <iterator>                         // For iterators
#include <functional>                       // For function

class Root
{
public:
    double operator()(double x)
    {
        return std::sqrt(x);
    };
};

int main()
{
    double data[] { 1.5, 2.5, 3.5, 4.5, 5.5};
    // Passing a function object
    Root root;                                // Function object
    std::cout << "Square roots are:" << std::endl;
    std::transform(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "), root);
    // Using an lambda expression as an argument
    std::cout << "\n\nCubes are:" << std::endl;
    std::transform(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "), [](double x) {
        return x * x * x;
    });
    // Using a variable of type std::function<> as argument
    std::function<double(double)> op {[](double x)
    {
        return x * x * x;
    }};
    std::cout << "\n\nSquares are:" << std::endl;
    std::transform(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "), op);
    // Using a lambda expression that calls another lambda expression as argument
    std::cout << "\n\n4th powers are:" << std::endl;
    std::transform(std::begin(data), std::end(data), std::ostream_iterator<double>(std::cout, " "), [&op](double x) {
        return op(x) * op(x);
    });
    std::cout << std::endl;
}