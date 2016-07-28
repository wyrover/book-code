//
//  OpenMpTest.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "OpenMpTest.h"

#include <iostream>

using std::cout;
using std::endl;

int main_openmp()
{

#pragma omp for schedule(dynamic, 5)
    for (int i=0; i<50; ++i)
    {
        cout << " do this " << endl;
    }

    return 0;
}