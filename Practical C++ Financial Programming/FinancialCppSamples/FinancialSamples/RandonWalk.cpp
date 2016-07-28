//
//  RandonWalk.cpp
//
// (c) 2015 Carlos Oliveira
// This code is part of the book "Practical C++ Financial Programming"
//    by Carlos Oliveira, Apress, 2015.
// For more information, visit http://coliveira.net

#include "RandonWalk.h"

#include <cstdlib>
#include <iostream>

using std::vector;
using std::cout;
using std::endl;

RandomWalk::RandomWalk(int size, double start, double step)
: m_size(size),
  m_step(step),
  m_start(start)
{
}

RandomWalk::RandomWalk(const RandomWalk &p)
: m_size(p.m_size),
  m_step(p.m_step),
  m_start(p.m_start)
{
}

RandomWalk::~RandomWalk()
{
}

RandomWalk &RandomWalk::operator=(const RandomWalk &p)
{
    if (this != &p)
    {
        m_size = p.m_size;
        m_step = p.m_step;
        m_start = p.m_start;
    }
    return *this;
}

std::vector<double> RandomWalk::getWalk()
{
    vector<double> walk;
    double prev = m_start;

    for (int i=0; i<m_size; ++i)
    {
        int r = rand() % 3;
        double val = prev;
        if (r == 0) val += (m_step * val);
        else if (r == 1) val -= (m_step * val);
        walk.push_back(val);
        prev = val;
    }
    return walk;
}

int main_rw()
{
    RandomWalk rw(100, 30, 0.01);
    vector<double> walk = rw.getWalk();
    for (int i=0; i<walk.size(); ++i)
    {
        cout << ", " << i << ", " << walk[i];
    }
    cout << endl;
    return 0;
}
