//
//  MIPSolver.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "MIPSolver.h"

#include "Matrix.h"

#include <glpk.h>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

MIPSolver::MIPSolver(Matrix &A, const std::vector<double> &b, const std::vector<double> &c)
: LPSolver(A, b, c)
{
}

MIPSolver::MIPSolver(const MIPSolver &p)
: LPSolver(p)
{
}

MIPSolver::~MIPSolver()
{
}

MIPSolver &MIPSolver::operator=(const MIPSolver &p)
{
    return *this;
}

void MIPSolver::setColInteger(int colNum)
{
    glp_set_col_kind(getLP(), colNum+1, GLP_IV);
}

void MIPSolver::setColBinary(int colNum)
{
    glp_set_col_kind(getLP(), colNum+1, GLP_BV);
}

LPSolver::ResultType MIPSolver::solve(vector<double> &result, double &objValue)
{
    glp_simplex(getLP(), NULL);
    int res = glp_intopt(getLP(), NULL);
    if (res != 0)
    {
        cout << "res = " << res << " \n";
    }

    result.resize(getNumCols(), 0);
    objValue = glp_mip_obj_val(getLP());

    for (int i=0; i<getNumCols(); ++i)
    {
        result[i] = glp_mip_col_val(getLP(), i+1);
    }
    return LPSolver::FEASIBLE;
}

int main_mip()
{
    Matrix A(2,2);
    vector<double> b = { 2, 3 };
    vector<double> c = { 1, 1 };
    A[0][0] = 1;
    A[0][1] = 2;
    A[1][0] = 3;
    A[1][1] = 4;
    MIPSolver solver(A, b, c);

    solver.setMaximization();
    solver.setColInteger(0);

    vector<double> result;
    double objVal;
    solver.solve(result, objVal);
    cout << "optimum: " << objVal << endl;
    cout << " x0: " << result[0] << " x1: " << result[1] << endl;
    return 0;
}