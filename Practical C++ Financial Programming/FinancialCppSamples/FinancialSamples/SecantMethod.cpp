//
// SecantMethod.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net


#include "SecantMethod.h"

#include "MathFunction.h"
#include <iostream>
#include <cmath>

using std::cout;
using std::endl;

namespace {
const double DEFAULT_ERROR = 0.001;
}

SecantMethod::SecantMethod(MathFunction<double> &f)
: m_f(f),
  m_error(DEFAULT_ERROR)
{
}

SecantMethod::SecantMethod(const SecantMethod &p)
: m_f(p.m_f),
  m_error(p.m_error)
{
}

SecantMethod::~SecantMethod()
{
}

SecantMethod &SecantMethod::operator=(const SecantMethod &p)
{
	if (this != &p)
	{
		m_f = p.m_f;
		m_error = p.m_error;
	}
	return *this;
}

double SecantMethod::getRoot(double a, double b)
{
	double root = 0;
    double fa = m_f(a);
    double fb = m_f(b);
    double c = 0, fc = 0;
	do
	{
        c = b - fb*(b-a)/(fb-fa);
		root = c;
        fc = m_f(c);

        cout << "-> " << c << " " << fc << " " << endl; // this line just for demonstration

        a = b;
        fa = fb;
        b = c;
        fb = fc;
    }
    while (std::abs(a - b) > m_error);
	return root;
}

// ---- this is the implementation for an example function

namespace {

class F2 : public MathFunction<double> {
public:
	virtual ~F2();
	virtual double operator()(double value);
};

F2::~F2()
{
}

double F2::operator ()(double x)
{
	return (x-1)*(x-1)*(x-1);
//    return x*x*x + 4*x*x - 10;
}

}

int main_sec()
{
	F2 f;
	SecantMethod sm(f);
    double root = sm.getRoot(-10, 10);
	cout << " the root of the function is " << root << endl;
	return 0;
}

