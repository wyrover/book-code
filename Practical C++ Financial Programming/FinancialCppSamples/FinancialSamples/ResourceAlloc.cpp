//
//  ResourceAlloc.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "ResourceAlloc.h"

#include "MIPSolver.h"
#include "Matrix.h"

#include <iostream>

using std::vector;
using std::cout;
using std::endl;

ResourceAlloc::ResourceAlloc(vector<double> &result, double &objVal)
: m_results(result),
  m_objVal(objVal)
{
}

ResourceAlloc::ResourceAlloc(const ResourceAlloc &p)
: m_results(p.m_results),
  m_objVal(p.m_objVal)
{
}

ResourceAlloc::~ResourceAlloc()
{
}

ResourceAlloc &ResourceAlloc::operator=(const ResourceAlloc &p)
{
    if (this != &p)
    {
        m_results = p.m_results;
        m_objVal = p.m_objVal;
    }
    return  *this;
}

void ResourceAlloc::solveProblem()
{
    static const double cost_array[][5] = {
        // Years:
        // 1    2   3     4    5
        {1.81, 2.4,  2.5, 0.97, 1.5},  // proj 1
        {1.29, 1.8,  2.3, 0.56, 0.5},  // proj 2
        {1.22, 1.2,  0.1, 0.48, 0 },  // proj 3
        {1.43, 1.4,  1.2, 1.2, 1.2},  // proj 4
        {1.62, 1.9,  2.5, 2.0, 1.8},  // proj 5
    };

    Matrix costs(5,5);  // cost matrix
    for (int i=0; i<5; ++i) {
        for (int j=0; j<5; ++j) {
            costs[j][i] = cost_array[i][j];
        }
    }

    vector<double> returns = {12.13, 3.95, 7.2, 4.21, 11.39};  // investment returns
    vector<double> budgets = {5.1, 6.4, 6.84, 4.5, 3.8};       // annual budgets
    MIPSolver solver(costs, budgets, returns);
    solver.setMaximization();

    for (int i=0; i<5; ++i)
    {
        solver.setColBinary(i);
    }

    // --- solve the problem
    solver.solve(m_results, m_objVal);
}

int main_ra()
{

    vector<double> result;
    double objVal;

    ResourceAlloc ra(result, objVal);
    ra.solveProblem();
    cout << " optimum: " << objVal ;
    for (int i=0; i<result.size(); ++i)
    {
        cout << " x" << i << ": " << result[i];
    }
    cout << endl;
    return 0;
}
