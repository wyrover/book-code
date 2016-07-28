//
//  ModifiedCAP.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "ModifiedCAP.h"

#include "LPSolver.h"

#include <iostream>
#include <vector>

using std::vector;
using std::cout;
using std::endl;

ModifiedCAP::ModifiedCAP(int N, int T, double R, Matrix &expectedRet, const vector<double> &ret)
: m_N(N),
  m_T(T),
  m_R(R),
  m_retMatrix(expectedRet),
  m_assetRet(ret)
{
}

ModifiedCAP::ModifiedCAP(const ModifiedCAP &p)
: m_N(p.m_N),
  m_T(p.m_T),
  m_R(p.m_R),
  m_retMatrix(p.m_retMatrix),
  m_assetRet(p.m_assetRet)
{
}

ModifiedCAP::~ModifiedCAP()
{
}

ModifiedCAP &ModifiedCAP::operator=(const ModifiedCAP &p)
{
    if (this != &p)
    {
        m_N = p.m_N;
        m_T = p.m_T;
        m_R = p.m_R;
        m_retMatrix = p.m_retMatrix;
        m_assetRet = p.m_assetRet;
    }
    return *this;
}

void ModifiedCAP::solveModel(std::vector<double> &results, double &objVal)
{
    vector<double> c(m_T + m_N, 0);

    // objective function
    for (int i=m_N; i<m_N+m_T; ++i)
    {
        c[i] = 1;
    }

    // right-hand side vector
    vector<double> b(2*m_T + 2 + 1, 0);
    b[2*m_T]   =  1;
    b[2*m_T+1] = -1;
    b[2*m_T+2] = -m_R;

    // matrix A
    Matrix A(2*m_T + 2 + 1, m_T + m_N);
    for (int i=0; i<m_T; ++i)
    {
        for (int j=0; j<m_N; ++j)
        {
            A[i][j] = m_retMatrix[j][i] - m_assetRet[j];
        }
        A[i][m_N+i] = -1;
    }

    for (int i=m_T; i<2*m_T; ++i)
    {
        for (int j=m_N; j<2*m_N; ++j)
        {
            A[i][j] = - m_retMatrix[j-m_N][i-m_T] + m_assetRet[j-m_N];
        }
        A[i][m_N+i-m_T] = -1;
    }

    for (int j=0; j<m_N; ++j)
    {
        A[2*m_T][j]   = 1;
        A[2*m_T+1][j] = -1;
        A[2*m_T+2][j] = - m_assetRet[j];
    }

    LPSolver solver(A, b, c);
    solver.setMinimization();
    solver.solve(results, objVal);
}

void ModifiedCAP::solveExtendedModel(std::vector<double> &results, double &objVal)
{
    vector<double> c(m_T + m_N, 0);

    // objective function
    for (int i=m_N; i<m_N+m_T; ++i)
    {
        c[i] = 1;
    }

    const double M = 0.37; // maximum of each asset
    const double K_L = 0.15;   // minimum of combined assets 1 and 2

    // right-hand side vector
    vector<double> b(2*m_T + 2 + 1 + m_N + 1 , 0);
    b[2*m_T]   =  1;
    b[2*m_T+1] = -1;
    b[2*m_T+2] = -m_R;

    for (int i=2*m_T+3; i<2*m_T + 3 + m_N; ++i)
    {
        b[i] = M;
    }
    b[2*m_T + 3 + m_N] = -K_L;

    // matrix A
    Matrix A(2*m_T + 2 + 1 + m_N + 1, m_T + m_N);
    for (int i=0; i<m_T; ++i)
    {
        for (int j=0; j<m_N; ++j)
        {
            A[i][j] = m_retMatrix[j][i] - m_assetRet[j];
        }
        A[i][m_N+i] = -1;
    }

    for (int i=m_T; i<2*m_T; ++i)
    {
        for (int j=m_N; j<2*m_N; ++j)
        {
            A[i][j] = - m_retMatrix[j-m_N][i-m_T] + m_assetRet[j-m_N];
        }
        A[i][m_N+i-m_T] = -1;
    }

    for (int j=0; j<m_N; ++j)
    {
        A[2*m_T][j]   = 1;
        A[2*m_T+1][j] = -1;
        A[2*m_T+2][j] = - m_assetRet[j];
    }

    for (int i=2*m_T+3; i<2*m_T+3+m_N; ++i)
    {
        A[i][i-(2*m_T+3)] = 1;
    }
    // constraints for assets 1 and 2
    A[2*m_T+3+m_N][0] = -1;
    A[2*m_T+3+m_N][1] = -1;

    LPSolver solver(A, b, c);
    solver.setMinimization();
    solver.solve(results, objVal);
}

int main_mcap()
{

    // sample problem: 4 assets and 5 periods

    // build the expected return matrix
    double val[][5] = {
        {0.051, 0.050, 0.049, 0.051, 0.05},
        {0.10, 0.099, 0.102, 0.100, 0.101},
        {0.073, 0.077, 0.076, 0.075, 0.076},
        {0.061, 0.06, 0.059, 0.061, 0.062},
    };
    Matrix retMatrix(4, 5);
    for (int i=0; i<4; ++i)
    {
        for (int j=0; j<5; ++j)
        {
            retMatrix[i][j] = val[i][j];
        }
    }

    vector<double> assetReturns = {0.05, 0.10, 0.075, 0.06};

    ModifiedCAP mc(4, 5, 0.08, retMatrix, assetReturns);

    vector<double> results;
    double objVal;
    mc.solveExtendedModel(results, objVal);

    cout << "obj value: " << objVal/5 << endl;
    for (int i=0; i<results.size(); ++i)
    {
        cout << " x" << i << ": " << results[i];
    }
    cout << endl;
    return 0;
}