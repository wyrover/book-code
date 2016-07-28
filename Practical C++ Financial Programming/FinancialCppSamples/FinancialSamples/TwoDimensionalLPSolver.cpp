//
//  TwoDimensionalLPSolver.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "TwoDimensionalLPSolver.h"

#include "Matrix.h"
#include "LPSolver.h"

#include <iostream>

using std::vector;
using std::cout;
using std::endl;

TwoDimensionalLPSolver::TwoDimensionalLPSolver(const Vector &c, const Vector &A1,
                                               const Vector &A2, const Vector &b)
: m_c(c),
  m_A1(A1),
  m_A2(A2),
  m_b(b)
{
}

TwoDimensionalLPSolver::TwoDimensionalLPSolver(const TwoDimensionalLPSolver &p)
: m_c(p.m_c),
  m_A1(p.m_A1),
  m_A2(p.m_A2),
  m_b(p.m_b)
{
}

TwoDimensionalLPSolver::~TwoDimensionalLPSolver()
{
}

TwoDimensionalLPSolver &TwoDimensionalLPSolver::operator=(const TwoDimensionalLPSolver &p)
{
    if (this != &p)
    {
        m_c = p.m_c;
        m_A1 = p.m_A1;
        m_A2 = p.m_A2;
        m_b = p.m_b;
    }
    return *this;
}

bool TwoDimensionalLPSolver::solveProblem(Vector &res, double &objVal)
{
    int size = m_b.size();
    Matrix A(size, 2);
    for (int j=0; j<size; ++j)
    {
        A[j][0] = m_A1[j];
        A[j][1] = m_A2[j];
    }
    LPSolver solver(A, m_b, m_c);
    solver.setMaximization();
    return solver.solve(res, objVal) == LPSolver::ResultType::FEASIBLE;
}

int main_tds()
{
    vector<double> A1 = { -2.1, 3.1, 7.9};
    vector<double> A2 = { 1, 1, 1 };
    vector<double> c = { 5.3, 7.1 };
    vector<double> b = { 3.4, 4.3, 6 };
    TwoDimensionalLPSolver solver(c, A1, A2, b);

    vector<double> results;
    double objVal;
    solver.solveProblem(results, objVal);
    cout << "objVal : " << objVal << endl;
    for (int i=0; i<results.size(); ++i)
    {
        cout << " x" << i << ": " << results[i];
    }
    cout << endl;
    return 0;
}
