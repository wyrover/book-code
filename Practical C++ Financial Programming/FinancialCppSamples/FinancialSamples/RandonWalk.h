//
//  RandonWalk.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__RandonWalk__
#define __FinancialSamples__RandonWalk__

#include <vector>

// Simple random walk for price simulation
class RandomWalk {
public:
    RandomWalk(int size, double start, double step);
    RandomWalk(const RandomWalk &p);
    ~RandomWalk();
    RandomWalk &operator=(const RandomWalk &p);

    std::vector<double> getWalk();
private:
    int m_size;     // number of steps
    double m_step;  // size of each step (in percentage)
    double m_start; // starting price
};


#endif /* defined(__FinancialSamples__RandonWalk__) */
