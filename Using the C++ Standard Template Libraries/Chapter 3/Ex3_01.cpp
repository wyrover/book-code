// Ex3_01.cpp
// A simple calculator using stack containers

#include <cmath>                                          // For pow() function
#include <iostream>                                       // For standard streams
#include <stack>                                          // For stack<T> container
#include <algorithm>                                      // For remove()
#include <stdexcept>                                      // For runtime_error exception
#include <string>                                         // For string class
using std::string;

// Returns value for operator precedence
inline size_t precedence(const char op)
{
    if (op == '+' || op == '-')
        return 1;

    if (op == '*' || op == '/')
        return 2;

    if (op == '^')
        return 3;

    throw std::runtime_error {string {"invalid operator in precedence() function: "} + op};
}

// Execute an operation
double execute(std::stack<char>& ops, std::stack<double>& operands)
{
    double result {};
    double rhs {operands.top()};                            // Get rhs...
    operands.pop();                                         // ...and delete from stack
    double lhs {operands.top()};                            // Get lhs...
    operands.pop();                                         // ...and delete from stack

    switch (ops.top()) {                                    // Execute current op
    case '+':
        result = lhs + rhs;
        break;

    case '-':
        result = lhs - rhs;
        break;

    case '*':
        result = lhs * rhs;
        break;

    case '/':
        result = lhs / rhs;
        break;

    case '^':
        result = std::pow(lhs, rhs);
        break;

    default:
        throw std::runtime_error {string{"invalid operator in execute() function: "} + ops.top()};
    }

    ops.pop();                                              // Delete op just executed
    operands.push(result);
    return result;
}

int main()
{
    std::stack<double> operands;                            // Push-down stack of operands
    std::stack<char> operators;                             // Push-down stack of operators
    string exp;                                             // Expression to be evaluated
    std::cout << "An arithmetic expression can include the operators +, -, *, /, and ^ for exponentiation." << std::endl;

    try {
        while (true) {
            std::cout << "Enter an arithmetic expression and press Enter - enter an empty line to end:" << std::endl;
            std::getline(std::cin, exp, '\n');

            if (exp.empty()) break;

            // Remove spaces
            exp.erase(std::remove(std::begin(exp), std::end(exp), ' '), std::end(exp));
            size_t index {};                                    // Index to expression string
            // Every expression must start with a numerical operand
            operands.push(std::stod(exp, &index));              // Push the first (lhs) operand on the stack

            while (true) {
                operators.push(exp[index++]);                     // Push the operator on to the stack
                // Get rhs operand
                size_t i {};                                      // Index to substring
                operands.push(std::stod(exp.substr(index), &i));  // Push rhs operand
                index += i;                                       // Increment expression index

                if (index == exp.length()) {                      // If we are at end of exp...
                    while (!operators.empty())                      // ...execute outstanding ops
                        execute(operators, operands);

                    break;
                }

                // If we reach here, there's another op...
                // If there's a previous op of equal or higher precedence execute it
                while (!operators.empty() && precedence(exp[index]) <= precedence(operators.top()))
                    execute(operators, operands);                   //  Execute previous op.
            }

            std::cout << "result = " << operands.top() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
    }

    std::cout << "Calculator ending..." << std::endl;
}
