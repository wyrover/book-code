//
//  ParallelOptionsProbabilities.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "ParallelOptionsProbabilities.h"

#include "Thread.h"

#include <pthread.h>
#include <cstdlib>
#include <vector>


#include <boost/random/normal_distribution.hpp>
#include <boost/random.hpp>

using std::vector;
using std::cout;
using std::endl;

static boost::rand48 random_generator;

using std::vector;

/// ---


class RandomWalkThread : public Thread {
public:
    RandomWalkThread(int num_steps, double sigma, double startPrice);
    ~RandomWalkThread();
    virtual void run();

    double gaussianValue(double mean, double sigma);
    double getLastPriceOfWalk();

    double result();
private:
    int m_numberOfSteps;     // number of steps
    double m_sigma;          // size of each step (in percentage)
    double m_startingPrice;  // starting price

    double m_result;
};


RandomWalkThread::RandomWalkThread(int numSteps, double sigma, double startingPrice)
: m_numberOfSteps(numSteps),
  m_sigma(sigma),
  m_startingPrice(startingPrice)
{
}

RandomWalkThread::~RandomWalkThread()
{
}

double RandomWalkThread::gaussianValue(double mean, double sigma)
{
    boost::random::normal_distribution<> distrib(mean, sigma);
    return distrib(random_generator);
}

double RandomWalkThread::result()
{
    return m_result;
}

double RandomWalkThread::getLastPriceOfWalk()
{
    double prev = m_startingPrice;

    for (int i=0; i<m_numberOfSteps; ++i)
    {
        double stepSize = gaussianValue(0, m_sigma);
        int r = rand() % 2;
        double val = prev;
        if (r == 0) val += (stepSize * val);
        else val -= (stepSize * val);
        prev = val;
    }
    return prev;
}

void RandomWalkThread::run()
{

    cout << " running thread " << endl;

    int nAbove = 0;
    for (int i=0; i<m_numberOfSteps; ++i)
    {
        double val = getLastPriceOfWalk();
        if (val >= m_startingPrice)
        {
            nAbove++;
        }
    }

    m_result = nAbove/(double)m_numberOfSteps;
}

// ---


ParallelOptionsProbabilities::ParallelOptionsProbabilities(int size, double start, double step)
: m_numSteps(size),
  m_strikePrice(start),
  m_step(step)
{
}

ParallelOptionsProbabilities::ParallelOptionsProbabilities(const ParallelOptionsProbabilities &p)
: m_numSteps(p.m_numSteps),
  m_strikePrice(p.m_strikePrice),
  m_step(p.m_step)
{
}

ParallelOptionsProbabilities::~ParallelOptionsProbabilities()
{
}

ParallelOptionsProbabilities &ParallelOptionsProbabilities::operator=(const ParallelOptionsProbabilities &p)
{
    if (this != &p)
    {
        m_numSteps = p.m_numSteps;
        m_strikePrice = p.m_strikePrice;
        m_step = p.m_step;
    }
    return *this;
}

double ParallelOptionsProbabilities::probFinishAboveStrike()
{
    const int numThreads = 20;

    vector<RandomWalkThread*> threads(numThreads);
    for (int i=0; i<numThreads; ++i)
    {
        threads[i] = new RandomWalkThread(m_numSteps, m_step, m_strikePrice);
        threads[i]->setJoinable(true);
        threads[i]->start();
    }

    for (int i=0; i<numThreads; ++i)
    {
        threads[i]->join();
    }

    double nAbove = 0;
    for (int i=0; i<numThreads; ++i)
    {
        nAbove += threads[i]->result();
        delete threads[i];
    }

    return nAbove/(double)(numThreads);
}


int main_parprob()
{
    ParallelOptionsProbabilities rw(100, 50.0, 52.0);
    double r = rw.probFinishAboveStrike();
    cout << " result is " << r << endl;
    return 0;
}