//
// NewtonMethod.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "NewtonMethod.h"

#include "MathFunction.h"

#include <iostream>
#include <cmath>

using std::endl;
using std::cout;

namespace {
const double DEFAULT_ERROR = 0.001;
}

NewtonMethod::NewtonMethod(MathFunction<double> &f, MathFunction<double> &derivative)
: m_f(f),
  m_derivative(derivative),
  m_error(DEFAULT_ERROR)
{
}

NewtonMethod::NewtonMethod(const NewtonMethod &p)
: m_f(p.m_f),
  m_derivative(p.m_derivative),
  m_error(p.m_error)
{
}

NewtonMethod::~NewtonMethod()
{
}

NewtonMethod &NewtonMethod::operator=(const NewtonMethod &p)
{
	if (this != &p)
	{
		m_f = p.m_f;
		m_derivative = p.m_derivative;
		m_error = p.m_error;
	}
	return *this;
}

double NewtonMethod::getRoot(double x0)
{
    double x1 = x0;
    do
    {
        x0 = x1;
        cout << " x0 is " << x0 << endl;  // this line just for demonstration
        double d = m_derivative(x0);
        double y = m_f(x0);
        x1 = x0 - y / d;
    }
    while (std::abs(x0 - x1) > m_error);
	return x1;
}

// ---- this is the implementation for an example function and its derivative

namespace {

class F3 : public MathFunction<double> {
public:
	virtual ~F3();
	virtual double operator()(double value);
};

F3::~F3()
{
}

double F3::operator ()(double x)
{
	return (x-1)*(x-1)*(x-1);
}

class DF3 : public MathFunction<double> {
public:
	virtual ~DF3();
	virtual double operator()(double value);
};

// represents the derivative of F3
DF3::~DF3()
{
}

double DF3::operator ()(double x)
{
	return 3*(x-1)*(x-1);
}

}

int main_nm()
{
	F3 f;
	DF3 df;
	NewtonMethod nm(f, df);
	cout << " the root of the function is " << nm.getRoot(100) << endl;
	return 0;
}

