//
// MonteCarloIntegration.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "MonteCarloIntegration.h"

#include <cmath>
#include <cstdlib>
#include <iostream>

#include "MathFunction.h"

#include <boost/math/distributions.hpp>

#include <boost/random.hpp>
#include <boost/random/uniform_real.hpp>

static boost::rand48 random_generator;

using std::cout;
using std::endl;

namespace {
	const int DEFAULT_NUM_SAMPLES = 1000;
}

MonteCarloIntegration::MonteCarloIntegration(MathFunction<double>& f)
: m_f(f),
  m_numSamples(DEFAULT_NUM_SAMPLES)
{
}

MonteCarloIntegration::MonteCarloIntegration(const MonteCarloIntegration& p)
: m_f(p.m_f),
  m_numSamples(p.m_numSamples)
{
}

MonteCarloIntegration::~MonteCarloIntegration()
{
}

MonteCarloIntegration& MonteCarloIntegration::operator =(const MonteCarloIntegration& p)
{
	if (this != &p)
	{
		m_f = p.m_f;
		m_numSamples = p.m_numSamples;
	}
	return *this;
}

void MonteCarloIntegration::setNumSamples(int n)
{
	m_numSamples = n;
}

double MonteCarloIntegration::integrateRegion(double a, double b, double min, double max)
{
    boost::random::uniform_real_distribution<> xDistrib(a, b);
    boost::random::uniform_real_distribution<> yDistrib(min, max);

    int pointsIn = 0;
    int pointsOut = 0;
    bool positive = max > 0;

    for (int i = 0; i < m_numSamples; ++i)
    {
        double x = xDistrib(random_generator);
        double y = m_f(x);

        double ry = yDistrib(random_generator);
        if (positive && min <= ry && ry <= y)
        {
            pointsIn++;
        }
        else if (!positive && y <= ry && ry <= max)
        {
            pointsIn++;
        }
        else
        {
            pointsOut++;
        }
    }

    double percentageArea = 0;
    if (pointsIn+pointsOut > 0)
    {
        percentageArea = pointsIn / double(pointsIn + pointsOut);
    }

    if (percentageArea > 0)
    {
        return (b-a) * (max-min) * percentageArea;
    }

    return 0;
}

double MonteCarloIntegration::getIntegral(double a, double b)
{
    boost::random::uniform_real_distribution<> distrib(a, b);

    double max = 0;
    double min = 0;

    for (int i = 0; i < m_numSamples; ++i)
    {
        double x = distrib(random_generator);
        double y = m_f(x);

        if (y > max)
        {
            max = y;
        }
        if (y < min)
        {
            min = y;
        }
    }
    double positiveIntg = max > 0 ? integrateRegion(a, b, 0, max) : 0;
    double negativeIntg = min < 0 ? integrateRegion(a, b, min, 0) : 0;
    return positiveIntg - negativeIntg;
}

// Example function

namespace  {

    class FSin : public MathFunction<double>
    {
    public:
        ~FSin();
        double operator()(double x);
    };


    FSin::~FSin()
    {
    }

    double FSin::operator()(double x)
    {
        return sin(x);
    }

}


int main()
{
	cout << "starting" << endl;
    FSin f;
    MonteCarloIntegration mci(f);
    double integral = mci.getIntegral(0.5, 4.9);
    cout << " the integral of the function is " << integral << endl;

	mci.setNumSamples(200000);
    integral = mci.getIntegral(0.5, 4.9);
    cout << " the integral of the function with 20000 intervals is " << integral << endl;
    return 0;
}

