//
//  FactorialTemplate.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "FactorialTemplate.h"

#include <iostream>

void showFactorial()
{
    std::cout << " Some factorial values: " << std::endl;
    std::cout << "fact(5)= " << Factorial<5>::result;
    std::cout << "fact(7)= " << Factorial<7>::result;
    std::cout << "fact(9)= " << Factorial<9>::result;
}