//
//  MatrixOperations.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "MatrixOperations.h"

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/io.hpp>
#include <boost/numeric/ublas/lu.hpp>

namespace ublas = boost::numeric::ublas;
using std::cout;
using std::endl;

MatrixOperations::MatrixOperations()
{
}

MatrixOperations::~MatrixOperations()
{
}

void MatrixOperations::addRow(const std::vector<double> &row)
{
	m_rows.push_back(row);
}

static Matrix fromMatrix(const ublas::matrix<double> &mp)
{
	using namespace ublas;

	int d1 = mp.size1();
	int d2 = mp.size2();
	Matrix res(d1, d2);
	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			res[i][j] = mp(i,j);
		}
	}
	return res;
}

Matrix MatrixOperations::elementwiseMultiply(Matrix &m)
{
	using namespace ublas;
	int d1 = m_rows.size();
	int d2 = m_rows[0].size();
	matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = m_rows[i][j];
		}
	}

	matrix<double> M2(d1, d2);
	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M2(i,j) = m[i][j];
		}
	}

	matrix<double> mp = element_prod(M, M2);
	return fromMatrix(mp);
}

Matrix MatrixOperations::transpose()
{
	using namespace ublas;
	int d1 = m_rows.size();
	int d2 = m_rows[0].size();
	matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = m_rows[i][j];
		}
	}

	matrix<double> mp = trans(M);
	return fromMatrix(mp);
}

Matrix MatrixOperations::multiply(Matrix &m)
{
	using namespace ublas;
	int d1 = m_rows.size();
	int d2 = m_rows[0].size();
	matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = m_rows[i][j];
		}
	}

	matrix<double> M2(d1, d2);
	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M2(i,j) = m[i][j];
		}
	}

    matrix<double> mp = prod(M, M2);
    return fromMatrix(mp);
}

Matrix MatrixOperations::scalarMultiply(double scalar)
{
	using namespace ublas;
	int d1 = m_rows.size();
	int d2 = m_rows[0].size();
	matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = m_rows[i][j];
		}
	}

	matrix<double> mp = scalar * M;
	return fromMatrix(mp);
}

std::vector<double> MatrixOperations::preMultiply(const std::vector<double> &v)
{
	using namespace ublas;
	ublas::vector<double> vec;
	std::copy(v.begin(), v.end(), vec.end());

	int d1 = m_rows.size();
	int d2 = m_rows[0].size();
	ublas::matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = m_rows[i][j];
		}
	}

	vector<double> pv = prod(vec, M);

	std::vector<double> res;
	std::copy(pv.begin(), pv.end(), res.end());
	return res;
}

std::vector<double> MatrixOperations::postMultiply(const std::vector<double> &v)
{
	using namespace ublas;
	ublas::vector<double> vec;
	std::copy(v.begin(), v.end(), vec.end());

	int d1 = m_rows.size();
	int d2 = m_rows[0].size();
	ublas::matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = m_rows[i][j];
		}
	}

	vector<double> pv = prod(M, vec);

	std::vector<double> res;
	std::copy(pv.begin(), pv.end(), res.end());
	return res;
}

int main_matrixop()
{
    MatrixOperations op;
    for (int i=0; i<5; ++i)
    {
        std::vector<double> row;
        for (int j=0; j<5; ++j)
        {
            row.push_back(sin((double)j+i));
        }
        op.addRow(row);
    }
    op.transpose();
    Matrix res = op.scalarMultiply(12);
    for (int i=0; i<5; ++i)
    {
        std::vector<double> row;
        for (int j=0; j<5; ++j)
        {
            cout << res[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}


// ---------------------------------------------------------------------------


static ublas::matrix<double> matrixFromRows(const std::vector<std::vector<double> > &rows)
{
	using namespace ublas;
	int d1 = rows.size();
	int d2 = rows[0].size();
	matrix<double> M(d1, d2);

	for (int i = 0; i < d1; ++i)
	{
		for (int j = 0; j < d2; ++j)
		{
			M(i,j) = rows[i][j];
		}
	}
	return M;
}


// -----------------


// The sign is calculated from a given permutation.
// Just flip the sign for each change in permutation.
int getDeterminantSign(const ublas::permutation_matrix<std::size_t>& pm)
{
    int sign = 1;
    for (int i = 0; i < pm.size(); ++i)
    {
        if (i != pm(i))
        {
            sign *= -1.0;
        }
    }
    return sign;
}

double computeDeterminant(ublas::matrix<double>& m )
{
    ublas::permutation_matrix<std::size_t> pm(m.size1());

    double det = 1.0;
    if (ublas::lu_factorize(m,pm))
    {
        det = 0.0;
    }
    else
    {
        for(int i = 0; i < m.size1(); i++)
        {
            det *= m(i,i);
        }
        det = det * getDeterminantSign( pm );
    }
    return det;
}

void determinantSample()
{
    ublas::matrix<double> M(3, 3);

    for (unsigned i = 0; i < M.size1() ; ++i)
    {
        for (unsigned j = 0; j < M.size2() ; ++j)
        {
            M(i,j) = sin(3 * j);
        }
    }

    double determinant = computeDeterminant(M);

    cout << " determinant value is " << determinant << " for matrix " << M << endl;
}
