//
//  ResourceAlloc.h
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#ifndef __FinancialSamples__ResourceAlloc__
#define __FinancialSamples__ResourceAlloc__

#include <vector>

class ResourceAlloc {
public:
    ResourceAlloc(std::vector<double> &result, double &objVal);
    ResourceAlloc(const ResourceAlloc &p);
    ~ResourceAlloc();
    ResourceAlloc &operator=(const ResourceAlloc &p);

    void solveProblem();
private:
    std::vector<double> &m_results;
    double &m_objVal;
};


#endif /* defined(__FinancialSamples__ResourceAlloc__) */
