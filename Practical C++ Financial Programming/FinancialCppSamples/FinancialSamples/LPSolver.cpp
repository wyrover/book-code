//
//  LPSolver.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "LPSolver.h"

#include <glpk.h>

#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

LPSolver::LPSolver(Matrix &m, const vector<double> &b, const vector<double> &c)
: m_M(m.numRows()),
  m_N(m[0].size()),
  m_c(c),
  m_b(b),
  m_A(m),
  m_lp(glp_create_prob())
{
    initProblem(m_M, m_N);
}


LPSolver::LPSolver(const LPSolver &p)
: m_M(p.m_M),
  m_N(p.m_N),
  m_c(p.m_c),
  m_b(p.m_b),
  m_A(p.m_A),
  m_lp(glp_create_prob())
{
    initProblem(m_M, m_N);
}

// performs necessary initialization of the given values
void LPSolver::initProblem(size_t M, size_t N)
{
    if (!m_lp) return;

    setRowBounds();
    setColumnCoefs();

    vector<int> I, J;
    vector<double> V;

    // indices in GLPK start on 1
    I.push_back(0);
    J.push_back(0);
    V.push_back(0);
    for (int i=0; i<M; ++i)
    {
        for (int j=0; j<N; ++j)
        {
            I.push_back(i+1);
            J.push_back(j+1);
            V.push_back(m_A[i][j]);
        }
    }
    glp_load_matrix(m_lp, (int)(m_M * m_N), &I[0], &J[0], &V[0]);
}

LPSolver::~LPSolver()
{
    glp_delete_prob(m_lp);
}

LPSolver &LPSolver::operator=(const LPSolver &p)
{
    if (this != &p)
    {
        m_M = p.m_M;
        m_N = p.m_N;
        m_c = p.m_c;
        m_b = p.m_b;
        m_A = p.m_A;
        m_lp = glp_create_prob();
        initProblem(m_M, m_N);
    }
    return *this;
}

void LPSolver::setName(const std::string &s)
{
    glp_set_prob_name(m_lp, s.c_str());
}

bool LPSolver::isValid()
{
    return m_lp != NULL;
}

void LPSolver::setMaximization()
{
    glp_set_obj_dir(m_lp, GLP_MAX);
}

void LPSolver::setMinimization()
{
    glp_set_obj_dir(m_lp, GLP_MIN);
}

void LPSolver::setRowBounds()
{
    glp_add_rows(m_lp, (int)m_M);
    for (int i=0; i<m_M; ++i)
    {
        glp_set_row_bnds(m_lp, i+1, GLP_UP, 0.0, m_b[i]);
    }
}

void LPSolver::setColumnCoefs()
{
    glp_add_cols(m_lp, (int)m_N);
    for (int j=0; j<m_N; ++j)
    {
        glp_set_col_bnds(m_lp, j+1, GLP_LO, 0.0, 0.0);
        glp_set_obj_coef(m_lp, j+1, m_c[j]);
    }
}

LPSolver::ResultType LPSolver::solve(std::vector<double> &result, double &objValue)
{

#if 0  /// debug print
    for (int i=0; i<m_M; ++i) {
        for (int j=0; j<m_N; ++j)
            cout << " " << m_A[i][j];
        cout << endl;
    }
#endif

    glp_simplex(m_lp, NULL);
    result.resize(m_N, 0);
    objValue = glp_get_obj_val(m_lp);

    for (int j=0; j<m_N; ++j)
    {
        result[j] = glp_get_col_prim(m_lp, j+1);
    }
    return LPSolver::FEASIBLE;
}

glp_prob *LPSolver::getLP()
{
    return m_lp;
}

int LPSolver::getNumCols()
{
    return (int)m_N;
}

int LPSolver::getNumRows()
{
    return (int)m_M;
}

int main_lps()
{
    Matrix A(3);
    A[0][0] = 1;  A[0][1] = 1; A[0][2] = 1;
    A[1][0] = 10; A[1][1] = 2; A[1][2] = 4;
    A[2][0] = 2;  A[2][1] = 5; A[2][2] = 6;

    vector<double> c = { 10, 6, 4 };
    vector<double> b = { 100, 600, 300 };

    LPSolver solver(A, b, c);
    solver.setMaximization();
    vector<double> results;
    double objVal;
    solver.solve(results, objVal);
    for (int i=0; i<results.size(); ++i)
    {
        cout << " x" << i << ": " << results[i];
    }
    cout << " max: " << objVal << endl;
    return 0;
}
