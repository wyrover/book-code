//
//  OptionsProbabilities.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "OptionsProbabilities.h"

#include <boost/random/normal_distribution.hpp>
#include <boost/random.hpp>

using std::vector;
using std::cout;
using std::endl;

static boost::rand48 random_generator;

namespace  {
    const int NUM_ITERATIONS = 1000;
}

OptionsProbabilities::OptionsProbabilities(double initialPrice,
                                           double strike, double avgStep, int nDays)
: m_initialPrice(initialPrice),
  m_strike(strike),
  m_avgStep(avgStep),
  m_numDays(nDays),
  m_numIterations(NUM_ITERATIONS)
{
}

OptionsProbabilities::OptionsProbabilities(const OptionsProbabilities &p)
: m_initialPrice(p.m_initialPrice),
  m_strike(p.m_strike),
  m_avgStep(p.m_avgStep),
  m_numDays(p.m_numDays),
  m_numIterations(p.m_numIterations)
{
}

OptionsProbabilities::~OptionsProbabilities()
{
}

OptionsProbabilities &OptionsProbabilities::operator=(const OptionsProbabilities &p)
{
    if (this != &p)
    {
        m_initialPrice = p.m_initialPrice;
        m_strike = p.m_strike;
        m_avgStep = p.m_avgStep;
        m_numDays = p.m_numDays;
        m_numIterations = p.m_numIterations;
    }
    return *this;
}

void OptionsProbabilities::setNumIterations(int n)
{
    m_numIterations = n;
}

double OptionsProbabilities::probFinishAboveStrike()
{
    int nAbove = 0;
    for (int i=0; i<m_numIterations; ++i)
    {
        double val = getLastPriceOfWalk();
        if (val >= m_strike)
        {
            nAbove++;
        }
    }

    return nAbove/(double)m_numIterations;
}

double OptionsProbabilities::probFinishBelowStrike()
{
    int nBelow = 0;
    for (int i=0; i<m_numIterations; ++i)
    {
        double val = getLastPriceOfWalk();
        if (val <= m_strike)
        {
            nBelow++;
        }
    }

    return nBelow/(double)m_numIterations;
}

double OptionsProbabilities::probFinalPriceBetweenPrices(double lowPrice, double highPrice)
{
    int nBetween = 0;
    for (int i=0; i<m_numIterations; ++i)
    {
        double val = getLastPriceOfWalk();
        if (lowPrice <= val && val <= highPrice)
        {
            nBetween++;
        }
    }

    return nBetween/(double)m_numIterations;
}

double OptionsProbabilities::gaussianValue(double mean, double sigma)
{
	boost::random::normal_distribution<> distrib(mean, sigma);
	return distrib(random_generator);
}

double OptionsProbabilities::getLastPriceOfWalk()
{
    double prev = m_initialPrice;

    for (int i=0; i<m_numDays; ++i)
    {
        double stepSize = gaussianValue(0, m_avgStep);
        int r = rand() % 2;
        double val = prev;
        if (r == 0) val += (stepSize * val);
        else val -= (stepSize * val);
        prev = val;
    }
    return prev;
}

std::vector<double> OptionsProbabilities::getWalk()
{
    vector<double> walk;
    double prev = m_initialPrice;

    for (int i=0; i<m_numDays; ++i)
    {
        double stepSize = gaussianValue(0, m_avgStep);
        int r = rand() % 2;
        double val = prev;
        if (r == 0) val += (stepSize * val);
        else val -= (stepSize * val);
        walk.push_back(val);
        prev = val;
    }
    return walk;
}

int main_optp()
{
    OptionsProbabilities optP(30, 35, 0.01, 100);


    cout << " above strike prob: " << optP.probFinishAboveStrike() << endl;
    cout << " Below strike prob: " << optP.probFinishBelowStrike() << endl;
    cout << " between 28 and 32  prob: " << optP.probFinalPriceBetweenPrices(28, 32) << endl;

    return 0;
}
