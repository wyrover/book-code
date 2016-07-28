//
//  ParallelRandomWalk.cpp

#include "ParallelRandomWalk.h"

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


ParallelRandomWalk::ParallelRandomWalk(int size, double start, double step)
: m_size(size),
  m_start(start),
  m_step(step)
{
}

ParallelRandomWalk::ParallelRandomWalk(const ParallelRandomWalk &p)
: m_size(p.m_size),
  m_start(p.m_start),
  m_step(p.m_step)
{
}

ParallelRandomWalk::~ParallelRandomWalk()
{
}

ParallelRandomWalk &ParallelRandomWalk::operator=(const ParallelRandomWalk &p)
{
    if (this != &p)
    {
        m_size = p.m_size;
        m_start = p.m_start;
        m_step = p.m_step;
    }
    return *this;
}

double ParallelRandomWalk::probFinishAboveStrike()
{
    const int numThreads = 20;

    vector<RandomWalkThread*> threads(numThreads);
    for (int i=0; i<numThreads; ++i)
    {
        threads[i] = new RandomWalkThread(m_size, m_step, m_start);
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
    }

    return nAbove/(double)(numThreads);
}


int main()
{
    ParallelRandomWalk rw(100, 50.0, 52.0);
    double r = rw.probFinishAboveStrike();
    cout << " result is " << r << endl;
    return 0;
}