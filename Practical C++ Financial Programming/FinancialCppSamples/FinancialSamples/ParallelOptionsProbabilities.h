//
//  ParallelRandomWalk.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__ParallelRandomWalk__
#define __FinancialSamples__ParallelRandomWalk__

class ParallelOptionsProbabilities  {
public:
    ParallelOptionsProbabilities(int size, double strike, double sigma);
    ParallelOptionsProbabilities(const ParallelOptionsProbabilities &p);
    ~ParallelOptionsProbabilities();
    ParallelOptionsProbabilities &operator=(const ParallelOptionsProbabilities &p);

    double probFinishAboveStrike();

private:
    int m_numSteps;     // number of steps
    double m_step;  // size of each step (in percentage)
    double m_strikePrice; // starting price
};

#endif /* defined(__FinancialSamples__ParallelRandomWalk__) */
