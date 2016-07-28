//
// BisectionMethod.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "BisectionMethod.h"

#include "MathFunction.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

namespace {
const double DEFAULT_ERROR = 0.001;
}

BisectionMethod::BisectionMethod(MathFunction<double> &f)
: m_f(f),
  m_error(DEFAULT_ERROR)
{
}

BisectionMethod::BisectionMethod(const BisectionMethod &p)
: m_f(p.m_f),
  m_error(p.m_error)
{
}


BisectionMethod::~BisectionMethod()
{
}

BisectionMethod &BisectionMethod::operator =(const BisectionMethod &p)
{
	if (this != &p)
	{
		m_f = p.m_f;
		m_error = p.m_error;
	}
	return *this;
}

double BisectionMethod::getRoot(double x1, double x2)
{
	double root = 0;
	while (std::abs(x1 - x2) > m_error)
	{
		double x3 = (x1 + x2) / 2;
		root = x3;
        cout << "root is " << x3 << endl;  // this line just for demonstration
		if (m_f(x1) * m_f(x3) < 0)
		{
			x2 = x3;
		}
		else
		{
			x1 = x3;
		}
		if (m_f(x1) * m_f(x2) > 0)
		{
			cout << " function does not converge "  << endl;
			break;
		}
	}
	return root;
}

// ---- this is the implementation for an example function

namespace {

class F1 : public MathFunction<double> {
public:
	virtual ~F1();
	virtual double operator()(double value);
};

F1::~F1()
{
}

double F1::operator ()(double x)
{
	return (x-1)*(x-1)*(x-1);
}

}

int main_bisec()
{
	F1 f;
	BisectionMethod bm(f);
	cout << " the root of the function is " << bm.getRoot(-100, 100) << endl;
	return 0;
}
